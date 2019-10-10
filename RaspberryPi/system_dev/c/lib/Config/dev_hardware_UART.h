/*****************************************************************************
* | File        :   dev_hardware_UART.h
* | Author      :   Waveshare team
* | Function    :   Read and write /dev/UART,  hardware UART
* | Info        :
*----------------
* |	This version:   V1.0
* | Date        :   2019-06-26
* | Info        :   Basic version
*
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documnetation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to  whom the Software is
# furished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS OR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.
#
******************************************************************************/
#ifndef __DEV_HARDWARE_UART_
#define __DEV_HARDWARE_UART_

#include <stdint.h>


#define DEV_HARDWARE_UART_DEBUG 0
#if DEV_HARDWARE_UART_DEBUG
#define DEV_HARDWARE_UART_Debug(__info,...) printf("Debug: " __info,##__VA_ARGS__)
#else
#define DEV_HARDWARE_UART_Debug(__info,...)
#endif

/**
 * Define UART attribute
**/
typedef struct UARTStruct {
    //GPIO
    uint16_t TXD_PIN;
    uint16_t RXD_PIN;
    
    int fd; //
    uint16_t addr; //
} HARDWARE_UART;

void DEV_HARDWARE_UART_begin(char *UART_device);
void DEV_HARDWARE_UART_end(void);
void DEV_HARDWARE_UART_setBaudrate(uint32_t Baudrate);

uint8_t DEV_HARDWARE_UART_write(const char * buf, uint32_t len);
uint8_t DEV_HARDWARE_UART_read(char* buf, uint32_t len);
uint8_t DEV_HARDWARE_UART_writeByte(uint8_t buf);
uint8_t DEV_HARDWARE_UART_readByte(void);

int DEV_HARDWARE_UART_Set(int databits, int stopbits, int parity);
#endif