/**
*@file moving_average.h
*@author Dirk Dubois, Alain Slak
*@date February 5th, 2013
*@brief A function that performs a moving average on a data point passed into the function. 
*@warning DEPTH define must be set before any function in movingAverage is called.
*@warning DEPTH must be set in both this header file and the lab1.s assembly file. They must have the same value
*
*/

#ifndef __MOVING_AVERAGE_H
#define __MOVING_AVERAGE_H

/*Includes*/
#include <stdint.h>

/**@defgroup MOVING_AVERAGE_DEFINES
* @{
*/


#define DEPTH 10 /**< DEPTH Defines the depth of the moving average filter */

#define DEPTH_INVERSE 1 / DEPTH /**< DEPTH_INVERSE Defines the depth inverse of the moving average filter */

#define DEPTH_BITS DEPTH*4 /**< DEPTH_BITS Defines the depth of the moving average filter in bits */
/**
*@}
*/

/*Global Variables*/

/*Structures*/

/**@struct AVERAGE_DATA_TYPEDEF
* A structure that holds all necessary values for the moving average filter, except the depth
*/
typedef struct{
	float previousSum; /**< The sum of the moving average before the division is performed*/
	float* lowestIndex; /**< A pointer to the current lowest index*/
	float previousValues[DEPTH]; /**< The previous values needed for an optomized movingAverage*/
}AVERAGE_DATA_TYPEDEF;
	
/**
  * @brief  A function that performs a moving average on a data point passed into the function,
	* based on previously stored values in the passed struct.
  * @param[in] value Where value is the new piece of data
	* @param[in,out] data Pointer to the struct that contains previous information for average
  * @retval Returns the moving average value for that data point
  */
float movingAverage(float value, AVERAGE_DATA_TYPEDEF* data);

/**
  * @brief  A function that performs the initialization for movingAverage
	* @param[in,out] data Pointer to the struct that contains previous information for average
  * @retval None
  */
void movingAverageInit(AVERAGE_DATA_TYPEDEF* data);

#endif
