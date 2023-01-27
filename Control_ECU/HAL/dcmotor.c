/******************************************************************************
 *
 * Module: DC-Motor
 *
 * File Name: dcmotor.c
 *
 * Description: Header file for the DC-motor driver
 *
 * Author: Omar Muhammad
 *
 *******************************************************************************/

#include "../MCAL/common_macros.h" /* For SET_BIT and CLEAR_BIT Macro */
#include "../MCAL/gpio.h"
#include "dcmotor.h"
#include "../MCAL/pwm.h"

/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/

/*
 * Description :
 * Initialize the DC-Motor:
 * 1. Setup the direction for the two motor pins through the GPIO driver.
 * 2. Stop the DC-Motor at the beginning through the GPIO driver .
 */
void DCMOTOR_init(void)
{
	/* Configure the direction for INT1 and INT2 of H_bridge pins as output pins */
	GPIO_setupPinDirection(DCMOTOR_INT1_PORT_ID,DCMOTOR_INT1_PIN_ID,PIN_OUTPUT);
	GPIO_setupPinDirection(DCMOTOR_INT2_PORT_ID,DCMOTOR_INT2_PIN_ID,PIN_OUTPUT);

	/* Stop the DC-Motor at the beginning through the GPIO driver
	 * by writing logical low on both INT1 and INT2 pins of the H_bridge*/
	GPIO_writePin(DCMOTOR_INT1_PORT_ID,DCMOTOR_INT1_PIN_ID, LOGIC_LOW);
	GPIO_writePin(DCMOTOR_INT2_PORT_ID,DCMOTOR_INT2_PIN_ID, LOGIC_LOW);
}

/*
 * Description :
 * 1. Rotate the DC Motor clockwise (CW), anti-clockwise (A_CW), or
 *    stop the motor based on the state input state value.
 * 2. Send the required duty cycle to the PWM driver based on the
 *    required speed value.
 */

void DcMotor_Rotate(DcMotor_State state, uint8 speed)
{
	/* Set the direction of the rotation or stop the motor */
	switch (state)
	{
	case CW:
		GPIO_writePin(DCMOTOR_INT1_PORT_ID,DCMOTOR_INT1_PIN_ID, LOGIC_HIGH);
		GPIO_writePin(DCMOTOR_INT2_PORT_ID,DCMOTOR_INT2_PIN_ID, LOGIC_LOW);
		break;
	case A_CW:
		GPIO_writePin(DCMOTOR_INT1_PORT_ID,DCMOTOR_INT1_PIN_ID, LOGIC_LOW);
		GPIO_writePin(DCMOTOR_INT2_PORT_ID,DCMOTOR_INT2_PIN_ID, LOGIC_HIGH);
		break;
	case STOP:
		GPIO_writePin(DCMOTOR_INT1_PORT_ID,DCMOTOR_INT1_PIN_ID, LOGIC_LOW);
		GPIO_writePin(DCMOTOR_INT2_PORT_ID,DCMOTOR_INT2_PIN_ID, LOGIC_LOW);
		break;
	}
	/* Send the duty cycle to the PWM driver */
	PWM_Timer0_Start(speed);
}
