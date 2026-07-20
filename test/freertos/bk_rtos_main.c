
//this file is now the main for all tasksks
#include <stdint.h>
#include <rv32i.h>
//kernal
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

extern void freertos_risc_v_trap_handler(void);

//my uart init pvrt
//
uart uart_2={
  .tx_data = vol_uint UART2_TX_DATA,
  .tx_busy = vol_uint UART2_TX_BUSY,
  .rx_full = vol_uint UART2_RX_BUFFER_FULL,
  .rx_data = vol_uint UART2_RX_DATA
};

uart uart_1={
  .tx_data = vol_uint UART_TX_DATA,
  .tx_busy = vol_uint UART_TX_BUSY,
  .rx_full = vol_uint UART_RX_BUFFER_FULL,
  .rx_data = vol_uint UART_RX_DATA
};






void vApplicationTickHook(void) {
    // empty — blinking is handled by tasks
}

void vApplicationMallocFailedHook(void) {
    taskDISABLE_INTERRUPTS();
    uart_print("FAULT: malloc failed — increase configTOTAL_HEAP_SIZE\n");
    __asm volatile("ebreak");
    for(;;);
}

void vApplicationIdleHook(void) {
    // empty
}

void vApplicationStackOverflowHook(TaskHandle_t pxTask, char *pcTaskName) {
    (void) pcTaskName;
    (void) pxTask;
    taskDISABLE_INTERRUPTS();
    uart_print("FAULT: stack overflow\n");
    __asm volatile("ebreak");
    for(;;);
  

}

void SystemIrqHandler(uint32_t mcause) {
    uart_print("Unknown interrupt\n");
     
}


//tsk fxn 1
void blink_1(void *pvParameters){
  while(1) {
    toggle_gpio(0);
    vTaskDelay(pdMS_TO_TICKS(980));

  }
}
void reading_the_sen( void *pvParameters){
  while(1) {
      
   float result = read_tvoc(&uart_2);
   printf_("CO: %.1f ppm\r\n", result);
    
    
    vTaskDelay(pdMS_TO_TICKS(500));
  }
}


//tsk fxn 2
void blink_2(void *pvParameters){

  while(1){
    
    toggle_gpio(1);
    vTaskDelay(pdMS_TO_TICKS(770));
  }
}


void main(void){
  //sensor init step 
  set_mode_tvoc(&uart_2);
   

  csr_write(MTVEC, (uint32_t) &freertos_risc_v_trap_handler);
  xTaskCreate(
    blink_1,
    "led_1_task",
    200,
    NULL,
    1,
    NULL

  );
  
  xTaskCreate(
    blink_2,
    "led_2_task",
    200,
    NULL,
    1,
    NULL
  );
  xTaskCreate(
    reading_the_sen,
    "name",
    512,
    NULL,
    1,
    NULL
  );

  vTaskStartScheduler();
for(;;);

  
}
