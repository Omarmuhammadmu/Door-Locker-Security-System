/*
 ================================================================================================
 File Name: app.h
 Name        : Door Locker Security System
 Author      : Omar Muhammad
 Description : Header file of Control_ECU CONTROL_ECU is responsible for all the processing and decisions in the system
               like password checking, open the door and activate the system alarm.
 Date        : 26/10/2022
 ================================================================================================
 */

#ifndef APP_H_
#define APP_H_

#include "MCAL/std_types.h"

#define UART_BAUDRATE                    9600
#define PASSWORD_SIZE                    5
#define PASSWORD_ADDRESS_IN_EEPROM       0x0311
#define MEMORY_ADDRESS                   0x01
#define SAVED_PASSWORD                   108
#define FUNCTIONS_ARRAY_OF_POINTERS_SIZE 3

#define ERRORTRIALS                      3

/*Error state*/
#define DELAY_MINUTE                     60

/*DCMotor Speeds*/
#define MAX_SPEED                        100
#define ZERO_SPEED                       0

/*UART Commands and keywords*/
#define GET_READY                       0x00F1
#define READY                           0x00F2
#define STATE                           0x00F3
#define SENDING                         0x00F4
#define RECEIVED                        105
#define FINISHED                        107

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
 * Description :
 * 1. Check if the system was used previously or not
 * 2. Set the system state based on the saved status in EEPROM memory*/
void systemUsage (void);

/*
 * * Description :
 *    Set the state of the system whether to create password (as for the first time
 *    or to repeat the creating process as the password wasn't matched)
 *    or to move to main options
 */
void setSystemState (void);

/*
 * Description :
 * Receive the password from the HMI_ECU through UART
 */
void receiveCommand (void);

/*
 * Description :
 * 1. Receive two entered password from the user
 * 2. Check if the two passwords are matched and save one
 *    in the EEPROM memory
 * 3. If passwords not matched set repeat Creation flag
 */
void createSystemPassword(void);

/*
 * Description :
 * Receive the password from the HMI_ECU through UART
 */
void receivePassword (uint8 *password_Ptr);

/*
 * Description :
 * 1. Receive the password from the HMI_ECU through UART
 * 2. Check if the received password matches to that in the memory
 * 3. commands the HMI_ECU what to do next
 */
void mainOptions (void);

/*
 * Description :
 * open and close the motor
 */
void openDoor(void);

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
 * Activate the buzzer for a one minute
 */
void errorState (void);

/*
 * Description :
 * Sync the two micro-controllers
 */
void syncMicroCOntrollers(void);

/* Array of pointers to the three main function  */
void (*ptr_states[FUNCTIONS_ARRAY_OF_POINTERS_SIZE])(void) = {createSystemPassword, mainOptions, errorState};

#endif /* APP_H_ */
