/*
 ================================================================================================
 File Name: app.c
 Name        : Door Locker Security System
 Author      : Omar Muhammad
 Description : Source file of Control_ECU CONTROL_ECU is responsible for all the processing and decisions in the system
               like password checking, open the door and activate the system alarm.
 Date        : 26/10/2022
 ================================================================================================
 */

#include "app.h"
#include "MCAL/timer.h"
#include "HAL/buzzer.h"
#include "HAL/dcmotor.h"
#include "MCAL/uart.h"
#include "MCAL/twi.h"
#include "HAL/external_eeprom.h"
#include <util/delay.h>
#include <avr/io.h> /* To enable I- bit*/

/********************************************************************
 *                           Global variables
 ********************************************************************/
/*Two global arrays to hold the values of the received password from HMI_ECU*/
uint8 g_password[PASSWORD_SIZE], g_confirmpass[PASSWORD_SIZE];

uint8 commandReceiver;            /* Global variable to store the command of the action that is needed to be taken*/
system_state g_systemState;       /* Global variable to keep system state*/
uint8 g_seconds;                  /* Global variable to count seconds*/

/* Main function*/
int main(void)
{
	UART_ConfigType UART_Config = {EIGHT_BIT,PARITY_OFF,ONEBIT,UART_BAUDRATE};
	TWI_ConfigType  TWI_Config = {FAST_MODE, MEMORY_ADDRESS};

	Buzzer_init();           /* Initialize the buzzer Module*/
	DCMOTOR_init();          /* Initialize the DC-Motor Module*/
	UART_init(&UART_Config); /* Initialize the UART Module*/
	TWI_init(&TWI_Config);   /* Initialize the I2C Module*/

	SREG |= (1<<7);       /* Enable I-Bit for Interrupts */

	systemUsage();

	/*Set status*/
	setSystemState ();

	while(1)
	{
		/*Receive command from HME_ECU*/
		receiveCommand();

		/* calling functions from the array of functions */
		(*ptr_states[commandReceiver])();
	}

	return 0;
}


/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/

/*
 * Description :
 * 1. Check if the system was used previously or not
 * 2. Set the system state based on the saved status in EEPROM memory*/
void systemUsage (void)
{
	/* Variable used to read in it if there is a password saved or not*/
	uint8 PasswordFlag;

	/* Read password saved status from EEPROM */
	EEPROM_readByte(PASSWORD_ADDRESS_IN_EEPROM - 1, &PasswordFlag);

	if(PasswordFlag == SAVED_PASSWORD)
		/* Set the system state to the main options menu*/
		g_systemState = STARTUP;
	else
		/* Set the system state to create password option*/
		g_systemState = SETUP;
}

/*
 * * Description :
 *    Set the state of the system whether to create password (as for the first time
 *    or to repeat the creating process as the password wasn't matched)
 *    or to move to main options
 */
void setSystemState (void)
{
	/*Sync the two micro-controllers */
	syncMicroCOntrollers();
	/*----------------------------------------
	 *  Set the state of the system
	 *  --------------------------------------*/
	while(UART_recieveByte() != STATE){}
	UART_sendByte(SENDING);
	UART_sendByte(g_systemState);
}

/*
 * Description :
 * 1. Receive two entered password from the user
 * 2. Check if the two passwords are matched and save one
 *    in the EEPROM memory
 * 3. If passwords not matched set repeat Creation flag
 */
void createSystemPassword(void)
{
	uint8 matchedFlag = 1;
	uint8 counter;
	/*Receive the first password from the HMI_ECU*/
	receivePassword (g_password);
	/*Receive the second password from the HMI_ECU*/
	receivePassword (g_confirmpass);

	/*Check if the two received passwords are the matched*/
	for(counter = 0; counter <= (PASSWORD_SIZE-1); counter++)
	{
		if(g_password[counter] != g_confirmpass[counter])
		{
			matchedFlag = 0;
			break;
		}
	}

	if(matchedFlag == 1)
	{
		/*Save password in memory*/
		for(counter = 0; counter <= (PASSWORD_SIZE-1); counter++)
		{
			/* Write each digit in the external EEPROM */
			EEPROM_writeByte((PASSWORD_ADDRESS_IN_EEPROM + counter), g_password[counter]);
			_delay_ms(10);
		}

		/*Save a certain value in the memory to check if there is a saved
		 * password or not*/
		EEPROM_writeByte((PASSWORD_ADDRESS_IN_EEPROM - 1), SAVED_PASSWORD);
		_delay_ms(10);
		/*Set system state to main options*/
		g_systemState = STARTUP;

	}
	else
	{
		/*Set system state to create password from the beginning*/
		g_systemState = SETUP;
	}
	setSystemState ();
}

/*
 * Description :
 * Receive the password from the HMI_ECU through UART
 */
void receivePassword (uint8 *password_Ptr)
{
	uint8 counter;
	//syncMicroCOntrollers();
	for(counter = 0; counter <= (PASSWORD_SIZE-1); counter++)
	{
		password_Ptr [counter] = UART_recieveByte();
	}
	UART_sendByte(RECEIVED);
}

/*
 * Description :
 * 1. Receive the password from the HMI_ECU through UART
 * 2. Check if the received password matches to that in the memory
 * 3. commands the HMI_ECU what to do next
 */
void mainOptions (void)
{
	uint8 errorTrials = 0;      /* variable to count number of false trials for entering the password*/
	uint8 passwordState;        /* variable used as a flag to send read again command or not*/
	uint8 counter;

	do{
		receivePassword (g_password);
		passwordState = MATCHED;

		/*compare received password with the saved one*/
		for(counter = 0; counter <= PASSWORD_SIZE-1; counter++)
		{
			uint8 readByte;
			EEPROM_readByte(PASSWORD_ADDRESS_IN_EEPROM + counter, &readByte);

			if(g_password[counter] != readByte)
			{
				errorTrials++;
				passwordState = READ_AGAIN;
				break;
			}
		}

		/*Send check flag value*/
		if(errorTrials <= ERRORTRIALS-1)
			UART_sendByte(passwordState);
		else
		{
			passwordState = ERRORSYSTEM;
			UART_sendByte(passwordState);
			g_systemState = ERRORSYSTEM;

		}
	}while(passwordState == READ_AGAIN);


	receiveCommand();
	if(errorTrials == ERRORTRIALS)
	{
		setSystemState ();
		return;
	}
	else if (commandReceiver == CHANGE)
	{
		g_systemState = SETUP;
		setSystemState ();
	}
	else
	{
		g_systemState = OPEN_DOOR;
		setSystemState ();
		openDoor();
		g_systemState = STARTUP;
		setSystemState ();
	}
}

/*
 * Description :
 * open and close the motor
 */
void openDoor(void)
{
	DcMotor_Rotate(CW, MAX_SPEED);
	delaySeconds(15);

	/* Hold the door for 3 sec */
	DcMotor_Rotate(STOP, ZERO_SPEED);
	delaySeconds(3);

	/* lock the door for 15 sec */
	DcMotor_Rotate(A_CW, MAX_SPEED);
	delaySeconds(15);

	/* Stop the motor */
	DcMotor_Rotate(STOP, ZERO_SPEED);
}

/*
 * Description :
 * Callback function of the timer
 */
void countSec(void)
{
	g_seconds++;
}

/*
 * Description :
 * Delay function by seconds operates with Timer1
 */
void delaySeconds(uint8 sec)
{
	/* setting the callBack function to count seconds */
	Timer1_setCallBack(countSec);

	/* timer1 configurations to count sec per interrupt */
	Timer1_ConfigType timer1Config = {0,31250,F_CPU_256,CTC};

	Timer1_init(&timer1Config);

	while(g_seconds < sec){}
	g_seconds = 0;

	Timer1_deInit();
}

/*
 * Description :
 * Activate the buzzer for a one minute
 */
void errorState (void)
{
	Buzzer_on();

	delaySeconds(DELAY_MINUTE);

	Buzzer_off();
}

/*
 * Description :
 * Receive the password from the HMI_ECU through UART
 */
void receiveCommand(void)
{
	commandReceiver = UART_recieveByte();
	UART_sendByte(FINISHED);
}

/*
 * Description :
 * Sync the two micro-controllers
 */
void syncMicroCOntrollers(void)
{
	/*Sync the two micro-controllers */
	while(UART_recieveByte() != GET_READY){}
	UART_sendByte(READY);
}
