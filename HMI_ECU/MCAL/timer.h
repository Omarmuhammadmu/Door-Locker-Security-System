/******************************************************************************
 *
 * Module: TIMER
 *
 * File Name: timer.h
 *
 * Description: header file for the AVR atmega32 timers driver
 *
 * Author: Omar Muhammad
 *
 *******************************************************************************/

#ifndef TIMER_H_
#define TIMER_H_

#include "std_types.h"

/*******************************************************************************
 *                         Types Declaration                                   *
 *******************************************************************************/

typedef enum
{
	NOCLOCK, F_CPU_CLK, F_CPU_8 , F_CPU_64 , F_CPU_256 , F_CPU_1024
}Timer1_Prescaler;

typedef enum
{
	NORMAL, CTC
}Timer1_Mode;

typedef struct {
	uint16 initial_value;
	uint16 compare_value; // it will be used in compare mode only.
	Timer1_Prescaler prescaler;
	Timer1_Mode mode;
} Timer1_ConfigType;

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/
/*
 * Description :
 * Function to initialize the Timer driver
 * 1. Set the required clock.
 * 2. Set the required mode (normal or CTC).
 * 3. Enable Timer Module interrupt
 * 4. Initialize Timer1 Registers
 */
void Timer1_init(const Timer1_ConfigType * Config_Ptr);

/*
 * Description :
 * Function to disable the Timer1.
 */
void Timer1_deInit(void);

/*
 * Description :
 * Function to set the Call Back function address.
 */
void Timer1_setCallBack(void(*a_ptr)(void));

#endif /* TIMER_H_ */
