///this is the script for spi 
///written by bk201
///
#include <stdint.h>
#include "rv32i.h"

volatile uint32_t *spi_data = (volatile uint32_t *) (SPI_DATA);
volatile uint32_t *spi_status = (volatile uint32_t *) (SPI_STATUS);
volatile uint32_t *spi_rx_rdy = (volatile uint32_t *) (SPI_RX_RDY);
//
//
//ok this is deafult by mode 0 
//
//this function does thw work like this- 
//send
//written
//read
//that's it
uint8_t spi_go_rcv( uint8_t data){
  while ((*spi_status & 1));
  *spi_data = data;
  while (* spi_status & 1);
  while (!(* spi_rx_rdy & 1));
  return (uint8_t) *spi_data;
}

bool spi_available ( void){
  while (*spi_status & 1);
  *spi_data = 0x00;
  while (*spi_status& 1);
  while (!(*spi_rx_rdy & 1));
  return !((bool)( *spi_data & 1));
}
