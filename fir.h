 /**
*@file fir.h
*@author Dirk Dubois, Alain Slak
*@date February 21th, 2013
*@brief 
*
*/

#ifndef __FIR_H
#define __FIR_H

/*Defines*/
#define NUMBER_OF_SAMPLES 19 /**<The number of samples pass to the filter*/
#define SCALE 10000

/**@struct FIR_DATA
* A structure that holds all necessary values for the FIR filter
*/

typedef struct{
	int16_t coeffFixed[19];
	int16_t values[NUMBER_OF_SAMPLES];
	int16_t* lowestIndex;
	int16_t result;
	uint8_t count;
}FIR_DATA;

/**
*@brief A function that acts as a FIR filter
*@param[in] value The newest value to the filter
*@param[in] data The data structure for the filter
*@retval The flag indicating a new output is ready, 1 indicates a sample is ready
*/
uint8_t fir(float32_t value, FIR_DATA* data);

/**
*@brief A function that initializes the FIR filter
*@param[in] data The data structure for the filter
*@retval None
*/
void firInit(FIR_DATA* data);

#endif
