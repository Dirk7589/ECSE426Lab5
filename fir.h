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
#define NUMBER_OF_SAMPLES 10 /**<The number of samples pass to the filter

/**@struct FIR_DATA
* A structure that holds all necessary values for the FIR filter
*/

typedef struct{
	int16_t coefficients[19];
	int16_t values[NUMBER_OF_SAMPLES];
	int16_t result;
}FIR_DATA

/**
*@brief A function that acts as a FIR filter
*
*@retval None
*/
void fir(void);

/**
*@brief A function that initializes the FIR filter
*
*@retval None
*/
void firInit(void);

#endif
