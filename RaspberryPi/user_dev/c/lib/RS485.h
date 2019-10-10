#ifndef _RS485_H_
#define _RS485_H_

#include "SC16IS752.h"

#define RS485_CH1_begin(_Baud)  SC16IS752_UART_begin(CHANNEL_1);\
                                SC16IS752_UART_SetBaudrate(CHANNEL_1, _Baud);
                                
#define RS485_CH2_begin(_Baud)  SC16IS752_UART_begin(CHANNEL_2);\
                                SC16IS752_UART_SetBaudrate(CHANNEL_2, _Baud);

#define RS485_CH1_ReadByte()    (DEV_Digital_Write(TXDEN_1, 1),SC16IS752_UART_ReadByte(CHANNEL_1))
#define RS485_CH2_ReadByte()    (DEV_Digital_Write(TXDEN_2, 1),SC16IS752_UART_ReadByte(CHANNEL_2))


#define RS485_CH1_WriteString(pData)    DEV_Digital_Write(TXDEN_1, 0),\
                                        DEV_Delay_ms(1);\
                                        SC16IS752_UART_WriteString(CHANNEL_1, pData);\
                                        DEV_Delay_ms(5);\
                                        DEV_Digital_Write(TXDEN_1, 1)
                                        
#define RS485_CH2_WriteString(pData)    DEV_Digital_Write(TXDEN_2, 0),\
                                        DEV_Delay_ms(1);\
                                        SC16IS752_UART_WriteString(CHANNEL_2, pData);\
                                        DEV_Delay_ms(5);\
                                        DEV_Digital_Write(TXDEN_1, 1)

#define RS485_CH1_Write(pData, Len)     DEV_Digital_Write(TXDEN_1, 0),\
                                        DEV_Delay_ms(1);\
                                        SC16IS752_UART_Write(CHANNEL_1, pData, Len);\
                                        DEV_Delay_ms(5);\
                                        DEV_Digital_Write(TXDEN_1, 1)
                                        
#define RS485_CH2_Write(pData, Len)     DEV_Digital_Write(TXDEN_2, 0),\
                                        DEV_Delay_ms(1);\
                                        SC16IS752_UART_Write(CHANNEL_2, pData, Len);\
                                        DEV_Delay_ms(5);\
                                        DEV_Digital_Write(TXDEN_1, 1)


#endif

