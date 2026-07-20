#include <stdint.h>
#include <rv32i.h>

//
//
float read_zeo7 ( uart *my_uart){
//this one is the question itself
static const uint8_t cmd[9] = {0xFF, 0x01, 0x86, 0x00, 0x00, 0x00, 0x00, 0x00, 0x79};


  uint8_t array[9] = {0};

  my_uart_send_byte(my_uart, cmd, 9);

  do {
    array[0] = my_uart_read_byte(my_uart);
   } while (array[0]!= 0xFF); 
  //so the sensor's first byte is oxff in question ans mode
   for(int i = 1; i<=8; i++){
    array[i] = my_uart_read_byte(my_uart);

  }
  //so byte 2 is high byte then byte 3 is low byte 
  //so according to the data sheet
  //Gas concentration value = (High Byte*256+Low Byte) x 0.1
  return ((float)(array[2]<<8 | array[3]))* 0.1f;

}

void set_mode_zeo7 ( uart *my_uart){
  //this sets the mode 
  static const uint8_t question[9] = {0xFF, 0x01, 0x78, 0x41, 0x00, 0x00, 0x00, 0x00, 0x46};
  my_uart_send_byte(my_uart, question, 9);

}
