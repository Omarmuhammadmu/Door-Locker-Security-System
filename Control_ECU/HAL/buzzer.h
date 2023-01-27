/******************************************************************************
 *
 * Module: Buzzer
 *
 * File Name: buzzer.h
 *
 * Description: Header file for the Buzzer driver
 *
 * Author: Omar Muhammad
 *
 *******************************************************************************/

#ifndef HAL_BUZZER_H_
#define HAL_BUZZER_H_

/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/

/* Buzzer HW Ports and Pins Ids */
#define BUZZER_PORT_ID                 PORTC_ID
#define BUZZER_PIN_ID                  PIN7_ID

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/
/*
 * Description :
 * Setup the direction for the buzzer pin as output pin through the GPIO driver.
 * Turn off the buzzer through the GPIO.*/
void Buzzer_init();

/*
 * Description :
 * Function to enable the Buzzer through the GPIO.*/
void Buzzer_on(void);

/*
 * Description :
 * Function to disable the Buzzer through the GPIO.*/
void Buzzer_off(void);

#endif /* HAL_BUZZER_H_ */
