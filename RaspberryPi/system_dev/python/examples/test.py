#!/usr/bin/python
# -*- coding:utf-8 -*-
import serial
import RPi.GPIO as GPIO
import time

import os
import sys
import logging

logging.basicConfig(level=logging.INFO)
libdir = os.path.join(os.path.dirname(os.path.dirname(os.path.realpath(__file__))), 'lib')
if os.path.exists(libdir):
    sys.path.append(libdir)

from waveshare_2_CH_RS485_HAT import config

TXDEN_1 = 27
TXDEN_2 = 22

ser1 = config.config(dev = "/dev/ttySC0")
ser2 = config.config(dev = "/dev/ttySC1")
data = 'waveshare_2_CH_RS485_HAT\r\n'
while(1):
    GPIO.output(TXDEN_1, GPIO.LOW) #send
    GPIO.output(TXDEN_2, GPIO.HIGH) #read
    ser1.Uart_SendString(data)
    data_t = ser2.Uart_ReceiveString(26)
    if(data_t == data):
        print ('Channel 1 send channel 2 received successfully')
        print (data_t)
        data_t = ''
    
    GPIO.output(TXDEN_1, GPIO.HIGH) #read
    GPIO.output(TXDEN_2, GPIO.LOW) #send
    ser2.Uart_SendString(data)
    data_t = ser1.Uart_ReceiveString(26)
    if(data_t == data):
        print ('Channel 2 send channel 1 received successfully')
        print (data_t)
        data_t = ''
    print('\r\n')
    time.sleep(1)

     
     
     
     