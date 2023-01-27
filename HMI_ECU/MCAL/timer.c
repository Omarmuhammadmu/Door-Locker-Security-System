/******************************************************************************
 *
 * Module: TIMER
 *
 * File Name: timer.c
 *
 * Description: Source file for the AVR atmega32 timers driver
 *
 * Author: Omar Muhammad
 *
 *******************************************************************************/

#include"timer.h"
#include <avr/io.h>
#include<avr/interrupt.h>

/*******************************************************************************
 *                           Global Variables                                  *
 *******************************************************************************/
/* Global variables to hold the address of the call back function in the application */
static volatile void (*g_callBackPtr)(void) = NULL_PTR;

/*******************************************************************************
 *                       Interrupt Service Routines                            *
 *******************************************************************************/

ISR(TIMER1_OVF_vect)
{
	if(g_callBackPtr != NULL_PTR)
	{
		/* Call the Call Back function in the application after the edge is detected */
		(*g_callBackPtr)();
	}
}

ISR(TIMER1_COMPA_vect)
{
	if(g_callBackPtr != NULL_PTR)
	{
		/* Call the Call Back function in the application after the edge is detected */
		(*g_callBackPtr)();
	}
}

/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/
/*
 * Description :
 * Function to initialize the Timer driver
 * 1. Set the required clock.
 * 2. Set the required mode (normal or CTC).
 * 3. Enable Timer Module interrupt
 * 4. Initialize Timer1 Registers
 */
void Timer1_init(const Timer1_ConfigType * Config_Ptr)
{
	/* Set timer1 initial count to the configured value */
		TCNT1 = Config_Ptr->initial_value;

		/* Set the Compare value to configured compare value */
		OCR1A = Config_Ptr->compare_value;

		TCCR1A = (1<<FOC1A) | (1<<FOC1B);

		/*Set overflow or CTC Mode*/
		TCCR1B = (TCCR1B & 0xF7) | (Config_Ptr->mode << 3);

		/*Set the Timer1 Prescaler*/
		TCCR1B = (TCCR1B & 0xF8) | (Config_Ptr->prescaler & 0x07);

		if(Config_Ptr->mode == NORMAL)
		{
			/* Enable Timer1 Overflow Interrupt */
			TIMSK |= (1<<TOIE1);
		}
		else
		{
			/* Enable Timer1 Compare A Interrupt */
			TIMSK |= (1<<OCIE1A);
		}
}

/*
 * Description :
 * Function to disable the Timer1.
 */
void Timer1_deInit(void)
{
	/* Clear All Timer1 Registers */
	TCCR1A = 0;
	TCCR1B = 0;
	TCNT1 = 0;
	ICR1 = 0;
}

/*
 * Description :
 * Function to set the Call Back function address.
 */
void Timer1_setCallBack(void(*a_ptr)(void))
{
	g_callBackPtr = (volatile void (*)(void))a_ptr;
}
