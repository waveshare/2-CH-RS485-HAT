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
    System_dev : Use the driver that comes with the Raspberry Pi system. You need to add 
                 the necessary description in /boot/config.txt. To load into the system 
                 kernel, then read the device through the serial port library, using SPI1,
                 Use system_dev to connect the module directly.

    user_dev   : Use SP1 read and write register mode to drive, use SPI0, use user_dev 
                 need to connect through DuPont line, Can not be used directly.

3.Need to add driver when using system_dev
    # In /boot/config.txt:
    sudo nano /boot/config.txt
    
    # Add the following, int_pin is set according to the actual welding mode(BCM coding):
    dtoverlay=sc16is752-spi1,int_pin=24

    # reboot device
    sudo reboot
    
    After reboot, the driver of SC16IS752 will be loaded into the system 
    kernel. At this time, you can run ls/dev to see that there will be more 
    devices as follows:
    ttySC0      ttySC1
    
