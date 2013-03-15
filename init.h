/**
*@file init.h
*@author Dirk Dubois, Alain Slak
*@date February 6th, 2013
*@brief A set of functions that intialize the user button LEDs and temperature sensor.
*
*/

#ifndef __INIT_H
#define __INIT_H
/*Includes*/
#include <stdint.h>


/*Defines*/
/**@defgroup TIM3_SCALERS
*@{
*/
#define PRESCALER 41999 /**<A prescaler value for TIM3*/
#define PERIOD 19 /**<A period value for TIM3*/
#define BUTTON_PRIORITY 2 /**<Subpriority for NVIC*/
/**
*@}
*/

/**
* @brief An intialization funtion for the IO Ports.
* This function sets pins 12-15 as outputs on GPIOD to drive STM32FDiscovery boards LEDS.
* It also sets pin 0 as input on GPIOA, with weak pull down, for active high user button
* on STM32FDiscovery board.
* @note In order for GPIO banks A & D to function correctly, their peripheral clocks are enabled.
* @retval None
*/
void initIO(void);

/**
* @brief A initialzation function for the ADC on channel 16 for built in temp sensor.
* The following function initializes the ADC, without interupts, DMA, or continous scanning,
* for use with the built in temperature sensor on ADC channel 16. The data is right aligned, and
* the resolution is set to 12 bits. 
*	@note The software convertion is started in this function, activating sampling right after this function returns.
* @note In order for the ADC to function correctly, its peripheral clock is enabled.
* @retval None 
*/
void initTempADC(void);

/**
*@brief A function that intilizes Timer3 for use with the acclerometer.
*The timer will trip an interupt every 10 ms for a rate of 100Hz. This function
*enables the NVIC and sets TIM3 to a subpriority of 0 with a preemption priority of 0.
*@retval None
*/
void initTim3(void);

/**
*@brief A function that intializes DMA for use with the accelerometer.
*@retval None
*/
void initDMAACC(void);

/**
*@brief A function that enables EXTI for the button
*@retval None
*/
void initEXTIButton(void);
#endif
