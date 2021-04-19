#define ADC_A       GPIOA
#define ADC_B     GPIOB
#define ADC_C     GPIOC
#define Ray				GPIOF
#define RCC_ADC_A_ON  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE)
#define RCC_ADC_B_ON  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE)
#define RCC_ADC_C_ON  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE)
#define RCC_Ray_ON  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF, ENABLE)
#define RCC_ADC1_ON   RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE)
#define RCC_AFIO_ON   RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE)
#define RCC_ADC_TIM_ON  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE)
#define GPIO_ADC_A    GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7
#define GPIO_ADC_B    GPIO_Pin_0 | GPIO_Pin_1
#define GPIO_ADC_C    GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5
#define GPIO_Ray			GPIO_Pin_0 |GPIO_Pin_1 |GPIO_Pin_2 |GPIO_Pin_3 |GPIO_Pin_4 |GPIO_Pin_5 |GPIO_Pin_6 |GPIO_Pin_7 |GPIO_Pin_8 |GPIO_Pin_9 |GPIO_Pin_10 |GPIO_Pin_11 |GPIO_Pin_12 |GPIO_Pin_13 |GPIO_Pin_14 |GPIO_Pin_15


void RCC_Configuration_ADC(void);
void GPIO_Configuration_ADC(void);
void ADC1_Configuration(void);
void ADC_Channel_Converter(void);
void delay_cnt_LCD(uint32_t delay);
void TIM_Configuration_ADC(void) ;
void NVIC_Configuration_ADC(void);

int adc_num;
uint16_t ADC[16];


int ms_counter;

int main(void)
{
	RCC_Configuration_ADC();
	GPIO_Configuration_ADC();
	ADC1_Configuration();
	TIM_Configuration_ADC();
	NVIC_Configuration_ADC();
	RCC_Configuration_LCD();
	GPIO_Configuration_LCD();
	LCD_init();
	LCD_print();
	while(1)
	{
		
	}
}



void RCC_Configuration_ADC(void)
{
    RCC_ADC_A_ON;
    RCC_ADC_B_ON;
    RCC_ADC_C_ON;
		RCC_Ray_ON;
    RCC_ADC1_ON;
    RCC_ADC_TIM_ON;
}



void GPIO_Configuration_ADC(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  
  /* ADC Pin Set */
  GPIO_InitStructure.GPIO_Pin = GPIO_ADC_A;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
  GPIO_Init(ADC_A, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = GPIO_ADC_B;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
  GPIO_Init(ADC_B, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = GPIO_ADC_C;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
  GPIO_Init(ADC_C, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Ray;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(Ray, &GPIO_InitStructure);
}

void ADC1_Configuration(void)
{
  ADC_InitTypeDef ADC_InitStructure;
 
  ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;  //독립적으로 수행
  ADC_InitStructure.ADC_ScanConvMode = DISABLE;  //ADC control register1 의 scan 비트부분 설정
  ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;  //ADC control register2 의 cont 비트부분 설정(0이면 한번,1이면 여러번수행)
  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None; //ADC control register2 의 Extsel 비트부분 설정
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right; //ADC control register2 의 align 비트부분 설정(0이면 right,1이면 left)
  ADC_InitStructure.ADC_NbrOfChannel = 1; //몇개의 신호변환인지
  ADC_Init(ADC1, &ADC_InitStructure);
   
  ADC_RegularChannelConfig(ADC1, ADC_Channel_0,  1, ADC_SampleTime_55Cycles5); //ADC채널및 sampling time 설정
  
  ADC_Cmd(ADC1, ENABLE);
   
  ADC_ResetCalibration(ADC1);
  while(ADC_GetResetCalibrationStatus(ADC1));

 
  ADC_StartCalibration(ADC1);
  while(ADC_GetCalibrationStatus(ADC1));
}

void NVIC_Configuration_ADC(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;

  NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
  NVIC_Init(&NVIC_InitStructure);
}

void TIM_Configuration_ADC(void) 
{ 
  TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;

  TIM_TimeBaseStructure.TIM_Period = 39; //100us 주기 계산해서 맞춰놈 원래 99임 
  TIM_TimeBaseStructure.TIM_Prescaler = 719;
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;  // 0x0
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  // 0x0
   
  TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
   
  TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);

  TIM_Cmd(TIM4, ENABLE);
}

void ADC_Channel_Converter(void)
{
 
  ADC_RegularChannelConfig(ADC1, ADC_Channel_0 + adc_num,  1, ADC_SampleTime_55Cycles5); //ADC채널및 sampling time 설정
  GPIO_Write(Ray, 0x0001 << adc_num);

  ADC_Cmd(ADC1, ENABLE);
   
  ADC_ResetCalibration(ADC1); // Calibration 안하면 쓰레기값
  while(ADC_GetResetCalibrationStatus(ADC1));
 
  ADC_StartCalibration(ADC1);
  while(ADC_GetCalibrationStatus(ADC1));    
}

void TIM4_IRQHandler(void)
{
  if(TIM_GetITStatus(TIM4,TIM_IT_Update) != RESET)
  {
		
    TIM_ClearITPendingBit(TIM4, TIM_IT_Update); // Clear the interrupt flag
    ADC_Channel_Converter(); // 발광 키기 && ADC셋팅
 //   delay_us(10);
//     tmp_Sum = 0;
//     tmp_value = 0;

    ADC_SoftwareStartConvCmd(ADC1, ENABLE); //adc 변환 시작
    delay_cnt_LCD(120);
    if(ADC_GetFlagStatus(ADC1,0x2)== 1) //변환이 끝나고 End of Conversion Stais가 활성화 되면 데이터를 가지고옴
    ADC[adc_num]= ADC_GetConversionValue(ADC1); // ADC_GetFlagStatus(ADC1,0x2)==RESET 이 레지스터 값이 0이면 데이터받는중 1이면 데이터받기끝
    Ray->ODR = 0x0000;
//     
//     if(menu == 1 || menu == 2)
//     {
//       if(menu == 1)
//       {
//         if(ADC[adc_num] > ADC_Max[adc_num]) ADC_Max[adc_num] = ADC[adc_num];
//       }
//       else
//       { 
//         if(ADC[adc_num] > ADC_Min[adc_num]) ADC_Min[adc_num] = ADC[adc_num];
//       }
//     }
//     
//     ADC_Normal[adc_num] = (ADC[adc_num] - ADC_Min[adc_num]) * 100 / (ADC_Max[adc_num] - ADC_Min[adc_num]);
//     if(ADC_Normal[adc_num] > 90) ADC_Normal[adc_num] = 90;
//     else if(ADC_Normal[adc_num] < 10) ADC_Normal[adc_num] = 0;

//     for(i = 0; i < 16; i++)
//     {
//       tmp_Sum += ADC_Normal[i];
//       tmp_value += ADC_Normal[i] * weight[i];
//     }
//     ADC_Sum = tmp_Sum;
//     mis_value = tmp_value / ADC_Sum;
//  //   ADC_SUM = (ADC_Normal[0] + ADC_NOMAL[1] + ADC_Normal[2] + ADC_Normal[3] + ADC_Normal[4] + ADC_Normal[5] + ADC_Normal[6] + ADC_Normal[7] + ADC_Normal[8] + ADC_Normal[9] + ADC_Normal[10] + ADC_Normal[11] + ADC_Normal[12] + ADC_Normal[13] + ADC_Normal[14] + ADC_Normal[15]);
//  //   mis_value=((ADC_Normal[0] * weight[0]) + (ADC_Normal[1] * weight[1]) + (ADC_Normal[2] * weight[2]) + (ADC_Normal[3] * weight[3]) + (ADC_Normal[4] * weight[4]) + (ADC_Normal[5] * weight[5]) + (ADC_Normal[6] * weight[6]) + (ADC_Normal[7] * weight[7]) + (ADC_Normal[8] * weight[8]) + (ADC_Normal[9] * weight[9]) + (ADC_Normal[10] * weight[10]) + (ADC_Normal[12] * weight[0]) + (ADC_Normal[0] * weight[0]) + (ADC_Normal[0] * weight[0]) + (ADC_Normal[0] * weight[0]) + (ADC_Normal[0] * weight[0]) )/ADC_SUM;  

    adc_num++;
    if(adc_num > 15) adc_num = 0;
   }
}
