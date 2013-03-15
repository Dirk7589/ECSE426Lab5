/**
*@file access.c
*@author Dirk Dubois, Alain Slak
*@date March 6th, 2013
*@brief A set of functions that provide access to global variables shared accross multiple threads
*
*/

/*Includes*/
#include "access.h"
#include "common.h"
#include "cmsis_os.h"
/**
*@brief A function that safely access's the corrected values of the accelerometer
*@param[inout] accValues A pointer to the new location in memory that data is copied to
*@retval None
*/
void getACCValues(float* accValues)
{
	
	int i = 0;
	for(i = 0; i < 3; i++){
		osSemaphoreWait(accId, osWaitForever);
		accValues[i] = accCorrectedValues[i]; //Critical access portion
		osSemaphoreRelease(accId);
	}
}

/**
*@brief A function that safely access's the temperature value of the temperature sensor
*@retval temperature
*/
float getTemperature(void)
{
	float temp;
	osSemaphoreWait(tempId, osWaitForever);
	temp = temperature; //Critical access portion
	osSemaphoreRelease(tempId);
	return temp;
}
