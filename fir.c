 /**
*@file fir.c
*@author Dirk Dubois, Alain Slak
*@date February 21th, 2013
*@brief 
*
*/

/*Includes*/
#include <stdint.h>
#include <arm_math.h>
#include "fir.h"

float32_t coeffFloat[] = {
0.07416114002096 , 0.03270293265285 , 0.03863816947328 ,
0.04444276039114 , 0.0497985768307 , 0.05449897183339 ,
0.05841301164775 , 0.06134989872289 , 0.06313835392955 ,
0.06373119762273 , 0.06313835392955 , 0.06134989872289 ,
0.05841301164775 , 0.05449897183339 , 0.0497985768307 ,
0.04444276039114 , 0.03863816947328 , 0.03270293265285 ,
0.07416114002096
};

/**
*@brief A function that acts as a FIR filter
*@param[in] value The newest value to the filter
*@param[inout] data The data structure for the filter
*@retval The flag indicating a new output is ready, 1 indicates a sample is ready
*/
uint8_t fir(float value, FIR_DATA* data){
	
	int16_t valueFixed; //result in q15_t var
	int16_t mult[NUMBER_OF_SAMPLES]; //result of multiplication
	int16_t* lowestPtr = data->lowestIndex; //pointer to the lowest index pointer of the struct
	
	value = value/FIR_SCALE; //Scale incoming float value
	
	arm_float_to_q15(&value, &valueFixed, 1); //Convert float to q15
	
	data->counter++; //Increase the data counter
	
	//Check if pointer has wrapped around
	if(lowestPtr == &data->values[NUMBER_OF_SAMPLES-1]){
		lowestPtr = &data->values[0];
	}
	
	*lowestPtr = valueFixed; //Update lowest index value
	lowestPtr++; //Increment pointer
	
	arm_mult_q15(&data->coeffFixed[0], &data->values[0], &mult[0], NUMBER_OF_SAMPLES); //Multiply coefficients
	
	for(int i = 0; i < NUMBER_OF_SAMPLES; i++){
		data->result = data->result + mult[i]; //Sum results
	}
	
	data->lowestIndex = lowestPtr; //Update lowest index ptr
	
	//Check if we have latched in 10 samples
	if(data->counter == 10){
		data->counter = 0; //reset counter
		
		return 1; //Signal that result is ready
	}
	else{
		return 0; //Signal that result is not! ready
	}
}

/**
*@brief A function that initializes the FIR filter
*@param[in] data The data structure for the filter
*@retval None
*/
void firInit(FIR_DATA* data){
	
	arm_float_to_q15(&coeffFloat[0], &data->coeffFixed[0], 19); //Convert coefficients
	for(int i = 0; i < NUMBER_OF_SAMPLES; i++){
		data->values[i] = 0; //Zero structures
	}
	data->result = 0;
	data->lowestIndex = &data->values[0];
	data->counter = 0;
}

