/**
*@file init.c
*@author Dirk Dubois, Alain Slak
*@date February 6th, 2013
*@brief A set of functions that intialize the user button LEDs and temperature sensor.
*
*/

/*Includes*/
#include "init.h"
#include "stm32f4xx.h"
#include "stm32f4xx_conf.h"

/**
* @brief An intialization funtion for the IO Ports.
* This function sets pins 12-15 as outputs on GPIOD to drive STM32FDiscovery boards LEDS.
* It also sets pin 0 as input on GPIOA, with weak pull down, for active high user button
* on STM32FDiscovery board.
* @note In order for GPIO banks A & D to function correctly, their peripheral clocks are enabled.
* @retval None
*/
void initIO(void)
{
	//RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); //Enable peripheral clocks
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	
	GPIO_InitTypeDef gpio_init_leds;			//Create the intialization struct
	GPIO_StructInit(&gpio_init_leds);			//Intialize the struct
	
	gpio_init_leds.GPIO_Pin	= GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15; //Setting all LEDs
	gpio_init_leds.GPIO_Mode = GPIO_Mode_OUT; //Set as output
	gpio_init_leds.GPIO_Speed = GPIO_Speed_100MHz; //Set at max slew rate
	gpio_init_leds.GPIO_OType = GPIO_OType_PP; //Push Pull config
	gpio_init_leds.GPIO_PuPd = GPIO_PuPd_NOPULL; //Turn off pull ups 
	
	GPIO_Init(GPIOD, &gpio_init_leds); //Intialize port
	
	GPIO_InitTypeDef gpio_init_button;			//Create the intialization struct
	GPIO_StructInit(&gpio_init_button);			//Intialize the struct
	
	gpio_init_button.GPIO_Pin	= GPIO_Pin_0; //Set button input
	gpio_init_button.GPIO_Mode = GPIO_Mode_IN; //Set as input
	gpio_init_button.GPIO_Speed = GPIO_Speed_50MHz; //Set at max slew rate
	gpio_init_button.GPIO_OType = GPIO_OType_PP; //Push Pull config
	gpio_init_button.GPIO_PuPd = GPIO_PuPd_DOWN; //Turn pull up on low
	
	GPIO_Init(GPIOA, &gpio_init_button); //Intialize port
	
}

/**
* @brief An initialzation function for the ADC on channel 16 for built in temp sensor.
* The following function initializes the ADC, without interupts, DMA, or continous scanning,
* for use with the built in temperature sensor on ADC channel 16. The data is right aligned, and
* the resolution is set to 12 bits. 
* @note In order for the ADC to function correctly, its peripheral clock is enabled.
* @note To use the built in temperature sensor ADC_TempSensorVrefintCmd must be set to ENABLE.
* @retval None 
*/
void initTempADC(void)
{
	//Create ADC structs
	ADC_InitTypeDef	adc_init_s;
	ADC_CommonInitTypeDef adc_common_init_s;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE); //Turn on peripheral clock
	
	//Set common ADC behavior
	adc_common_init_s.ADC_Mode = ADC_Mode_Independent;
	adc_common_init_s.ADC_Prescaler = ADC_Prescaler_Div2;
	adc_common_init_s.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
	adc_common_init_s.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;
	
	ADC_CommonInit(&adc_common_init_s); //Initialize common settings
	
	//Specify ADC1 behaviour
	adc_init_s.ADC_Resolution = ADC_Resolution_12b;
	adc_init_s.ADC_ScanConvMode = DISABLE;
	adc_init_s.ADC_ContinuousConvMode = DISABLE;
	adc_init_s.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
	adc_init_s.ADC_DataAlign = ADC_DataAlign_Right;
	adc_init_s.ADC_NbrOfConversion = 1;
	ADC_Init(ADC1, &adc_init_s);
	
	ADC_Cmd(ADC1, ENABLE); //Enable ADC1
	
	ADC_TempSensorVrefintCmd(ENABLE); //Enable the connections to the temp sensor
	
	ADC_RegularChannelConfig(ADC1, ADC_Channel_TempSensor, 1, ADC_SampleTime_480Cycles); //Cofigure the ADC Channel
	
}

/**
*@brief A function that intilizes Timer3 for use with the acclerometer
*The timer will trip an interupt every 10 ms for a rate of 100Hz. This function
*enables the NVIC and sets TIM3 to a subpriority of 0 with a preemption priority of 0.
*@retval None
*/
void initTim3(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);//Enable APB1 peripheral clock for TIM3
	
	NVIC_InitTypeDef NVIC_Struct; //Create intialization struct for NVIC
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct; //Create TIM base struct
	
	TIM_TimeBaseStructInit(&TIM_TimeBaseInitStruct); //Intialize the struct
	
	//Configure Timer for 100Hz
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Down; 	//Setup countdown mode
	TIM_TimeBaseInitStruct.TIM_Period = (uint16_t)PERIOD;						//Set Period given in init.h
	TIM_TimeBaseInitStruct.TIM_Prescaler = (uint16_t)PRESCALER;			//Set Prescaler given in init.h
	TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 0;

	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStruct); //Initalize the timer
	
	//Enable the NVIC if needed
	NVIC_Struct.NVIC_IRQChannel = TIM3_IRQn; //Select timer 3 interupt
	NVIC_Struct.NVIC_IRQChannelPreemptionPriority = 0; //Set preemption priority
	NVIC_Struct.NVIC_IRQChannelSubPriority =0; //Set sub prioirity
	NVIC_Struct.NVIC_IRQChannelCmd = ENABLE; //Enable NIVC
	
	NVIC_Init(&NVIC_Struct); //Setup NVIC with struct
	
	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE); //Enable the corresponding NVIC interupt
	
	TIM_Cmd(TIM3, ENABLE); //Enable the timer
}

/**
*@brief A function that intializes DMA for use with the accelerometer.
*@retval None
*@Warning initACC must be called before hand in order to configure SPI correctly
*/
void initDMAACC(void)
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE); //Enable peripheral clock for DMA2
	
	DMA_InitTypeDef DMA_InitStruct; //Create DMA init struct
	NVIC_InitTypeDef NVIC_Struct; //Create intialization struct for NVIC
	
	DMA_DeInit(DMA2_Stream0);
	DMA_StructInit(&DMA_InitStruct); //Prepare the structure
	
	DMA_InitStruct.DMA_Channel = DMA_Channel_3; //Select the Channel connected to SPI1 RX, pg 168 of FRM
	DMA_InitStruct.DMA_PeripheralBaseAddr = (uint32_t) &SPI1->DR; //Must be a uint32
	//DMA_InitStruct.DMA_Memory0BaseAddr = (uint32_t) &accValuesDestination; //Variable where the data will be stored
	DMA_InitStruct.DMA_DIR = DMA_DIR_PeripheralToMemory;
	DMA_InitStruct.DMA_BufferSize = 7; //Size of buffer
	DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	DMA_InitStruct.DMA_Mode = DMA_Mode_Normal;
	DMA_InitStruct.DMA_Priority = DMA_Priority_Medium;
	DMA_InitStruct.DMA_FIFOMode = DMA_FIFOMode_Disable;
	DMA_InitStruct.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
	DMA_InitStruct.DMA_MemoryBurst = DMA_MemoryBurst_Single;
	DMA_InitStruct.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
	
	DMA_Init(DMA2_Stream0, &DMA_InitStruct); //Intialize the structure
	
	DMA_InitStruct.DMA_DIR = DMA_DIR_MemoryToPeripheral; //
	DMA_Init(DMA2_Stream3, &DMA_InitStruct); //Setup Tx stream

	NVIC_Struct.NVIC_IRQChannel = DMA2_Stream0_IRQn; //Select timer 3 interupt
	NVIC_Struct.NVIC_IRQChannelPreemptionPriority = 0; //Set preemption priority
	NVIC_Struct.NVIC_IRQChannelSubPriority =0; //Set sub prioirity
	NVIC_Struct.NVIC_IRQChannelCmd = ENABLE; //Enable NIVC
	
	
	
	DMA_ITConfig(DMA2_Stream0, DMA_IT_TC, ENABLE); //Enable the corresponding NVIC interupt
	
    NVIC_Init(&NVIC_Struct); //Setup NVIC with struct
    
	DMA_Cmd(DMA2_Stream0, ENABLE); //Enable DMA for RX
	DMA_Cmd(DMA2_Stream3, ENABLE); //Enable DMA for TX
	
	SPI_DMACmd(SPI1, SPI_DMAReq_Rx | SPI_DMAReq_Tx, ENABLE); //Start Rx dma on SPI1
	//SPI_DMACmd(SPI1, SPI_DMAReq_Tx, ENABLE); //Start Tx dma on SPI1
	
}

/**
*@brief A function that enables EXTI for the button
*@retval None
*/
void initEXTIButton(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE); //Enable peripheral clock for EXTI
	
	//Setup interupts
	EXTI_InitTypeDef exti_init;

	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource0);	//Select pin to interupt from
	
	exti_init.EXTI_Line = EXTI_Line0; //Select line 1
	exti_init.EXTI_LineCmd = ENABLE; //Enable
	exti_init.EXTI_Mode = EXTI_Mode_Interrupt; //Interupt mode
	exti_init.EXTI_Trigger = EXTI_Trigger_Rising; //Rising edge trigger
	
	EXTI_Init(&exti_init);	//Configure the interupt mode

	//Enable the NVIC if needed
	NVIC_InitTypeDef NVIC_Struct; //Create intialization struct for NVIC
	
	NVIC_Struct.NVIC_IRQChannel = EXTI0_IRQn; //Select EXTI0
	NVIC_Struct.NVIC_IRQChannelPreemptionPriority = 0; //Set preemption priority
	NVIC_Struct.NVIC_IRQChannelSubPriority = 0; //Set sub prioirity
	NVIC_Struct.NVIC_IRQChannelCmd = ENABLE; //Enable NIVC
	
	NVIC_Init(&NVIC_Struct); //Setup NVIC with struct//Configure the NVIC for use with EXTI
}

