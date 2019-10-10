#!/usr/bin/python
# -*- coding:utf-8 -*-

import RPi.GPIO as GPIO
import logging

import os
import sys
libdir = os.path.dirname(os.path.realpath(__file__))
if os.path.exists(libdir):
    sys.path.append(libdir)

import config

XTAL1       = 14745600 #14.7456M
PRESCALER   = 1

CMD_WRITE   = 0x00  
CMD_READ    = 0x80  

CHANNEL_2   = 0x02 
CHANNEL_1   = 0x00 

#General register
THR = 0x00 #Write is to send data(Internal with 64 byte buffer)
RHR = 0x00 #Reading is receiving data(Internal with 64 byte buffer)
IER = 0x01 # Mode enable 
FCR = 0x02 
LCR = 0x03 
MCR = 0x04 #[:7]1:clock/4 0:clock/1 
LSR = 0x05 #Status register
MSR = 0x06 
SPR = 0x07

TCR = 0x06 #These registers are accessible only when EFR[4] = 1  and MCR[2] = 1
TLR = 0x07

TXLVL       = 0x08
RXLVL       = 0x09
IODIR_752   = 0x0a
IOSTATE     = 0x0b
IOINTENA    = 0x0c
RESERVED    = 0x0d
IOCONTROL   = 0x0e
EFCR        = 0x0f

#Special register
DLL = 0x00
DLH = 0x01

#Enhanced register 

EFR     = 0x02
XON1    = 0x04
XON2    = 0x05
XOFF1   = 0x06
XOFF2   = 0x07

B300        = 300 
B600        = 600 
B1200       = 1200 
B2400       = 2400 
B4800       = 4800 
B9600       = 9600 
B14400      = 14400 
B19200      = 19200 
B38400      = 38400 
B56000      = 56000 #Deviation 2.8%
B57600      = 57600 
B115200     = 115200
B128000     = 128000#Deviation 2.8%
B230400     = 230400
B460800     = 460800
B921600     = 921600

def REG(_Value):
     return _Value<<3

com = config.config()

class SC16IS752(object):
    CHANNEL_2   = 0x02
    CHANNEL_1   = 0x00 
    IRQ_PIN     = com.IRQ_PIN
    def __init__(self,Channel):
        if(Channel != CHANNEL_1 and Channel != 1):
            Channel = CHANNEL_2
        self.Channel = Channel;
        self.WR_REG(CMD_WRITE|REG(LCR)|self.Channel, 0xbf) #Enter Enhanced register
        self.WR_REG(CMD_WRITE|REG(EFR)|self.Channel, 0x10) #Enable Enhanced 
        self.WR_REG(CMD_WRITE|REG(LCR)|self.Channel, 0x03) #
        # LCR = 0x03  None Parity, 1 Stop bity,  8 data bity

        self.WR_REG(CMD_WRITE|REG(FCR)|self.Channel, 0x06) #Clear the transmit and receive buffers
        # DEV_Delay_ms(1) #wait
        self.WR_REG(CMD_WRITE|REG(FCR)|self.Channel, 0xf1) #Enabling the FIFOs

        self.WR_REG(CMD_WRITE|REG(SPR)|self.Channel, 0x41) 
        #Set useless GPIO to output low level
        self.WR_REG(CMD_WRITE|REG(IODIR_752)|self.Channel, 0xff) 
        self.WR_REG(CMD_WRITE|REG(IOSTATE)|self.Channel, 0x00) 
        #Enable data reception interrupt
        self.WR_REG(CMD_WRITE|REG(IER)|self.Channel, 0x01)

    def WR_REG(self, Reg, Value):
        if(type(Value) != int):
            for i in range(0, len(Value)):
                Value[i] = ord(Value[i])
            Data_t = com.SPI_transmission_nByte([Reg]+ Value)
        else:
            Data_t = com.SPI_transmission_nByte([Reg,Value])
        Value = Data_t[1:len(Data_t)]
        return Value
    
    def EnableSleep(self):
        Reg_IER = (self.WR_REG(CMD_READ|REG(IER)|self.Channel, 0xff)[0]);
        self.WR_REG(CMD_WRITE|REG(IER)|self.Channel, Reg_IER|0x10);#nter sleep mode
        
    def DisableSleep(self):
        Reg_IER = self.WR_REG(CMD_READ|REG(IER)|self.Channel, 0xff)[0];
        self.WR_REG(CMD_WRITE|REG(IER)|self.Channel, Reg_IER&(~0x10));#nter sleep mode
    
    
    def Set_Baudrate(self, Baud):
        if(Baud == 921600 or Baud == 460800 or Baud == 230400 or Baud == 128000 or 
        Baud == 115200 or Baud == 57600 or Baud == 56000 or Baud == 38400 or Baud == 19200 or 
        Baud == 14400  or Baud == 9600  or Baud == 4800  or Baud == 2400  or Baud == 1200  or 
        Baud == 600    or Baud == 300 ):
            Baud = Baud
        else :
            logging.debug("The baud rate parameter is incorrect and has been modified to 115200")
            Baud = 115200
        
        Baud_t = int(XTAL1/PRESCALER/16/Baud) 
        Reg_IER = self.WR_REG(CMD_READ|REG(IER)|self.Channel, 0xff) 
        Reg_LCR = self.WR_REG(CMD_READ|REG(LCR)|self.Channel, 0xff) 
        
        self.WR_REG(CMD_WRITE|REG(IER)|self.Channel, 0x10) #nter sleep mode
        self.WR_REG(CMD_WRITE|REG(LCR)|self.Channel, 0x80) #Special register

        self.WR_REG(CMD_WRITE|REG(DLH)|self.Channel, int(Baud_t/256)) #Frequency divider H
        self.WR_REG(CMD_WRITE|REG(DLL)|self.Channel, int(Baud_t%256)) #Frequency divider L
        self.WR_REG(CMD_WRITE|REG(DLL)|self.Channel, int(Baud_t%256)) #Frequency divider L

        self.WR_REG(CMD_WRITE|REG(LCR)|self.Channel, int(Reg_LCR[0])) #Exit Special register
        self.WR_REG(CMD_WRITE|REG(IER)|self.Channel, int(Reg_IER[0])) #Exit sleep mode

    def UART_Write(self, buf):
        buf = list(buf)
        Reg = CMD_WRITE|REG(THR)|self.Channel 
        self.WR_REG(Reg, buf) 

    def UART_Read(self, Len):
        Reg = CMD_READ|REG(RHR)|self.Channel 
        i = 0 
        buf = ''
        while(1):
            if(GPIO.input(self.IRQ_PIN) ==  0):
                buf = buf + chr(self.WR_REG(Reg, 0xff)[0])
                i = i+1
                if(i >= Len):
                    break 
        return buf
    
    def UART_ReadByte(self):
        return self.UART_Read(1)[0]

    
    
    
    
    
    
    
    
    

