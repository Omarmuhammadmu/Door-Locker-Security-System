/*
 ================================================================================================
 File Name: app.h
 Name        : Door Locker Security System
 Author      : Omar Muhammad
 Description : Header file of HMI_ECU (Human Machine Interface) is just responsible interaction with the user
               just take inputs through keypad and display messages on the LCD.
 Date        : 26/10/2022
 ================================================================================================
 */

#ifndef APP_H_
#define APP_H_

#include "MCAL/std_types.h"

#define UART_BAUDRATE                    9600
#define SYSTEM_OPENING_DELAY             1000
#define PASSWORD_SIZE                    5
#define PRESS_TIME                       500
#define FUNCTIONS_ARRAY_OF_POINTERS_SIZE 3

/*UART Commands and keywords*/
#define GET_READY                       0x00F1
#define READY                           0x00F2
#define STATE                           0x00F3
#define SENDING                         0x00F4
#define RECEIVED                        105
#define FINISHED                        107

/*Error state*/
#define ERROR_MESSAGEO_ROW               0
#define ERROR_MESSAGEO_COLUMN            4
#define DELAY_MINUTE                     60

/*Control_ECU States codes*/
#define CREATE_TWO_PASSWORD             0
#define CHECK_PASSWORD                  1
#define FalsePassword                   2

#define SETUP                           109
#define STARTUP                         110
#define ERRORSYSTEM                     111
#define READ_AGAIN                      114
#define CHANGE                          115
#define OPEN                            116
#define MATCHED                         117
#define OPEN_DOOR                       118

/*******************************************************************************
 *                         Types Declaration                                   *
 *******************************************************************************/
typedef enum
{
	CREATE_SYSTEM , MAIN_OPTION , ERROR_STATE
}system_state;

/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/
/*
 *Description :
 *    Set the state of the system whether to create password (as for the first time
 *    or to repeat the creating process as the password wasn't matched)
 *    or to move to main options
 */
void setSystemState (void);

/*
 * Description :
 * UART send command to Control_ECU
 */
void sendCommand (uint8 a_command);

/*
 * Description :
 * 1. Display messages to guide the user to create password
 * 2. Read the password from the user and send it to the Control_ECU
 *    through the UART Module to save it in memory and to confirm the
 *    password in the second attempt
 */
void createSystemPassword(void);

/*
 * Description :
 * Read the password from the user and store it in an array
 */
void ReadPassword (void);

/*
 * Description :
 * Send the password to the Control_ECU through UART
 */
void SendPassword (const uint8 *password_Ptr);

/*
 * Description :
 * 1. Display main option message
 * 2. Read the desired decision from the user
 * 3. Read and check password with Contol_ECU
 * 4. Set the state of the system to open door or change password
 */
void mainOptions(void);

/* Description :
* 1. Read password from the user
* 2. communicate with the Control_ECU to check if the
* entered password is like that saved in the memory or not
*/
void checkAuthority(void);

/*
 * Description :
 * Display the state of the door
 */
void openDoorScreen(void);

/*
 * Description :
 * 1. Display error message on the LCD
 * 2. Hold the keypad response for one minute
 */
void errorState (void);

/*
 * Description :
 * Callback function of the timer
 */
void countSec(void);

/*
 * Description :
 * Delay function by seconds operates with Timer1
 */
void delaySeconds(uint8 sec);

/*
 * Description :
 * Sync the two micro-controllers
 */
void syncMicroCOntrollers(void);

/* Array of pointers to the three main function  */
void (*ptr_states[FUNCTIONS_ARRAY_OF_POINTERS_SIZE])(void) = {createSystemPassword, mainOptions, errorState};

#endif /* APP_H_ */
