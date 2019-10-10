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

from waveshare_2_CH_RS485_HAT import RS485

RS485 = RS485.RS485()
RS485.RS485_CH2_begin(115200)
RS485.RS485_CH1_begin(115200)

print ('2-CH_RS485_HAT')

data = 'waveshare_2_CH_RS485_HAT\r\n'

try:
    while(1):
        RS485.RS485_CH2_Write(data)
        data_t = RS485.RS485_CH1_Read(26)
        if(data_t == data):
            print ('Channel 1 send channel 2 received successfully')
            print (data_t)
            data_t = ''
        
        RS485.RS485_CH1_Write(data)
        data_t = RS485.RS485_CH2_Read(26)
        if(data_t == data):
            print ('Channel 2 send channel 1 received successfully')
            print (data_t)
            data_t = ''
        print('\r\n')
        time.sleep(1)

except KeyboardInterrupt:
    logging.info("ctrl + c:")
    RS485.RS485_CH2_EnableSleep()
    RS485.RS485_CH1_EnableSleep()
    exit()
     
     
     