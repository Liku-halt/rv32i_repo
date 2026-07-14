//this lib is for AS5600 hall-eff sensor
//created by - bk201
#include "rv32i.h"
#include <stdint.h>
#include <inttypes.h>

//this is neaded
void _putchar(char c);

//config registers
const uint8_t zmco = 0x00; //this is read only
const uint8_t zpos = 0x01; // or ox02 // this is a r-w registers
const uint8_t mpos = 0x03; // or 0x04
const uint8_t mang = 0x05; // or 0x06
const uint8_t conf = 0x07; //or 0x08
//
//output registers
const uint8_t raw_most_sig =  0x0c;
const uint8_t raw_least_sig = 0x0d;
const uint8_t angle_most_sig =  0x0e;
const uint8_t angle_least_sig = 0x0f;
//
//status registers
const uint8_t status_ = 0x0b;
const uint8_t mag_status = 0x1a; //this tells if the magnet is too close too far if mag is detected
//
//devie addr
const uint8_t as5600 = 0x36;
//
//read read write 
const bool as_r = 0x01;
const bool as_w = 0x00;
//
//actual values
const uint8_t as_output_mode[3] = {0, 1, 2};
const uint8_t as_power_mode[5]  = {0, 1, 2, 3, 4};
const uint8_t as_pwm_freq[5]    = {0, 1, 2, 3, 4};
const uint8_t as_pwm_hyst[5]    = {0, 1, 2, 3, 4};
const uint8_t as_low_filter[5]  = {0, 1, 2, 3, 4};
const uint8_t as_fast_filter[8] = {0, 1, 2, 3, 4, 5, 6, 7};
const uint8_t as_watch_dog[2]   = {0,1};

//////////////////////////////////////////////////////////////////
uint8_t as5600_begin(void){
  uint8_t ack = 0;
  ack = i2c_write_address(as5600<<1 | as_w );
  if(ack==0){
   uart_print(" as5600 is not responding!!!\r\n");
   return 0;
  }
  printf("the ack: "PRIu8" \r\n",ack);
  i2c_write_byte(status_);
  i2c_stop();
  i2c_write_address(as5600<<1 | as_r);
  uint8_t recived_data = i2c_read_byte();
  i2c_stop();
  printf("status of sensor: %"PRIx8" \r\n", recived_data);//x8 to print in hex form u8 for intiger form
  return ack; 
}
