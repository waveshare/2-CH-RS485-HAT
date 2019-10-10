#!/usr/bin/python
# -*- coding:utf-8 -*-
import serial
import time
import os
import sys
import logging

logging.basicConfig(level=logging.INFO)
libdir = os.path.join(os.path.dirname(os.path.dirname(os.path.realpath(__file__))), 'lib')
if os.path.exists(libdir):
    sys.path.append(libdir)

import RPi.GPIO as GPIO

from waveshare_2_CH_RS485_HAT import RS485

print ('2-CH_RS485_HAT')

RS485 = RS485.RS485()
RS485.RS485_CH1_begin(115200)
time.sleep(0.01)
RS485.RS485_CH1_Write("Waveshare 2-CH RS485 HAT\r\n")
data = ''

try:
    while(1):
        data_t = RS485.RS485_CH1_ReadByte()
        data += str(data_t)
        if(data_t == '\n'):
            print(data)
            RS485.RS485_CH1_Write(data)
            data = ''

except KeyboardInterrupt:
    logging.info("ctrl + c:")
    RS485.RS485_CH1_EnableSleep()
    exit()


     
     
     
     