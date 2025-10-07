uint32_t get_adc_single(void) {
   HAL_ADC_Start(&hadc1);
   HAL_ADC_PollForConversion(&hadc1, 10);
   uint32_t adc = HAL_ADC_GetValue(&hadc1);
   HAL_ADC_Stop(&hadc1);
   return adc; 
//continuous conversion mode关闭了（这个东西在cubeMX里面可以打开，但是默认关闭）
//adc只能一次读一次，读完就关                 P.S.  AI认为HAL_ADC_Stop(&hadc1);是多余的，因为它读完之后会自己关
//这一部分需要粘贴在专门用来定义函数的区域

//下一部分在while(1)里面
float adc_result = 3300 * get_adc_single() / 4096;  //这里输出的是电压，满电压3.3v，分辨率4096（AI认为应该除以4095）
printf("voltage = %f mV",adc_result);

/*
为什么输出不能以V为单位
因为那样会出现小数
microLIB里面的printf默认是不支持小数的

有没有办法输出小数
用printf("voltage = %lu.%03lu V\r\n", voltage_mv / 1000, voltage_mv % 1000);
%lu.%03lu   第一个长无符号整型.第二个长无符号整型（强行凑三位，不够的再左侧补0）
voltage_mv / 1000  都是整型，除法会抹掉小数
voltage_mv % 1000  这种运算是做除法，而且只输出余数，假如是3000，就没有余数，假如是1234，就输出234
那么假如是1024，就输出24，再加上前面那个强行凑三位的，最终就会变成024
*/
