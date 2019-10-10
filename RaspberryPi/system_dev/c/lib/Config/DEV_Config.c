/*****************************************************************************
* | File      	:   DEV_Config.c
* | Author      :   Waveshare team
* | Function    :   Hardware underlying interface
* | Info        :
*----------------
* |	This version:   V2.0
* | Date        :   2019-07-08
* | Info        :   Basic version
*
******************************************************************************/
#include "DEV_Config.h"
#include <unistd.h>
#include <fcntl.h>

uint32_t fd,UART_fd;
char *UART_Device;

int TXDEN_1;
int TXDEN_2;
int IRQ_PIN;
/******************************************************************************
function:	Equipment Testing
parameter:
Info:   Only supports Jetson Nano and Raspberry Pi
******************************************************************************/
static int DEV_Equipment_Testing(void)
{
	int i;
	int fd;
	char value_str[20];
	fd = open("/etc/issue", O_RDONLY);
    printf("Current environment: ");
	while(1) {
		if (fd < 0) {
			return -1;
		}
		for(i=0;; i++) {
			if (read(fd, &value_str[i], 1) < 0) {
				return -1;
			}
			if(value_str[i] ==32) {
				printf("\r\n");
				break;
			}
			printf("%c",value_str[i]);
		}
		break;
	}

	if(i<5) {
		printf("Unrecognizable\r\n");
        return -1;
	} else {
		char RPI_System[10]   = {"Raspbian"};
		for(i=0; i<6; i++) {
			if(RPI_System[i] != value_str[i]) {
                #if USE_DEV_LIB    
                    return 'J';
                #endif
                return -1;
			}
		}
        return 'R';
	}
	return -1;
}


/******************************************************************************
function:	GPIO Function initialization and transfer
parameter:
Info:
******************************************************************************/
void DEV_GPIO_Mode(uint16_t Pin, uint16_t Mode)
{
    /*
        0:  INPT   
        1:  OUTP
    */
#ifdef USE_BCM2835_LIB  
    if(Mode == 0 || Mode == BCM2835_GPIO_FSEL_INPT){
        bcm2835_gpio_fsel(Pin, BCM2835_GPIO_FSEL_INPT);
    }else {
        bcm2835_gpio_fsel(Pin, BCM2835_GPIO_FSEL_OUTP);
    }
#elif USE_WIRINGPI_LIB
    if(Mode == 0 || Mode == INPUT){
        pinMode(Pin, INPUT);
        pullUpDnControl(Pin, PUD_UP);
    }else{ 
        pinMode(Pin, OUTPUT);
    }
#elif USE_DEV_LIB
    SYSFS_GPIO_Export(Pin);
    if(Mode == 0 || Mode == SYSFS_GPIO_IN){
        SYSFS_GPIO_Direction(Pin, SYSFS_GPIO_IN);
    }else{
        SYSFS_GPIO_Direction(Pin, SYSFS_GPIO_OUT);
    }
#endif   
}

void DEV_Digital_Write(uint16_t Pin, uint8_t Value)
{
#ifdef USE_BCM2835_LIB
    bcm2835_gpio_write(Pin, Value);
    
#elif USE_WIRINGPI_LIB
    digitalWrite(Pin, Value);
    
#elif USE_DEV_LIB
    SYSFS_GPIO_Write(Pin, Value);
    
#endif
}

uint8_t DEV_Digital_Read(uint16_t Pin)
{
    uint8_t Read_value = 0;
#ifdef USE_BCM2835_LIB
    Read_value = bcm2835_gpio_lev(Pin);
    
#elif USE_WIRINGPI_LIB
    Read_value = digitalRead(Pin);
    
#elif USE_DEV_LIB
    Read_value = SYSFS_GPIO_Read(Pin);
#endif
    return Read_value;
}


/**
 * delay x ms
**/
void DEV_Delay_ms(UDOUBLE xms)
{
#ifdef USE_BCM2835_LIB
    bcm2835_delay(xms);
#elif USE_WIRINGPI_LIB
    delay(xms);
#elif USE_DEV_LIB
    UDOUBLE i;
    for(i=0; i < xms; i++){
        usleep(1000);
    }
#endif
}


void GPIO_Config(void)
{
    int Equipment = DEV_Equipment_Testing();
    if(Equipment=='R'){
        /************************
        Raspberry Pi GPIO
        ***********************/
        TXDEN_1 = 27;
        TXDEN_2 = 22;
        IRQ_PIN = 25;
    }else if(Equipment=='J'){
        #if USE_DEV_LIB
        /************************
        Jetson Nano GPIO
        ***********************/
        TXDEN_1 = GPIO27;
        TXDEN_2 = GPIO22;
        IRQ_PIN = GPIO25;
        #endif
    }else{
        printf("Device read failed or unrecognized!!!\r\n");
        while(1);
    }
    
    DEV_GPIO_Mode(TXDEN_1, 1);
    DEV_GPIO_Mode(TXDEN_2, 1);
    DEV_GPIO_Mode(IRQ_PIN, 0);
}

/******************************************************************************
function:	SPI Function initialization and transfer
parameter:
Info:
******************************************************************************/
void DEV_SPI_Init()
{
#if DEV_SPI
    #ifdef USE_BCM2835_LIB
        printf("BCM2835 SPI Device\r\n");  
        bcm2835_spi_begin();                                         //Start spi interface, set spi pin for the reuse function
        bcm2835_spi_setBitOrder(BCM2835_SPI_BIT_ORDER_MSBFIRST);     //High first transmission
        bcm2835_spi_setDataMode(BCM2835_SPI_MODE0);                  //spi mode 0
        bcm2835_spi_setClockDivider(BCM2835_SPI_CLOCK_DIVIDER_128);  //Frequency
        bcm2835_spi_chipSelect(BCM2835_SPI_CS0);                     //set CE0
        bcm2835_spi_setChipSelectPolarity(BCM2835_SPI_CS0, LOW);     //enable cs0
        
    #elif USE_WIRINGPI_LIB
        printf("WIRINGPI SPI Device\r\n");       
        //wiringPiSPISetup(0,9000000);
        wiringPiSPISetupMode(0, 9000000, 0);
        
    #elif USE_DEV_LIB
        printf("DEV SPI Device\r\n"); 
        DEV_HARDWARE_SPI_begin("/dev/spidev0.0");
    #endif
#endif
}

void DEV_SPI_WriteByte(uint8_t Value)
{
#if DEV_SPI
    #ifdef USE_BCM2835_LIB
        bcm2835_spi_transfer(Value);
        
    #elif USE_WIRINGPI_LIB
        wiringPiSPIDataRW(0,&Value,1);
        
    #elif USE_DEV_LIB
        DEV_HARDWARE_SPI_TransferByte(Value);
    #endif
#endif
}

void DEV_SPI_Write_nByte(uint8_t *pData, uint32_t Len)
{
#if DEV_SPI
    #ifdef USE_BCM2835_LIB
        uint8_t rData[Len];
        bcm2835_spi_transfernb(pData,rData,Len);
        
    #elif USE_WIRINGPI_LIB
        wiringPiSPIDataRW(0, pData, Len);
        
    #elif USE_DEV_LIB
        DEV_HARDWARE_SPI_Transfer(pData, Len);
        
    #endif
#endif
}

void DEV_SPI_transmission_nByte(uint8_t *pData, uint32_t Len)
{
#if DEV_SPI
    #ifdef USE_BCM2835_LIB
        // bcm2835_spi_transfernb(pData, Len);
        
    #elif USE_WIRINGPI_LIB
        // wiringPiSPIDataRW(0,pData,Len);
        
    #elif USE_DEV_LIB
        DEV_HARDWARE_SPI_Transfer(pData, Len);
        
    #endif
#endif
}
/******************************************************************************
function:	I2C Function initialization and transfer
parameter:
Info:
******************************************************************************/
void DEV_I2C_Init(uint8_t Add)
{
#if DEV_I2C
    #ifdef USE_BCM2835_LIB
        printf("BCM2835 I2C Device\r\n");  
        bcm2835_i2c_begin();
        bcm2835_i2c_setSlaveAddress(Add);
        
    #elif USE_WIRINGPI_LIB
        printf("WIRINGPI I2C Device\r\n");       
        fd = wiringPiI2CSetup(Add);
        
    #elif USE_DEV_LIB
        printf("DEV I2C Device\r\n"); 
        DEV_HARDWARE_I2C_begin("/dev/i2c-1");
        DEV_HARDWARE_I2C_setSlaveAddress(Add);
    #endif
#endif
}

void I2C_Write_Byte(uint8_t Cmd, uint8_t value)
{
	int ref;
#if DEV_I2C
    #ifdef USE_BCM2835_LIB
        char wbuf[2]={Cmd, value};
        bcm2835_i2c_write(wbuf, 2);
    #elif USE_WIRINGPI_LIB
        //wiringPiI2CWrite(fd,Cmd);
        ref = wiringPiI2CWriteReg8(fd, (int)Cmd, (int)value);
        while(ref != 0) {
            ref = wiringPiI2CWriteReg8 (fd, (int)Cmd, (int)value);
            if(ref == 0)
                break;
        }
    #elif USE_DEV_LIB
        char wbuf[2]={Cmd, value};
        DEV_HARDWARE_I2C_write(wbuf, 2);

    #endif
#endif
}

int I2C_Read_Byte(uint8_t Cmd)
{
	int ref;
#if DEV_I2C
    #ifdef USE_BCM2835_LIB
        char rbuf[2]={0};
        bcm2835_i2c_read_register_rs(&Cmd, rbuf, 1);
        ref = rbuf[0];
        
    #elif USE_WIRINGPI_LIB
        ref = wiringPiI2CReadReg8 (fd, (int)Cmd);
        
    #elif USE_DEV_LIB
        char rbuf[2]={0};
        DEV_HARDWARE_I2C_read(Cmd, rbuf, 1);
        ref = rbuf[0];
    #endif
#endif
    return ref;
}

int I2C_Read_Word(uint8_t Cmd)
{
	int ref;
#if DEV_I2C
    #ifdef USE_BCM2835_LIB
        char rbuf[2] = {0};
        bcm2835_i2c_read_register_rs(&Cmd, rbuf, 2);
        ref = rbuf[1]<<8 | rbuf[0];
        
    #elif USE_WIRINGPI_LIB
        ref = wiringPiI2CReadReg16 (fd, (int)Cmd);
        
    #elif USE_DEV_LIB
        char rbuf[2] = {0};
        DEV_HARDWARE_I2C_read(Cmd, rbuf, 2);
        ref = rbuf[1]<<8 | rbuf[0];
    #endif
#endif
    return ref;
}
/******************************************************************************
function:	SPI Function initialization and transfer
parameter:
Info:
******************************************************************************/
void DEV_UART_Init(char *Device)
{
    UART_Device = Device;
#if DEV_UART
    #ifdef USE_BCM2835_LIB
        printf("BCM2835 Not Supported UART!!! \r\n");
        
    #elif USE_WIRINGPI_LIB
        if((UART_fd = serialOpen(UART_Device,115200)) < 0){
            printf("set uart failed  !!! \r\n");
            return ;
        }else {
            printf("set uart success  !!! \r\n");
        }
    #elif USE_DEV_LIB
        DEV_HARDWARE_UART_begin(UART_Device);
        
    #endif
    // UART_Set_Baudrate(115200);
#endif
}

void UART_Write_Byte(uint8_t data)
{
#if DEV_UART
    #ifdef USE_BCM2835_LIB
        printf("BCM2835 Not Supported UART!!! \r\n");
        
    #elif USE_WIRINGPI_LIB
        serialPutchar(UART_fd, data);
        
    #elif USE_DEV_LIB
        DEV_HARDWARE_UART_writeByte(data);
    #endif
#endif
}

int UART_Read_Byte(void)
{
    int ref;
#if DEV_UART
    #ifdef USE_BCM2835_LIB
        printf("BCM2835 Not Supported UART!!! \r\n");
        
    #elif USE_WIRINGPI_LIB
        ref = serialGetchar(UART_fd);
        
    #elif USE_DEV_LIB
        ref = DEV_HARDWARE_UART_readByte();
    #endif
#endif
    return ref;
}

void UART_Set_Baudrate(uint32_t Baudrate)
{
#if DEV_UART
    #ifdef USE_BCM2835_LIB
        printf("BCM2835 Not Supported UART!!! \r\n");
        
    #elif USE_WIRINGPI_LIB
        serialClose(UART_fd);
        if((UART_fd = serialOpen(UART_Device, Baudrate)) < 0){
            printf("set uart failed	!!! \r\n");
        }else{
            printf("set uart success  !!! \r\n");
        }
        
    #elif USE_DEV_LIB
        DEV_HARDWARE_UART_setBaudrate(Baudrate);
    #endif
#endif
 
}

int UART_Write_nByte(uint8_t *pData, uint32_t Lan)
{
#if DEV_UART
    #ifdef USE_BCM2835_LIB
        printf("BCM2835 Not Supported UART!!! \r\n");
        
    #elif USE_WIRINGPI_LIB
        for(uint32_t i=0; i<Lan; i++){
            serialPutchar(UART_fd, *(pData+i));
        }
        
    #elif USE_DEV_LIB
        DEV_HARDWARE_UART_write(pData, Lan);
    #endif
#endif 
}
/******************************************************************************
function:	Module Initialize, the library and initialize the pins, SPI protocol
parameter:
Info:
******************************************************************************/
uint8_t DEV_ModuleInit(void)
{
 #ifdef USE_BCM2835_LIB
    if(!bcm2835_init()) {
        printf("bcm2835 init failed  !!! \r\n");
        return 1;
    } else {
        printf("bcm2835 init success !!! \r\n");
    }

#elif USE_WIRINGPI_LIB  
    //if(wiringPiSetup() < 0)//use wiringpi Pin number table  
    if(wiringPiSetupGpio() < 0) { //use BCM2835 Pin number table
        printf("set wiringPi lib failed	!!! \r\n");
        return 1;
    } else {
        printf("set wiringPi lib success  !!! \r\n");
    }

#elif USE_DEV_LIB
    printf("USE_DEV_LIB \r\n");
#endif
    GPIO_Config();
    

    
    return 0;
}

/******************************************************************************
function:	Module exits, closes SPI and BCM2835 library
parameter:
Info:
******************************************************************************/
void DEV_ModuleExit(void)
{
#ifdef USE_BCM2835_LIB
    #if DEV_I2C
        bcm2835_i2c_end();
    #endif
    #if DEV_SPI
        bcm2835_spi_end();
    #endif
    bcm2835_close();
    
#elif USE_WIRINGPI_LIB
    #if DEV_UART
        serialFlush(UART_fd);
        serialClose(UART_fd);
    #endif
    
#elif USE_DEV_LIB
    #if DEV_I2C
        DEV_HARDWARE_I2C_end();
    #endif
    #if DEV_SPI
        DEV_HARDWARE_SPI_end();
    #endif
#endif
}

