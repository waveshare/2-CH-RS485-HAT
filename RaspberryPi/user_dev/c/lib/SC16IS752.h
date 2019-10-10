/*****************************************************************************
* | File      	:   SC16IS752.h
* | Author      :   Waveshare team
* | Function    :   SC16IS752 drive
* | Info        :
*----------------
* |	This version:   V1.0
* | Date        :   2019-09-26
* | Info        :   Basic version
*
******************************************************************************/
#ifndef _SC16IS752_H_
#define _SC16IS752_H_

#include "DEV_Config.h"

#define XTAL1       14745600//14.7456M
#define PRESCALER   1

#define REG(_Value) _Value<<3

#define CMD_WRITE   0x00  
#define CMD_READ    0x80  

#define CHANNEL_2   0x02 
#define CHANNEL_1   0x00 

//General register
#define THR 0x00 //Write is to send data(Internal with 64 byte buffer)
#define RHR 0x00 //Reading is receiving data(Internal with 64 byte buffer)
#define IER 0x01 // Mode enable 
#define FCR 0x02 
#define LCR 0x03 
#define MCR 0x04 //[:7]1:clock/4 0:clock/1 
#define LSR 0x05 //Status register
#define MSR 0x06 
#define SPR 0x07

#define TCR 0x06 //These registers are accessible only when EFR[4] = 1, and MCR[2] = 1
#define TLR 0x07

#define TXLVL       0x08
#define RXLVL       0x09
#define IODIR_752   0x0a
#define IOSTATE     0x0b
#define IOINTENA    0x0c
#define RESERVED    0x0d
#define IOCONTROL   0x0e
#define EFCR        0x0f

//Special register
#define DLL 0x00
#define DLH 0x01

//Enhanced register 

#define EFR     0x02
#define XON1    0x04
#define XON2    0x05
#define XOFF1   0x06
#define XOFF2   0x07

typedef enum
{
    B300        = 300,
    B600        = 600,
    B1200       = 1200,
    B2400       = 2400,
    B4800       = 4800,
    B9600       = 9600,
    B14400      = 14400,
    B19200      = 19200,
    B38400      = 38400,
    B56000      = 56000,//Deviation 2.8%
    B57600      = 57600,
    B115200     = 115200,
    B128000     = 128000,//Deviation 2.8%
    B230400     = 230400,
    B460800     = 460800,
    B921600     = 921600,
}BaudRate;

void SC16IS752_UART_begin(UBYTE Channel);
void SC16IS752_UART_SetBaudrate(UBYTE Channel, UDOUBLE Baud);

void SC16IS752_UART_WriteByte(UBYTE Channel, UBYTE buf);
UBYTE SC16IS752_UART_ReadByte(UBYTE Channel);
void SC16IS752_UART_Write(UBYTE Channel, const UBYTE * buf, UDOUBLE Len);
void SC16IS752_UART_Read(UBYTE Channel, UBYTE * buf, UDOUBLE Len);
void SC16IS752_UART_WriteString(UBYTE Channel, const UBYTE * buf);

void SC16IS752_UART_EnableSleep(UBYTE Channel);
void SC16IS752_UART_DisableSleep(UBYTE Channel);

#endif
