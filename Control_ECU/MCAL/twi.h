/******************************************************************************
 *
 * Module: TWI(I2C)
 *
 * File Name: twi.h
 *
 * Description: Header file for the TWI(I2C) AVR driver
 *
 * Author: Omar Muhammad
 *
 *******************************************************************************/ 

#ifndef TWI_H_
#define TWI_H_

#include "std_types.h"

/*******************************************************************************
 *                      Preprocessor Macros                                    *
 *******************************************************************************/

/* I2C Status Bits in the TWSR Register */
#define TWI_START         0x08 /* start has been sent */
#define TWI_REP_START     0x10 /* repeated start */
#define TWI_MT_SLA_W_ACK  0x18 /* Master transmit ( slave address + Write request ) to slave + ACK received from slave. */
#define TWI_MT_SLA_R_ACK  0x40 /* Master transmit ( slave address + Read request ) to slave + ACK received from slave. */
#define TWI_MT_DATA_ACK   0x28 /* Master transmit data and ACK has been received from Slave. */
#define TWI_MR_DATA_ACK   0x50 /* Master received data and send ACK to slave. */
#define TWI_MR_DATA_NACK  0x58 /* Master received data but doesn't send ACK to slave. */

/*******************************************************************************
 *                         Types Declaration                                   *
 *******************************************************************************/

/*Description:
 * Normal mode : 100Kb/s
 * Fast mode: 400 kb/s
 * Fast mode Plus 1 Mb/s
 * High speed mode 3.4 Mb/s
 * */
typedef enum
{
	NORMALMODE, FAST_MODE
#if(F_CPU == 16000000ul)
	,FAST_MODE_PLUS

	/*Cannot be achieved by either 8 MHz or 16 MHz F_CPU*/
	/*Bit Rate: 3.4 Mb/s*/
	/*, HIGH_SPEED_MODE*/
#endif
}TWI_BaudRate;


typedef struct{

	uint8 address;
	TWI_BaudRate bit_rate;

}TWI_ConfigType;


/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/


/*
 *Description :
 *    Initialize I2C (TWI) Interface
 */
void TWI_init(const TWI_ConfigType * Config_Ptr);

/*
 *Description :
 *    Send the Start bit
 */
void TWI_start(void);

/*
 *Description :
 *    Send the Stop bit
 */
void TWI_stop(void);

/*
 *Description :
 *    Send the Start bit
 */
void TWI_writeByte(uint8 data);

/*
 *Description :
 *    Reads the byte received with ACK bit sent to Continue receiving
 */
uint8 TWI_readByteWithACK(void);

/*
 *Description :
 *    Reads the byte received with NACK bit sent to Stop receiving
 */
uint8 TWI_readByteWithNACK(void);

/*
 *Description :
 *    Send the Start bit
 */
uint8 TWI_getStatus(void);


#endif /* TWI_H_ */
