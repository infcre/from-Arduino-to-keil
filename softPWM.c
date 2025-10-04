while (1) {
  __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_3,30000);
		
  for(int i = 1;i  < 11;i++) {
    for(int iin = 1;iin < 11;iin++) {	
      HAL_GPIO_WritePin(GPIOC,GPIO_PIN_13,GPIO_PIN_SET);
      HAL_Delay(10 - i);
      HAL_GPIO_WritePin(GPIOC,GPIO_PIN_13,GPIO_PIN_RESET);
      HAL_Delay(i);
    }
  }
HAL_UART_Transmit(&huart1, (uint8_t*)"Hello UART\r\n", 13, 100);
}
