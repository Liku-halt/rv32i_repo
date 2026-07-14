#include <stdint.h>
#include "rv32i.h"

//this script is a test for using uart through the basys3 fpga 
//it also include the use of inbuilt lib itoa 
//

//main fxn ......................................................//
 
void main (void){

 char buff[32];
 int count_down = 10;
  while(1){
   
   count_down = count_down<=0 ? 10 : count_down -1;
   
   snprintf_(buff,sizeof(buff), "count down: %d\r\n", count_down);//the last argument is the basis i think
   delay_ms(500);
   uart_print(buff);


  };

}
