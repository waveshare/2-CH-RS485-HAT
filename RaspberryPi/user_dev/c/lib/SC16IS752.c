/*****************************************************************************
* | File      	:   SC16IS752.c
* | Author      :   Waveshare team
* | Function    :   SC16IS752 drive
* | Info        :
*----------------
* |	This version:   V1.0
* | Date        :   2019-09-26
* | Info        :   Basic version
*
******************************************************************************/
#include "SC16IS752.h"
#include <string.h>


/******************************************************************************
function:	Spi read and write SC16IS752
parameter:
        Reg: Register address
               [7]: 1:read   0:write
             [6:3]: Reg
             [2:1]: Channel
               [0]: Not
        Value: Write to the value of the register
       return: Read the value of the register
Info:
    If it is a write value (register bit 7 = 0), the return value is 0xff
    If it is a read value (register bit 7=1), the value of the parameter is 0xff.
******************************************************************************/
static UBYTE SC16IS752_WR_REG(UBYTE Reg, UBYTE Value)
{
    UBYTE Data_t[2]={Reg,Value};
    DEV_SPI_transmission_nByte(Data_t, 2);
    Value = Data_t[1];
    return Value;
}

/******************************************************************************
function:	SC16IS752 enters sleep mode
parameter:
      Channel: CHANNEL_1 or  CHANNEL_2
         Baud: enum BaudRate
Info:
******************************************************************************/
void SC16IS752_UART_EnableSleep(UBYTE Channel)
{
    UBYTE Reg_IER;
    if(Channel != CHANNEL_1 && Channel != 1){
         Channel = CHANNEL_2;
    }
    Reg_IER = SC16IS752_WR_REG(CMD_READ|REG(IER)|Channel, 0xff);
    SC16IS752_WR_REG(CMD_WRITE|REG(IER)|Channel, Reg_IER|0x10);//nter sleep mode
}

/******************************************************************************
function:	SC16IS752 exits sleep mode
parameter:
      Channel: CHANNEL_1 or  CHANNEL_2
         Baud: enum BaudRate
Info:
******************************************************************************/
void SC16IS752_UART_DisableSleep(UBYTE Channel)
{
    UBYTE Reg_IER;
    if(Channel != CHANNEL_1 && Channel != 1){
         Channel = CHANNEL_2;
    }
    Reg_IER = SC16IS752_WR_REG(CMD_READ|REG(IER)|Channel, 0xff);
    SC16IS752_WR_REG(CMD_WRITE|REG(IER)|Channel, Reg_IER&(~0x10));
}

/******************************************************************************
function:	Set the baud rate of the SC16IS752 serial port output
parameter:
      Channel: CHANNEL_1 or  CHANNEL_2
         Baud: enum BaudRate
Info:
******************************************************************************/
void SC16IS752_UART_SetBaudrate(UBYTE Channel, UDOUBLE Baud)
{
    if(Channel != CHANNEL_1 && Channel != 1){
         Channel = CHANNEL_2;
    }
     switch(Baud)        //Set the number of data bits
    {

        case 921600:    Baud = 921600;  break;
        case 460800:    Baud = 460800;  break;
        case 230400:    Baud = 230400;  break;
        case 128000:    Baud = 128000;  break;
        case 115200:    Baud = 115200;  break;
        case 57600:     Baud = 57600;   break;
        case 56000:     Baud = 56000;   break;
        case 38400:     Baud = 38400;   break;
        case 19200:     Baud = 19200;   break;
        case 14400:     Baud = 14400;   break;
        case 9600:      Baud = 9600;    break;
        case 4800:      Baud = 4800;    break;
        case 2400:      Baud = 2400;    break;
        case 1200:      Baud = 1200;    break;
        case 600:       Baud = 600;     break;
        case 300:       Baud = 300;     break;
        default:        Baud = 115200;  break;

    }

    UDOUBLE Baud_t = XTAL1/PRESCALER/16/Baud;
    UBYTE Reg_IER,Reg_LCR;
    // printf("Baud_t = %d\r\n",Baud_t);
    // printf("Deviation = %f\r\n",(XTAL1*1.0)/PRESCALER/16/Baud_t*1.0-Baud*1.0);
    
    Reg_IER = SC16IS752_WR_REG(CMD_READ|REG(IER)|Channel, 0xff);
    Reg_LCR = SC16IS752_WR_REG(CMD_READ|REG(LCR)|Channel, 0xff);
    
    SC16IS752_WR_REG(CMD_WRITE|REG(IER)|Channel, 0x10);//nter sleep mode
    SC16IS752_WR_REG(CMD_WRITE|REG(LCR)|Channel, 0x80);//Special register
    
    SC16IS752_WR_REG(CMD_WRITE|REG(DLH)|Channel, Baud_t/256);//Frequency divider H
    SC16IS752_WR_REG(CMD_WRITE|REG(DLL)|Channel, Baud_t%256);//Frequency divider L
     
    SC16IS752_WR_REG(CMD_WRITE|REG(LCR)|Channel, Reg_LCR);//Exit Special register
    SC16IS752_WR_REG(CMD_WRITE|REG(IER)|Channel, Reg_IER);//Exit sleep mode
    
}

/******************************************************************************
function:	Set the SC16IS752 serial port initialization
parameter:
      Channel: CHANNEL_1 or  CHANNEL_2
Info:
******************************************************************************/
void SC16IS752_UART_begin(UBYTE Channel)
{
    if(Channel != CHANNEL_1 && Channel != 1){
         Channel = CHANNEL_2;
    }
    SC16IS752_WR_REG(CMD_WRITE|REG(LCR)|Channel, 0xbf);//Enter Enhanced register
    SC16IS752_WR_REG(CMD_WRITE|REG(EFR)|Channel, 0x10);//Enable Enhanced 
    SC16IS752_WR_REG(CMD_WRITE|REG(LCR)|Channel, 0x03);//
    // LCR = 0x03  None Parity, 1 Stop bity,  8 data bity
    
    SC16IS752_WR_REG(CMD_WRITE|REG(FCR)|Channel, 0x06);//Clear the transmit and receive buffers
    // DEV_Delay_ms(1);//wait
    SC16IS752_WR_REG(CMD_WRITE|REG(FCR)|Channel, 0xf1);//Enabling the FIFOs
    
    SC16IS752_WR_REG(CMD_WRITE|REG(SPR)|Channel, 0x41);
    //Set useless GPIO to output low level
    SC16IS752_WR_REG(CMD_WRITE|REG(IODIR_752)|Channel, 0xff);
    SC16IS752_WR_REG(CMD_WRITE|REG(IOSTATE)|Channel, 0x00);
    //Enable data reception interrupt
    SC16IS752_WR_REG(CMD_WRITE|REG(IER)|Channel, 0x01);
}

/******************************************************************************
function:	The SC16IS752 serial port sends the specified length data.
parameter:
      Channel: CHANNEL_1 or  CHANNEL_2
          buf: Send data buffer
          Len: Length of transmission
Info:
******************************************************************************/
void SC16IS752_UART_Write(UBYTE Channel, const UBYTE * buf, UDOUBLE Len)
{
    if(Channel != CHANNEL_1 && Channel != 1){
         Channel = CHANNEL_2;
    }
    UBYTE Reg = CMD_WRITE|REG(THR)|Channel;
    UBYTE Data_t[Len+1];
    Data_t[0] =  Reg;
    memcpy(Data_t+1, buf, Len);//wData -> Data_t
    DEV_SPI_transmission_nByte(Data_t, Len+1);
}

/******************************************************************************
function:	SC16IS752 serial port sends a string
parameter:
      Channel: CHANNEL_1 or  CHANNEL_2
          buf: Sent string(Must end with '\0')
Info:
******************************************************************************/
void SC16IS752_UART_WriteString(UBYTE Channel, const UBYTE * buf)
{
    if(Channel != CHANNEL_1 && Channel != 1){
         Channel = CHANNEL_2;
    }
    UBYTE Reg = CMD_WRITE|REG(THR)|Channel;
    UBYTE Data_t[strlen(buf)+1];
    Data_t[0] =  Reg;
    memcpy(Data_t+1, buf, strlen(buf));//wData -> Data_t
    DEV_SPI_transmission_nByte(Data_t, strlen(buf)+1);
}

/******************************************************************************
function:	The SC16IS752 serial port receives the specified length data.
parameter:
      Channel: CHANNEL_1 or  CHANNEL_2
          buf: receive data buffer
          Len: Length of transmission
Info:
******************************************************************************/
void SC16IS752_UART_Read(UBYTE Channel, UBYTE * buf, UDOUBLE Len)
{   
    if(Channel != CHANNEL_1 && Channel != 1){
         Channel = CHANNEL_2;
    }
    UBYTE Reg = CMD_READ|REG(RHR)|Channel;
    UDOUBLE i=0;
    while(1){
        if(DEV_Digital_Read(IRQ_PIN) ==  0){
            buf[i] = SC16IS752_WR_REG(Reg, 0xff);
            i++;
            if(i >= Len)
                break;
        }
    }
}

/******************************************************************************
function:	SC16IS752 serial port sends one byte of data
parameter:
      Channel: CHANNEL_1 or  CHANNEL_2
          buf: Sent data
Info:
******************************************************************************/
void SC16IS752_UART_WriteByte(UBYTE Channel, UBYTE buf)
{
    if(Channel != CHANNEL_1 && Channel != 1){
         Channel = CHANNEL_2;
    }
    UBYTE Reg = CMD_WRITE|REG(THR)|Channel;
    SC16IS752_WR_REG(Reg, buf);
}

/******************************************************************************
function:	SC16IS752 serial port reads one byte of data
parameter:
      Channel: CHANNEL_1 or  CHANNEL_2
       return: reads data
Info:
******************************************************************************/
UBYTE SC16IS752_UART_ReadByte(UBYTE Channel)
{
    if(Channel != CHANNEL_1 && Channel != 1){
         Channel = CHANNEL_2;
    }
    UBYTE Reg = CMD_READ|REG(RHR)|Channel;
    while(DEV_Digital_Read(IRQ_PIN) ==  1);
    // while(SC16IS752_WR_REG(CMD_READ|REG(RXLVL)|Channel,0xff)==0);
    return SC16IS752_WR_REG(Reg, 0xff);
}
