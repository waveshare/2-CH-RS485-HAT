#include <stdio.h>      //printf()
#include <stdlib.h>     //exit()
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include "DEV_Config.h"

void  Handler(int signo)
{
    //System Exit
    printf("\r\nHandler:Program stop\r\n"); 

    DEV_ModuleExit();

    exit(0);
}

int main(int argc, char **argv)
{
    // Exception handling:ctrl + c
    signal(SIGINT, Handler);
    
	if (DEV_ModuleInit()==1)return 1;
    
    DEV_UART_Init("/dev/ttySC0");
    UART_Set_Baudrate(115200);
    
    UBYTE pData[100]={0};
    int i=0;
    
    DEV_Digital_Write(TXDEN_1, 0);
    UART_Write_nByte("Waveshare 2-CH RS485 HAT\r\n", 26);
    DEV_Delay_ms(5);//Waiting to send
    DEV_Digital_Write(TXDEN_1, 1);
    while(1){
        pData[i] = UART_Read_Byte();
        if(pData[i] == '\n'){
            DEV_Digital_Write(TXDEN_1, 0);
            for(int j=0; j<i+1; j++)
                printf("%c",pData[j]);
            UART_Write_nByte(pData, i+1);
            DEV_Delay_ms(5);//Waiting to send
            DEV_Digital_Write(TXDEN_1, 1);
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
