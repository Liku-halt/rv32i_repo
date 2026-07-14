///////////////////////////////////////////////////////
// this is a custom bootloader for the anjelo jecobe/ riscv project 
// written by bk201
// pointer addr for the app, that is mem map starts form 0x00000400 
//the deafult uart bud rate is 96000 
///////////////////////////////////////////////////////
#include <stdint.h>
#include "rv32i.h"
//
//boot pin 
//to go to dfu or firmeare download mode or boot loader update mode 
//first press rest then boot pin and hold it in place then release the rest pin 
//u will enter the dfu mode
//
const unit8_t boot_pin = 0;
//
//the operation start check const
//simply it means that if we first send this val 
//then we stant downloading the hex file
const uint32_t = 0xAA;
//
//a bit of macros i will need maybe
//#define check_number (1<<boot_pin)
//
//pinter to the addresses
//
volatile uint32_t *gpio_mode = (volatile *uint32_t )GPIO_MODE;
volatile uint32_t *gpio_read = (volatile *uint32_t )GPIO_READ;
//
//uart addresses
volatile uint32_t *uart_tx_data = (volatile uint32_t *) UART_TX_DATA;
volatile uint32_t *uart_tx_busy = (volatile uint32_t *) UART_TX_BUSY;
volatile uint32_t *uart_rx_full = (volatile uint32_t *) UART_RX_BUFFER_FULL;
volatile uint32_t *uart_rx_data = (volatile uint32_t *) UART_RX_DATA;
//
//there are functions to read pins in the rv32i.h but i wanted to use the fancy logic bitwise operations for this so yeah..
//
//
////////////////////////////////////////////////////////////////
void main(void){
  //i think making the whole 32 bit reg = 0 puts every pin into read mode
  //well..
  //
  *gpio_mode = 0x00;
  //main logic to enter the bootloader mode for digital firmeare update
  unit_32t result = (*gpio_read) & (1<<boot_pin);
  if( result ){
    while(!uart_available());
    //ok now its available 
    
    
    //well we have entered the bootloader mode 
    //now we will wait for the 0xAA to be written  here so that we can start 
    //downloading the firmware 
    //

  }else{
    //we did not enter the bootloader mode not pointing to the app pointer
    //
  }


}
//////////////////////////////////////////////////////////////////////////
//yup functions 
//
//uart_available fxn 
bool uart_available(void){
  bool res = *uart_rx_full & 1<<0;
  return res;
}
//
//
//read function for my uart
char my_uart_read(void){
  while(uart_available());
  char res = (char)*uart_rx_full
  //ok this function is blocking 
  return res;
}



