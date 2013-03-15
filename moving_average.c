/**
*@file moving_average.c
*@author Dirk Dubois, Alain Slak
*@date February 5th, 2013
*@brief A function that performs a moving average on a data point passed into the function. 
*@warning depth variable must be set before any function in moving_average is called.
*
*/

/*Includes*/
#include "moving_average.h"

/**
  * @brief  A function that performs a moving average on a data point passed into the function,
	* based on previously stored values in the passed struct.
  * @param[in] value Where value is the new piece of data
	* @param[in,out] data Pointer to the struct that contains previous information for average
  * @retval Returns the moving average value for that data point
  */
float movingAverage(float value, AVERAGE_DATA_TYPEDEF* data)
{
	float* lowestPtr = data->lowestIndex;
	float previousValue = *lowestPtr;
	
	float intermediateSum = data->previousSum;
	intermediateSum = intermediateSum + value;
	intermediateSum = intermediateSum - previousValue;

	data->previousSum = intermediateSum;	//Update previous sum with the new sum	
	
	//Update new lowest index
	*lowestPtr = value;
    
	//Update newest lowest index ptr
	lowestPtr++;
    

	
	//load the pointer to the bottom location in the array
	float* bottomPtr = &data->previousValues[DEPTH-1];
	
	//check if the pointer has passed the bottom of the array
	if (lowestPtr > bottomPtr){
	
        float* topPtr = bottomPtr - DEPTH + 1;
		//loop to the top of the array
        lowestPtr = topPtr;
	
	}
    
    data->lowestIndex = lowestPtr;
	
	float average = intermediateSum*DEPTH_INVERSE;
	
	return (float)average;
	//return intermediateSum*depthInverse; //Return computed average
}

/**
  * @brief  A function that performs the initialization for movingAverage
	* @param[in,out] data Pointer to the struct that contains previous information for average
  * @retval None
  */
void movingAverageInit(AVERAGE_DATA_TYPEDEF* data)
{
	uint32_t i; //Counting variable
	data->lowestIndex = &data->previousValues[0];
	
	for(i=0; i < DEPTH; i++){
		data->previousValues[i] = 0;
	}
}

