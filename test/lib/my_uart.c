#include <stdint.h> 
#include <rv32i.h>
#include "stdbool.h"

////////////////
///
/*
volatile uint32_t *uart_tx_data = (volatile uint32_t *) UART_TX_DATA;
volatile uint32_t *uart_tx_busy = (volatile uint32_t *) UART_TX_BUSY;
volatile uint32_t *uart_rx_full = (volatile uint32_t *) UART_RX_BUFFER_FULL;
volatile uint32_t *uart_rx_data = (volatile uint32_t *) UART_RX_DATA;

volatile uint32_t *uart2_tx_data = (volatile uint32_t *) UART2_TX_DATA;
volatile uint32_t *uart2_tx_busy = (volatile uint32_t *) UART2_TX_BUSY;
volatile uint32_t *uart2_rx_full = (volatile uint32_t *) UART2_RX_BUFFER_FULL;
volatile uint32_t *uart2_rx_data = (volatile uint32_t *) UART2_RX_DATA;
*/



// print characters serially via UART
void my_uart_print(uart *uart_struct_pointer ,char *message) {
    int i = 0;
    while (message[i] != '\0') {
        while (*uart_struct_pointer->tx_busy);  // wait for UART to be ready
        *uart_struct_pointer->tx_data = message[i];
        i++;
    }
}

//check if read buffer is full and data can be read
bool my_uart_rx_buffer_full(uart *uart_struct_pointer ){
    bool ready = *uart_struct_pointer->rx_full;
    return ready;
}

//read data from buffer (make sure to check first if rx buffer is full)
char my_uart_read(uart *uart_struct_pointer ){
    char read_data;
    read_data = *uart_struct_pointer->rx_data; 
    return read_data;
}

////////////////////////////////
///this fucntion is only for sending uint8_t bytes ok 
void my_uart_send_byte( uart *uart_struct_pointer, const uint8_t *data, int len ){
  int i = 0 ;
  while (i < len){
    while (*uart_struct_pointer->tx_busy);
    *uart_struct_pointer->tx_data = data[i];
    i++;
  }
}
uint8_t my_uart_read_byte(uart *uart_struct_pointer ){
  uint8_t read_data;
  while (!(my_uart_rx_buffer_full(uart_struct_pointer)));
  read_data = *uart_struct_pointer->rx_data;
  return read_data;
}

