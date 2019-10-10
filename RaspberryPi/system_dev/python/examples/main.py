#!/usr/bin/python
# -*- coding:utf-8 -*-
import serial
import os
import sys
import logging

logging.basicConfig(level=logging.INFO)
libdir = os.path.join(os.path.dirname(os.path.dirname(os.path.realpath(__file__))), 'lib')
if os.path.exists(libdir):
    sys.path.append(libdir)
    
import RPi.GPIO as GPIO
import time
from waveshare_2_CH_RS485_HAT import config

TXDEN_1 = 27
TXDEN_2 = 22

ser = config.config(dev = "/dev/ttySC0")
data = ''
GPIO.output(TXDEN_1, GPIO.LOW) 
ser.Uart_SendString('Waveshare 2-CH RS485 HAT\r\n')
time.sleep(0.005)#Waiting to send
GPIO.output(TXDEN_1, GPIO.HIGH)

try:
    while(1):
        data_t = ser.Uart_ReceiveByte()
        data += str(data_t)
        if(data_t == '\n'):
            GPIO.output(TXDEN_1, GPIO.LOW)   
            print(data)
            ser.Uart_SendString(data)
            time.sleep(0.005)#Waiting to send
            data = ''
            GPIO.output(TXDEN_1, GPIO.HIGH)
            
except KeyboardInterrupt:    
    logging.info("ctrl + c:")
    exit()


     
     
     
     