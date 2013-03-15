/**@file initACC.h
*@Author Dirk Dubois, Alain Slak
*@Date February 13, 2013
*/

#ifndef __INIT_ACC_H
#define __INIT_ACC_H

/*Includes*/
#include <stdint.h>

/*Defines*/
#define CONVERSION_TO_DEG 57.29577951308233 /**<The scalling factor from radians to degrees*/
#define ANGLE_THRESHOLD 20 /**<Threshold angle for displayDominant angle*/
#define MOVEMENT_LED_THRESHOLD 150 /**<Threshold acceleration for displayBoardMovement*/
#define MOVEMENT_THRESHOLD 100 /**<Threshold acceleration for displayBoardMovement*/
/*Structs*/



/**
*@brief A function that initalizes the ACC over SPI
*The function enables the LIS302DL accelerometer for all three axes, sets the data rate to 100Hz for SPI1,
*and enables low power mode. It also enables interupts for the single click tap mode.
*@retval None
*@note This function automatically enables and configures SPI1 for use for the accelerometer.
*@note LIS302DL_Init correctly configures the corresponding GPIO pin for its interupts.
*/
void initACC(void);

/**
*@brief A function that converts the corrected and filtered acceleration values to an angle in degrees.
*@param[in] accValues the passed in accelerations
*@param[out] angles the resulting roll and pitch angles in degress
*@retval None
*@warning CONVERSION_TO_DEG must be set in order for the function to return the value in degrees,
*if the user wants the value in radians set CONVERSION_TO_DEG to 1.
*/
void toAngle(float *accValues, float *angles);

/**
*@brief A function that calibrates the accelerometer.
*@param[in] accValues The uncorrected inputed accelerometer values
*@param[out] accCorrectedValues The resulting corrected accelerometer values
*@warning This function can only be called once initACC has been called
*/
void calibrateACC(int32_t* accValues, float* accCorrectedValues);

/**
*@brief A function that enables external interupt on change for GPIOE pin 0
*@retval None
*@warning This function can only be called after initACC().
*/
void initEXTIACC(void);

/**
*@brief A function that displays the dominant angle of the board.
*@param[in] accCorrectedValues The calibrated and filtered values from the accelerometer
*@retval None
*@note ANGLE_THRESHOLD must be set to a value greater than 0
*/
void displayDominantAngle(float* accCorrectedValues);

/**
*@brief A function that displays the direction of the board movement
*@param[in] accCorrectValues The calibrated and filtered values from the accelerometer
*@retval None
*/
void displayBoardMovement(float* accCorrectedValues, float* previousValues, float* accelerationTotals);

#endif
