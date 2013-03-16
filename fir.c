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
*@param[in] data The data structure for the filter
*@retval The flag indicating a new output is ready, 1 indicates a sample is ready
*/
uint8_t fir(float value, FIR_DATA* data){
	value = value/FIR_SCALE;
	int16_t valueFixed;
	arm_float_to_q15(&value, &valueFixed, 1);
	int16_t mult[NUMBER_OF_SAMPLES];
	data->counter++;
	int16_t* lowestPtr = data->lowestIndex;
	
	
	if(lowestPtr == &data->values[NUMBER_OF_SAMPLES-1]){
		lowestPtr = &data->values[0];
	}
	
	*lowestPtr = valueFixed;
	lowestPtr++;
	
		arm_mult_q15(&data->coeffFixed[0], &data->values[0], &mult[0], NUMBER_OF_SAMPLES);
	//arm_mult_q15(&data->coeffFixed, &data->values, &mult, NUMBER_OF_SAMPLES);
	
	for(int i = 0; i < NUMBER_OF_SAMPLES; i++){
		data->result = data->result + mult[i];
	}
	
	data->lowestIndex = lowestPtr;
	
	if(data->counter == 10){
		data->counter = 0;
		//sample ready
		return 1;
	}
	else{
		return 0;
	}
}

/**
*@brief A function that initializes the FIR filter
*@param[in] data The data structure for the filter
*@retval None
*/
void firInit(FIR_DATA* data){
	arm_float_to_q15(&coeffFloat[0], &data->coeffFixed[0], 19);
	for(int i = 0; i < NUMBER_OF_SAMPLES; i++){
		data->values[i] = 0;
	}
	data->result = 0;
	data->lowestIndex = &data->values[0];
	data->counter = 0;
}

