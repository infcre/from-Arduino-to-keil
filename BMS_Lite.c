__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_1,655);
HAL_GPIO_WritePin(GPIOC,GPIO_PIN_13,0);
for(int ci = 1;ci < 5;ci++) {
  HAL_GPIO_TogglePin(GPIOC,GPIO_PIN_13);
	HAL_Delay(200);
}
while (1) {
  HAL_GPIO_WritePin(GPIOC,GPIO_PIN_13,0);
	float adc_result = 3.3*get_adc_single()/4096;
	if(adc_result > 1.5) {
		__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_1,0);
		HAL_Delay(15000);
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_13,0);
	} else if (adc_result > 1.3) {
		__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_1,16384);
		HAL_Delay(5000);
	} else if(adc_result > 1.0) {
		__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_1,30000);
		HAL_Delay(5000);
	} else if(adc_result > 0.7){
	__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_1,10000);
		HAL_Delay(5000);
	} else {
	__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_1,655);
		HAL_Delay(1000);
	}
