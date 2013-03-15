/**
*@file common.h
*@author Dirk Dubois, Alain Slak
*@date March 6th, 2013
*@brief A header file that contains the global variables being used
*/

#ifndef __COMMON_H
#define __COMMON_H

#include "cmsis_os.h"

/*Global Variables*/
extern float temperature; /**<The temperature variable*/
extern float accCorrectedValues[3]; /**<The corrected accelerometer values*/
extern float angles[2]; /**<A variable containing the pitch and roll */

extern osSemaphoreId tempId;
extern osSemaphoreId accId;
#endif
