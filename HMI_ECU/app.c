/*
 ================================================================================================
 File Name: app.c
 Name        : Door Locker Security System
 Author      : Omar Muhammad
 Description : Source file of HMI_ECU (Human Machine Interface) is just responsible interaction with the user
               just take inputs through keypad and display messages on the LCD.
 Date        : 26/10/2022
 ================================================================================================
 */

#include "app.h"
#include "HAL/lcd.h"
#include "HAL/keypad.h"
#include "MCAL/timer.h"
#include "MCAL/uart.h"
#include <util/delay.h>
#include <avr/io.h> /* To enable I- bit*/

/********************************************************************
 *                           Global variables
 ********************************************************************/
uint8 g_passArray[PASSWORD_SIZE]; /* Global array to keep the read password*/
system_state g_systemState;       /* Global variable to keep system state*/
uint8 g_seconds;                  /* Global variable to count seconds*/

/* Main function*/
int main(void)
{
	UART_ConfigType UART_Config = {EIGHT_BIT,PARITY_OFF,ONEBIT,UART_BAUDRATE};

	LCD_init();              /* Initialize the LCD Module*/
	UART_init(&UART_Config); /* Initialize the UART Module*/

	SREG |= (1<<7);       /* Enable I-Bit for Interrupts */

	/*Display system name message on the LCD*/
	LCD_moveCursor(0,3);
	LCD_displayString("Door locker");
	LCD_moveCursor(1,1);
	LCD_displayString("Security system");
	_delay_ms(SYSTEM_OPENING_DELAY);

	/*Set status*/
	setSystemState ();

	while(1)
	{
		/* calling functions from the array of functions */
		(*ptr_states[g_systemState])();
	}

	return 0;
}

/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/

/*
 * Description :
 *    Set the state of the system whether to create password (as for the first time
 *    or to repeat the creating process as the password wasn't matched)
 *    or to move to main options*/

void setSystemState (void)
{
	system_state state;

	/*Sync the two micro-controllers */
	syncMicroCOntrollers();

	/*----------------------------------------
	 *  Set the state of the system
	 *  --------------------------------------*/

	/* Get the next state of the system based on the password state*/
	UART_sendByte(STATE);
	while(UART_recieveByte() != SENDING){}
	state = UART_recieveByte();

	if(state == SETUP)
		g_systemState = CREATE_SYSTEM;
	else if (state == ERRORSYSTEM)
	{
		g_systemState = ERROR_STATE;
	}
	else if (state == STARTUP)
	{
		g_systemState = MAIN_OPTION;
	}
	else
	{
		g_systemState = OPEN_DOOR;
	}
}

/*
 * Description :
 * 1. Display messages to guide the user to create password
 * 2. Read the password from the user and send it to the Control_ECU
 *    through the UART Module to save it in memory and to confirm the
 *    password in the second attempt
 */
void createSystemPassword(void)
{
	/* Send command to the Control_Ecu to store two coming passwords */
	sendCommand (CREATE_TWO_PASSWORD);

	/*Entering the password messages
	 * -----------------------------------------------------
	 * 1. Display on the LCD to enter the password*/
	LCD_clearScreen();
	LCD_displayString("Plz enter pass:");
	LCD_moveCursor(1,0);
	/*2. Reading the password from the user*/
	ReadPassword();
	/*3. Send the password to the Control_ECU*/
	SendPassword(g_passArray);

	/*Confirmation of entered password messages
	 * --------------------------------------------------
	 * 1. Display on the LCD to re-enter the password*/
	LCD_moveCursor(0,0);
	LCD_displayString("Plz re-enter the");
	LCD_moveCursor(1,0);
	LCD_displayString("same pass:");
	/*2. Read the password again from the user*/
	ReadPassword ();
	/*3. Send the password to the Control_ECU*/
	SendPassword(g_passArray);

	/* Receive from the Control_ECU the next state (action)*/
	setSystemState ();
}

/*
 * Description :
 * Read the password from the user and store it in an array
 */
void ReadPassword (void)
{
	uint8 counter;
	uint8 PasswordDigit;

	/*Loop to get the password correctly from the user*/
	for(counter = 0; counter<= PASSWORD_SIZE-1; counter++)
	{
		/*Loop to ensure that the entered character from the keypad
		 * is within the accepted range*/
		do
		{
			PasswordDigit = KEYPAD_getPressedKey();

			if((PasswordDigit <= 9) && (PasswordDigit >= 0))
			{
				g_passArray[counter] = PasswordDigit;
				LCD_displayCharacter('*');
				break;
			}
		}while(1);
		_delay_ms(PRESS_TIME); /* Press time */
	}

	while(PasswordDigit != '=')
	{
		PasswordDigit = KEYPAD_getPressedKey();
	}
}

/*
 * Description :
 * Send the password to the Control_ECU through UART
 */
void SendPassword (const uint8 *password_Ptr)
{
	uint8 counter;

	/*Send the password*/
	for(counter = 0; counter <= (PASSWORD_SIZE-1); counter++)
	{
		UART_sendByte(password_Ptr[counter]);
	}
	while(UART_recieveByte() != RECEIVED){}
}

/*
 * Description :
 * 1. Display main option message
 * 2. Read the desired decision from the user
 * 3. Read and check password with Contol_ECU
 * 4. Set the state of the system to open door or change password
 */

void mainOptions(void)
{
	uint8 option;
	LCD_moveCursor(0,0);
	LCD_displayString("+ : Open Door   ");
	LCD_moveCursor(1,0);
	LCD_displayString("- : Change Pass ");

	do
	{
		option = KEYPAD_getPressedKey();
	}while(option != '+' && option != '-');

	/* Check authority by entering first the correct saved passwordS*/
	checkAuthority();

	if(option == '-')
	{
		/* Change password command*/
		/* Send command to the Control_Ecu to store one coming passwords */
		sendCommand (CHANGE);
	}
	else if (option == '+')
	{
		/*Open door command*/
		/* Send command to the Control_Ecu to store one coming passwords */
		sendCommand (OPEN);
	}

	/*Get the next state of the system*/
	setSystemState ();

	if(g_systemState == OPEN_DOOR)
	{
		openDoorScreen();
		/*Get the next state of the system*/
		setSystemState ();
	}
}

/*
 * Description :
 * 1. Read password from the user
 * 2. communicate with the Control_ECU to check if the
 * entered password is like that saved in the memory or not
 */
void checkAuthority(void)
{
	/* Send command to the Control_Ecu to store one coming passwords */
	sendCommand (CHECK_PASSWORD);

	do{
		/* 1. Display on the LCD to enter the password*/
		LCD_clearScreen();
		LCD_displayString("Plz enter pass:");
		LCD_moveCursor(1,0);
		/*2. Read the password from the user*/
		ReadPassword ();
		/*3. Send the password to the Control_ECU*/
		SendPassword(g_passArray);

	}while(UART_recieveByte() == READ_AGAIN);
}

/*
 * Description :
 * Display the state of the door
 */
void openDoorScreen(void)
{
	LCD_clearScreen();
	LCD_displayString("    Door is ");
	LCD_moveCursor(1,0);
	LCD_displayString(" 	 Unlocking");
	delaySeconds(15);

	LCD_clearScreen();
	LCD_displayString("  Door opened");
	delaySeconds(3);

	LCD_clearScreen();
	LCD_displayString(" Door is locking");
	delaySeconds(15);
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

	Timer1_init(&timer1Config); /* Initialize the TIMER1 Module*/

	while(g_seconds < sec){}
	g_seconds = 0;

	Timer1_deInit();           /* Disable the TIMER1 Module*/

}

/*
 * Description :
 * 1. Display error message on the LCD
 * 2. Hold the keypad response for one minute
 */
void errorState (void)
{
	sendCommand (FalsePassword);

	/* Display on the LCD Error message*/
	LCD_clearScreen();
	LCD_moveCursor(ERROR_MESSAGEO_ROW,ERROR_MESSAGEO_COLUMN);
	LCD_displayString("ERROR!");

	LCD_moveCursor(1,0);
	LCD_displayString("Wrong  Password");
;

	/* Delay for one minute*/
	delaySeconds(DELAY_MINUTE);

	g_systemState = MAIN_OPTION;
}

/*
 * Description :
 * UART send command to Control_ECU
 */
void sendCommand (uint8 a_command)
{
	UART_sendByte(a_command);
	while(UART_recieveByte() != FINISHED){}
}

/*
 * Description :
 * Sync the two micro-controllers
 */
void syncMicroCOntrollers(void)
{
	UART_sendByte(GET_READY);
	while(UART_recieveByte() != READY){}
}


