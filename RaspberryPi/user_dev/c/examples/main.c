#include <stdio.h>      //printf()
#include <stdlib.h>     //exit()
#include <signal.h>
#include <stdio.h>
#include <string.h>

#include "DEV_Config.h"
#include "SC16IS752.h"
#include "RS485.h"

void  Handler(int signo)
{
    //System Exit
    printf("\r\nHandler:Program stop\r\n"); 
    SC16IS752_UART_EnableSleep(1);
    SC16IS752_UART_EnableSleep(2);
    
    DEV_ModuleExit();
    exit(0);
}

int main(int argc, char **argv)
{
    // Exception handling:ctrl + c
    signal(SIGINT, Handler);
    int i=0;
    UBYTE pData[100];
    
	if (DEV_ModuleInit()==1)return 1;

    DEV_SPI_Init();
    
    RS485_CH1_begin(115200)
    DEV_Delay_ms(10);
    RS485_CH1_WriteString( "Waveshare 2-CH RS485 HAT\r\n");
    while(1){
        pData[i] = RS485_CH1_ReadByte();
        if(pData[i] == '\n'){
            for(int j=0; j<i+1; j++)
                printf("%c",pData[j]);
            RS485_CH1_Write(pData, i+1);
            i=0;
        }else if(i>100){
            i=0;
        }else{
            i++;
        }
    }
	DEV_ModuleExit();
    return 0; 
}

    
    
    
