/*首先要启动microLIB
然后在usart.c头文件定义段#include <stdio.h>
*/

/* USER CODE BEGIN 0 */
struct FILE { 
	int handle; 
}; 

int fputc(int ch, FILE * f){
	HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, 0xffff);
  	while(__HAL_UART_GET_FLAG(&huart1, UART_FLAG_TC) == RESET){}
  	return ch;
}


int fgetc(FILE * F) {
	uint8_t ch = 0;
	HAL_UART_Receive(&huart1,&ch, 1, 0xffff);
    while(__HAL_UART_GET_FLAG(&huart1, UART_FLAG_TC) == RESET){}
	return ch; 
}

/* USER CODE END 0 */

//注意代码的粘贴位置

/*
microLIB是 ARM 为嵌入式系统提供的精简版 C 标准库
printf和scanf在microLIB里面
printf和scanf分别用了fputc和fgetc
只要改掉了fputc和fgetc，printf和scanf就可以收发串口信息

FILE是一个空的结构体
但是没有它，fputc和fgetc里面的的FILE就会“找不到”，虽然它实际上没有用
*/
