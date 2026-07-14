
//this file is now the main for all tasksks
#include <stdint.h>
#include <rv32i.h>
//kernal
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

extern void freertos_risc_v_trap_handler(void);

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
void spi_to_stm( void *pvParameters){
  while(1) {
    as5600_begin();
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
   

  csr_write(MTVEC, (uint32_t) &freertos_risc_v_trap_handler);
  xTaskCreate(
    blink_1,
    "led_1_task",
    512,
    NULL,
    1,
    NULL

  );
  
  xTaskCreate(
    blink_2,
    "led_2_task",
    512,
    NULL,
    1,
    NULL
  );
  xTaskCreate(
    spi_to_stm,
    "name",
    400,
    NULL,
    1,
    NULL
  );

  vTaskStartScheduler();
for(;;);

  
}
