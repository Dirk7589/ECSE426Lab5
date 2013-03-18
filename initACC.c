/**@file initACC.c
*@Author Dirk Dubois, Alain Slak
*@Date February 13, 2013
*/

/*Includes*/
#include "initACC.h"
#include "common.h"
#include "stm32f4_discovery_lis302dl.h"
#include <math.h>

/*Defines*/
#define GREEN_LED 0x1000 /*!<Defines the bit location of the green LED*/
#define ORANGE_LED 0x2000 /**< Defines the bit location of the orange LED*/
#define ORANGE_LED_OFF 0x1FFF
#define RED_LED 0x4000 /**< Defines the bit location of the red LED*/
#define BLUE_LED 0x8000	/**< Defines the bit location of the blue LED*/

/*Global Variables*/

float ZOffset = 29.02; 
float ZSensitivityPos = 1.029;
float ZSensitivityNeg = 1.0299;

float YOffset = -15.87; 
float YSensitivityPos = 1.0161;
float YSensitivityNeg = 0.9844;

float XOffset = 11.26; 
float XSensitivityPos = 0.9889;
float XSensitivityNeg = 1.0114;

/*Includes*/
#include <math.h>

/**
*@brief A function that initalizes the ACC over SPI
*The function enables the LIS302DL accelerometer for all three axes, sets the data rate to 100Hz for SPI1,
*and enables low power mode. It also enables interupts for the single click tap mode.
*@retval None
*@note This function automatically enables and configures SPI1 for use for the accelerometer.
*@note LIS302DL_Init correctly configures the corresponding GPIO pin for its interupts.
*/
void initACC(void)
{
		
		uint8_t ctrl; //Variable used to set control registers
		
		//Enable the LIS302DL
    LIS302DL_InitTypeDef initStructACC; //Define structs
    
    initStructACC.Power_Mode = LIS302DL_LOWPOWERMODE_ACTIVE; //Turn low power mode on
    initStructACC.Output_DataRate = LIS302DL_DATARATE_100; //Set data rate to 100
    initStructACC.Axes_Enable = LIS302DL_X_ENABLE | LIS302DL_Y_ENABLE | LIS302DL_Z_ENABLE; //Turn on all access
    initStructACC.Full_Scale = LIS302DL_FULLSCALE_2_3;  //Set measurement range for g scale to +- 2.3g
    initStructACC.Self_Test = LIS302DL_SELFTEST_NORMAL; //Turn on self test in normal mode
    
    LIS302DL_Init(&initStructACC); //Intialize the LIS302DL
	
		//Enable interupts for Tap Detection for the LIS302DL
    LIS302DL_InterruptConfigTypeDef LIS302DL_InterruptConfigStruct; //Define struct
    
		ctrl = 0x38; //Correct mask value for click interupt
		LIS302DL_Write(&ctrl, LIS302DL_CTRL_REG3_ADDR, 1); //Configure control register for click interupt
	
    LIS302DL_InterruptConfigStruct.Latch_Request = LIS302DL_INTERRUPTREQUEST_NOTLATCHED; //Latch interupt request
    LIS302DL_InterruptConfigStruct.SingleClick_Axes = LIS302DL_CLICKINTERRUPT_Z_ENABLE; //Enable single click interupt for all 3 axes
    LIS302DL_InterruptConfigStruct.DoubleClick_Axes = LIS302DL_DOUBLECLICKINTERRUPT_XYZ_DISABLE; //Disable double click
    
    LIS302DL_InterruptConfig(&LIS302DL_InterruptConfigStruct); //Intialize the LIS302DL interupts behavior

		//Set remaining properties of the click interupt not configured by LIS302DL_InterruptConfig
		//They can be found in the LIS302DL data sheet section 7.
		ctrl = 0xFF;
		LIS302DL_Write(&ctrl, LIS302DL_CLICK_THSY_X_REG_ADDR, 1); //Setup minimum threshold for an interupt to occur
		
		ctrl = 0x0F;
		LIS302DL_Write(&ctrl, LIS302DL_CLICK_THSZ_REG_ADDR, 1); //Setup minimum threshold for an interupt to occur
		
		ctrl = 0x03;
		LIS302DL_Write(&ctrl, LIS302DL_CLICK_TIMELIMIT_REG_ADDR, 1); //Set time limit between interupt
		
		ctrl = 0xFA;
		LIS302DL_Write(&ctrl, LIS302DL_CLICK_LATENCY_REG_ADDR, 1); //Set latency before interupt is tripped
		
		ctrl = 0xFF;
		LIS302DL_Write(&ctrl, LIS302DL_CLICK_WINDOW_REG_ADDR, 1); //Set the window for latency and time limit.
} 

/**
*@brief A function that converts the corrected and filtered acceleration values to an angle in degrees.
*@param[in] accValues the passed in accelerations
*@param[out] angles the resulting roll and pitch angles in degress
*@retval None
*@note atanf() expects a double and returns a double, this implementation explicitly castes everything to float
*@warning CONVERSION_TO_DEG must be set in order for the function to return the value in degrees,
*if the user wants the value in radians set CONVERSION_TO_DEG to 1.
*/
void toAngle(float* accValues, float* angles)
{
    //Pull the accValues
    float xValue = accValues[0];
    float yValue = accValues[1];
    float zValue = accValues[2];
    
    float roll = 0; 
    float pitch = 0;
    
    roll = yValue*yValue + zValue*zValue; //sqaure denominator
    roll = sqrtf(roll); //Square root denominator
    roll = xValue / roll; //Divide by numerator
    roll = atanf(roll); //Take arctan
    
    roll = (float)(roll * CONVERSION_TO_DEG); //Convert to degrees and return as float
    
    
    pitch = xValue*xValue + zValue*zValue; //sqaure denominator
    pitch = sqrtf(pitch); //Square root denominator
    pitch = yValue / pitch; //Divide by numerator
    pitch = atanf(pitch); //Take arctan
    
    pitch = (float)(pitch * CONVERSION_TO_DEG);
    
    //Store results
    angles[0] = roll;
    angles[1] = pitch;
    
}

/**
*@brief A function that calibrates the accelerometer.
*@param[in] accValues The uncorrected inputed accelerometer values
*@param[out] accCorrectedValues The resulting corrected accelerometer values
*@warning This function can only be called once initACC has been called
*/
void calibrateACC(int32_t* accValues, float* accCorrectedValues)
{
	
	if(accValues[2] < 0)
		accCorrectedValues[2] = (float)((accValues[2] + ZOffset)*ZSensitivityNeg);
	else if(accValues[2] > 0)
		accCorrectedValues[2] = (float)((accValues[2] - ZOffset)*ZSensitivityPos);
	
	if(accValues[1] < 0)
		accCorrectedValues[1] = (float)((accValues[1] - YOffset)*YSensitivityNeg);
	else if(accValues[1] > 0)
		accCorrectedValues[1] = (float)((accValues[1] - YOffset)*YSensitivityPos);
		
	if(accValues[0] < 0)
		accCorrectedValues[0] = (float)((accValues[0] + XOffset)*XSensitivityNeg);
	else if(accValues[0] > 0)
		accCorrectedValues[0] = (float)((accValues[0] - XOffset)*XSensitivityPos);
	
}

/**
*@brief A function that enables external interupt on change for GPIOE on pin 0.
*@retval None
*@warning This function can only be called after initACC().
*/
void initEXTIACC(void)
{
	//The GPIO Ports for accelerometer interupts are configured in LIS302DL_Init
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE); //Enable peripheral clock for EXTI
	
	//Setup interupts
	EXTI_InitTypeDef exti_init;

	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource1);	//Select pin to interupt from
	
	exti_init.EXTI_Line = EXTI_Line1; //Select line 0
	exti_init.EXTI_LineCmd = ENABLE; //Enable
	exti_init.EXTI_Mode = EXTI_Mode_Interrupt; //Interupt mode
	exti_init.EXTI_Trigger = EXTI_Trigger_Rising; //Rising edge trigger
	
	EXTI_Init(&exti_init);	//Configure the interupt mode

	//Enable the NVIC if needed
	NVIC_InitTypeDef NVIC_Struct; //Create intialization struct for NVIC
	
	NVIC_Struct.NVIC_IRQChannel = EXTI1_IRQn; //Select EXTI0
	NVIC_Struct.NVIC_IRQChannelPreemptionPriority = 0; //Set preemption priority
	NVIC_Struct.NVIC_IRQChannelSubPriority = 1; //Set sub prioirity
	NVIC_Struct.NVIC_IRQChannelCmd = ENABLE; //Enable NIVC
	
	NVIC_Init(&NVIC_Struct); //Setup NVIC with struct//Configure the NVIC for use with EXTI

}

/**
*@brief A function that displays the dominant angle of the board.
*@param[in] accCorrectedValues The calibrated and filtered values from the accelerometer
*@retval None
*/
void displayDominantAngle(float* accCorrectedValues)
{
	toAngle(accCorrectedValues, angles); //Convert to pitch and roll
	
	if(angles[0] > 0 && angles[0] > ANGLE_THRESHOLD){
		//GPIOD->BSRRH = BLUE_LED; //Turn off other LED
		GPIOD->BSRRL = ORANGE_LED; //Roll to the right
	}
	else{
		GPIOD->BSRRH = ORANGE_LED;
	}
	
	if(angles[0] < 0 && angles[0] < -ANGLE_THRESHOLD){
		//GPIOD->BSRRH = ORANGE_LED; //Turn off LED
		GPIOD->BSRRL = BLUE_LED; //Roll to the left
	}
	else{
		GPIOD->BSRRH = BLUE_LED;
	}
	
	if(angles[1] > 0 && angles[1] > ANGLE_THRESHOLD){
		//GPIOD->BSRRH = RED_LED; //Turn off other LED
		GPIOD->BSRRL = GREEN_LED; //Pitch forward
	}
	else{
		GPIOD->BSRRH = GREEN_LED;
	}
	
	if(angles[1] < 0 && angles[1] < -ANGLE_THRESHOLD){
		//GPIOD->BSRRH = GREEN_LED; //Turn off LED
		GPIOD->BSRRL = RED_LED; //Pitch backwards
	}
	else{
		GPIOD->BSRRH = RED_LED;
	}
}

/**
*@brief A function that displays the direction of the board movement
*@param[in] accCorrectValues The calibrated and filtered values from the accelerometer
*@retval None
*/
void displayBoardMovement(float* accCorrectedValues, float* previousValues, float* accelerationTotals)
{	
	float accelerationDiff[2] = {0,0};

	//calculate the difference between the new and previous acceleration values
	accelerationDiff[0] = accCorrectedValues[0] - previousValues[0];
	accelerationDiff[1] = accCorrectedValues[1] - previousValues[1];
	
	//store the new values in previousValues to be used in the next iteration
	previousValues[0] = accCorrectedValues[0];
	previousValues[1] = accCorrectedValues[1];

	//if the absolute value of the difference is above MOVEMENT_THRESHOLD then add the difference to accelerationTotals
	if(accelerationDiff[0] > MOVEMENT_THRESHOLD || accelerationDiff[0] < -MOVEMENT_THRESHOLD){//&& absDiff0 < 500){
		accelerationTotals[0] = accelerationTotals[0] + accelerationDiff[0];
	}
	if(accelerationDiff[1] > MOVEMENT_THRESHOLD || accelerationDiff[1] < -MOVEMENT_THRESHOLD){//&& absDiff1 < 500){
		accelerationTotals[1] = accelerationTotals[1] + accelerationDiff[1];
	}
	
	//if the absolute value of accelerationTotals is smaller than MOVEMENT_THRESHOLD then reset accelerationTotals
	if(accelerationTotals[0] < MOVEMENT_THRESHOLD && accelerationTotals[0] > -MOVEMENT_THRESHOLD){
		accelerationTotals[0] = 0;
	}
	if(accelerationTotals[1] < MOVEMENT_THRESHOLD && accelerationTotals[1] > -MOVEMENT_THRESHOLD){
		accelerationTotals[1] = 0;
	}
	
	//if the absolute value of accelerationTotals gets too large then reset it
	//this prevents runaway
	if(accelerationTotals[0] > 1000 || accelerationTotals[0] < -1000){
		accelerationTotals[0] =0;
	}
	if(accelerationTotals[1] > 1000 || accelerationTotals[1] < -1000){
		accelerationTotals[1] = 0;
	}
	
	#if DEBUG
	printf("x-value: %f\n", accelerationTotals[0]); 
	printf("y-value: %f\n", accelerationTotals[1]); 
	#endif
	
	//check if accelerationTotals is above MOVEMENT_LED_THRESHOLD in each direction and set the LEDs accordingly
	if(accelerationTotals[0] > MOVEMENT_LED_THRESHOLD){
		GPIOD->BSRRL = BLUE_LED; //Moving in positive x direction
	}
	else{
		GPIOD->BSRRH = BLUE_LED;
	}
	if(accelerationTotals[0] < -MOVEMENT_LED_THRESHOLD){
		GPIOD->BSRRL = ORANGE_LED; //Moving in negative x direction
	}
	else{
		GPIOD->BSRRH = ORANGE_LED;
	}
	if(accelerationTotals[1] > MOVEMENT_LED_THRESHOLD){
		GPIOD->BSRRL = RED_LED; //Moving in positive y direction
	}
	else{
		GPIOD->BSRRH = RED_LED;
	}
	if(accelerationTotals[1] < -MOVEMENT_LED_THRESHOLD){
		GPIOD->BSRRL = GREEN_LED; //Moving in negative y direction
	}
	else{
		GPIOD->BSRRH = GREEN_LED;
	}
}

