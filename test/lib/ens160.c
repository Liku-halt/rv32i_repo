

//this is the driver file for ENS160 sensor
//written by bk201
//
#include <stdint.h>
#include "rv32i.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

//
//
//registers
/*const uint8_t device_addr = 0x52 //remember that u have to shift a bit and add 1 or 0 for write read
const uint8_t op_mode = 0x10;
const uint8_t config = 0x11;
const uint8_t command = 0x12;
const uint8_t temp_in = 0x13;
const uint8_t rh_in = 0x15;
const uint8_t device_status = 0x20;
const uint8_t data_aqi = 0x21;
const uint8_t data_tvoc = 0x22;
const uint8_t data_eco2 = 0x24;
const uint8_t data_t = 0x30;
const uint8_t data_rh = 0x32;*/

//making a bit more prof^nal
//ok i am copyting the registers addresses 
//form the dfrobot lib 
/* ENS160 register address *////
#define ENS160_PART_ID_REG     ((uint8_t)0x00)   ///< This 2-byte register contains the part number in little endian of the ENS160.
#define rtn_addr               ((uint16_t)0x0160)
#define device_addr            ((uint8_t)0x52)
#define ENS160_OPMODE_REG      ((uint8_t)0x10)   ///< This 1-byte register sets the Operating Mode of the ENS160.
#define ENS160_CONFIG_REG      ((uint8_t)0x11)   ///< This 1-byte register configures the action of the INTn pin.
#define ENS160_COMMAND_REG     ((uint8_t)0x12)   ///< This 1-byte register allows some additional commands to be executed on the ENS160.
#define ENS160_TEMP_IN_REG     ((uint8_t)0x13)   ///< This 2-byte register allows the host system to write ambient temperature data to ENS160 for compensation.
#define ENS160_RH_IN_REG       ((uint8_t)0x15)   ///< This 2-byte register allows the host system to write relative humidity data to ENS160 for compensation.
#define ENS160_DATA_STATUS_REG ((uint8_t)0x20)   ///< This 1-byte register indicates the current STATUS of the ENS160.
#define ENS160_DATA_AQI_REG    ((uint8_t)0x21)   ///< This 1-byte register reports the calculated Air Quality Index according to the UBA.
#define ENS160_DATA_TVOC_REG   ((uint8_t)0x22)   ///< This 2-byte register reports the calculated TVOC concentration in ppb.
#define ENS160_DATA_ECO2_REG   ((uint8_t)0x24)   ///< This 2-byte register reports the calculated equivalent CO2-concentration in ppm, based on the detected VOCs and hydrogen.
#define ENS160_DATA_ETOH_REG   ((uint8_t)0x22)   ///< This 2-byte register reports the calculated ethanol concentration in ppb.
#define ENS160_DATA_T_REG      ((uint8_t)0x30)   ///< This 2-byte register reports the temperature used in its calculations (taken from TEMP_IN, if supplied).
#define ENS160_DATA_RH_REG     ((uint8_t)0x32)   ///< This 2-byte register reports the relative humidity used in its calculations (taken from RH_IN if supplied).
#define ENS160_DATA_MISR_REG   ((uint8_t)0x38)   ///< This 1-byte register reports the calculated checksum of the previous DATA_ read transaction (of n-bytes).
#define ENS160_GPR_WRITE_REG   ((uint8_t)0x40)   ///< This 8-byte register is used by several functions for the Host System to pass data to the ENS160.
#define ENS160_GPR_READ_REG    ((uint8_t)0x48)   ///< This 8-byte register is used by several functions for the ENS160 to pass data to the Host System.
/* cmd(0x12) register command */
////////
#define ENS160_COMMAND_NOP          ((uint8_t)0x00)   /// reserved. No command.
#define ENS160_COMMAND_GET_APPVER   ((uint8_t)0x0E)   /// Get fw Version Command.
#define ENS160_COMMAND_CLRGPR       ((uint8_t)0xCC)   /// Clears GPR Read Registers Command.
/* opcode(Address 0x10) register mode */
///////
#define ENS160_SLEEP_MODE      ((uint8_t)0x00)   /// DEEP SLEEP mode (low power standby).
#define ENS160_IDLE_MODE       ((uint8_t)0x01)   /// IDLE mode (low-power).
#define ENS160_STANDARD_MODE   ((uint8_t)0x02)   /// STANDARD Gas Sensing Modes.
///
///Appearntely this is the id that the sensor will send ok
#define ENS160_PART_ID   ((uint16_t)0x160)   ///< ENS160 chip version
///
#define write 1 
#define read 0


/* Convenience Macro */
//ok this one is impressive
#define ENS160_CONCAT_BYTES(msb, lsb)   (((uint16_t)msb << 8) | (uint16_t)lsb)   ///< Macro combines two 8-bit data into one 16-bit data

//well now enums 
//
//opp_mode enums
typedef enum {
  deep_sleep,  
  idel,  //idle will be used if its a long reading process
  stnadard //this is what we will use 
}op_modes;
//
//for command that i think i will not use
//
//
typedef enum {
  clr_grp = 0xcc,
  firm_ver = 0xce
}cmds_;

typedef enum {
  normal_op,  //normal  operatin mode
  warm_phase, //starting warmup phase
  init_phase,  // initial startup state
  inval_mode = 1, //invalid output 6th bit
  //
  op_mode_running = 1, //7th bit 
  //
  new_data = 1,// now data is availble in the dat reg  ohh its bit 2
  new_grp = 1  // bit 1
}status_;



uint8_t init_ens160(void) {
  uint8_t ack = 0;
  uint8_t bfr[2];
  ack = i2c_write_address(device_addr<<1 | write);
  vTaskDelay(pdMS_TO_TICKS(1));

  if(ack == 0){
    uart_print("ENS160 NOT RESPONDING!!\r\n FAILED TO INIT\n");
    return 0;
  }else{
    uart_print("ENS160 ACKED!\r\n");
  ack = i2c_write_byte(ENS160_PART_ID);
 
   i2c_stop();
    vTaskDelay(pdMS_TO_TICKS(1));
    ack = i2c_write_address(device_addr<<1 | read);
    vTaskDelay(pdMS_TO_TICKS(1));
    bfr[0] = i2c_read_byte();
    bfr[1] = i2c_read_byte();
    i2c_stop();
    if(ENS160_CONCAT_BYTES(bfr[1],bfr[0]) == rtn_addr ) uart_print("RETURN ADDR CONFED DEVICE RESPONDING!\r\n");
    
    return 1;
  }

}
void ens_160_op_mode_init(op_modes op){
  uint8_t ack = 0;
  uint8_t bfr[2];
  ack = i2c_write_address(device_addr<<1 | write);
  vTaskDelay(pdMS_TO_TICKS(1));
  ack = i2c_write_byte(ENS160_OPMODE_REG);
  if(ack)uart_print("INITIATING THE MODE! ");
  vTaskDelay( pdMS_TO_TICKS(0.5));
  ack = i2c_write_byte(op);
  if(ack) uart_print("MODE INITIATION COMPLETE.");


 






   
}






