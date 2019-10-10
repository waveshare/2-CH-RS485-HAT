/*****************************************************************************
* | File      	:   Readme_EN.txt
* | Author      :   Waveshare team
* | Function    :   Help with use
* | Info        :
*----------------
* |	This version:   V1.0
* | Date        :   2019-09-24
* | Info        :   Here is an English version of the documentation for your quick use.
******************************************************************************/
This file is to help you use this Demo.
A brief description of the use of this project is here:

1. Basic information:
This Demo has been verified on the Raspberry Pi 3B+ and Jetson Nano;
This Demo has been verified using the 2-CH_RS485_HAT. 
You can view the corresponding test routines in the examples\ of the project;

2. Pin connection:
Pin connections can be viewed in lib Config DEV_Config.C. Here again:
2-CH_RS485_HAT      =>    RPI(BCM)
VCC                 ->    3.3V/5V
GND                 ->    GND
SCLK                ->    SCLK (11)
MISO                ->    MISO (9)
MOSI                ->    MOSI (10)
CS                  ->    8
IRQ                 ->    25
EN1                 ->    27
EN2                 ->    22


3.Installation library
Raspberry Pi
    Installation BCM2835
        1.	wget http://www.airspayce.com/mikem/bcm2835/bcm2835-1.60.tar.gz
        2.	tar zxvf bcm2835-1.60.tar.gz 
        3.	cd bcm2835-1.60/
        4.	sudo ./configure
        5.	sudo make
        6.	sudo make check
        7.	sudo make install
        8.	# More can refer to the official website：http://www.airspayce.com/mikem/bcm2835/
        
    Installation wiringPi
        1.	sudo apt-get install wiringpi
        2.	#For Raspberry Pi 4B you may need to upgrade:
        3.	cd /tmp
        4.	wget https://project-downloads.drogon.net/wiringpi-latest.deb
        5.	sudo dpkg -i wiringpi-latest.deb
        6.	gpio -v
        7.	# Running gpio -v will appear version 2.52, if there is no error, the installation is wrong.
    
In the Makefile
    # USELIB = USE_BCM2835_LIB
    # USELIB = USE_WIRINGPI_LIB
    USELIB = USE_DEV_LIB
Raspberry Pi stand by WIRINGPI and DEV_LIB

4. Basic use:
Test the program in the examples\ directory:
    make clean 
    make
    sudo ./main