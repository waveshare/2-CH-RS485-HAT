#!/usr/bin/python
# -*- coding:utf-8 -*-

import RPi.GPIO as GPIO
import logging
import spidev as SPI


class config(object):
    TXDEN_1 = 27
    TXDEN_2 = 22
    IRQ_PIN = 25
    def __init__(self):
        GPIO.setmode(GPIO.BCM)
        GPIO.setwarnings(False)
        GPIO.setmode(GPIO.BCM)
        GPIO.setup(self.TXDEN_1, GPIO.OUT)
        GPIO.setup(self.TXDEN_2, GPIO.OUT)
        GPIO.setup(self.IRQ_PIN, GPIO.IN)

        GPIO.output(self.TXDEN_1, GPIO.LOW)
        GPIO.output(self.TXDEN_2, GPIO.LOW)
        
        self._spi = SPI.SpiDev(0, 0)
        self._spi.mode = 0b00
        self._spi.max_speed_hz = 1000000

    def SPI_transmission_nByte(self, value):
        return self._spi.xfer2(value)


