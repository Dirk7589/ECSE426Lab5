/**
*@file temp.c
*@author Dirk Dubois, Alain Slak
*@date March 6th, 2013
*@brief 
*/

/*Includes*/
#include "temp.h"

/*Global Variables*/


/**
* @brief Conversion function for ADC value to degrees Celcius.
* The following function connverts an ADC value from the temperature
* sensor to a float value representing the temperature in degrees Celcius.
* The following formula is used: Temperature (in °C) = {(VSENSE – V25) / Avg_Slope} + 25
* @param[in] vSense the value returned by the ADC connected to the temperature sensor in bits
* @retval The temperature in degree C.
* @note The value for avg_slope is 2.5mV/C and v25 is 0.76V
* @warning vSense is internally converted to a value in miliVolts. Be sure to set SCALE to the appropriate value.
*/
float toDegreeC(uint16_t vSense)
{
	vSense = vSense * SCALE; //Convert from bits to miliVolts
	float temperature = (((vSense - V25) * AVG_SLOPE_INVERSE ) + 25 ); //Apply formula to convert to temperature
	return temperature;
}

/**
*@brief wrap around display for temperature readings. 
*2 degree increments starting at 30C
*@param[in] temperature The temperature to be displayed
*@retval None
*/
void displayTemperature(float temperature){
    if(temperature < 30){
        GPIOD->BSRRH = GREEN_LED | ORANGE_LED | RED_LED | BLUE_LED;
        GPIOD->BSRRL = BLUE_LED;
    }
    
    temperature = (int)fmodf(temperature, 8);
    
    if( temperature == 0 || temperature == 7){
        GPIOD->BSRRH = GREEN_LED | ORANGE_LED | RED_LED | BLUE_LED;
        GPIOD->BSRRL = BLUE_LED; //Max value of 32
    }
    
    if( temperature == 2 || temperature == 1 ){
        GPIOD->BSRRH = GREEN_LED | ORANGE_LED | RED_LED | BLUE_LED;
        GPIOD->BSRRL = BLUE_LED; //Max value of 34
        GPIOD->BSRRL = GREEN_LED;
    }
    
    if(temperature == 4 || temperature == 3 ){
        GPIOD->BSRRH = GREEN_LED | ORANGE_LED | RED_LED | BLUE_LED;
        GPIOD->BSRRL = BLUE_LED; //Max value of 36
        GPIOD->BSRRL = GREEN_LED;
        GPIOD->BSRRL = ORANGE_LED;
    }
    
    if(temperature == 6 || temperature == 5 ){
        GPIOD->BSRRH = GREEN_LED | ORANGE_LED | RED_LED | BLUE_LED;
        GPIOD->BSRRL = BLUE_LED; //Max value of 38
        GPIOD->BSRRL = GREEN_LED;
        GPIOD->BSRRL = ORANGE_LED;
        GPIOD->BSRRL = RED_LED;
    }
}

/**
*@brief A function that toggles the LEDs
*@param[in] LEDState current state of the LEDs
*@retval The current LEDState
*/
uint8_t LEDToggle(uint8_t LEDState){
    if(!LEDState){
        LEDState = 1; //update state
        //Turn on LEDs
        GPIOD->BSRRL = GREEN_LED;
        GPIOD->BSRRL = ORANGE_LED;
        GPIOD->BSRRL = RED_LED;
        GPIOD->BSRRL = BLUE_LED;
    }
    else{
        LEDState = 0; //update state
        GPIOD->BSRRH = GREEN_LED | ORANGE_LED | RED_LED | BLUE_LED; //Turn off leds
    }
		
		return LEDState;
}


