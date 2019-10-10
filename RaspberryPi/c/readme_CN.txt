/*****************************************************************************
* | File      	:   Readme_CN.txt
* | Author      :   Waveshare team
* | Function    :   Help with use
* | Info        :
*----------------
* |	This version:   V1.0
* | Date        :   2019-09-24
* | Info        :   在这里提供一个中文版本的使用文档，以便你的快速使用
******************************************************************************/
这个文件是帮助您使用本例程。
在这里简略的描述本工程的使用：

1.基本信息：
本例程均在Raspberry Pi 3B+上进行了验证;
本例程使用2-CH_RS485_HAT模块进行了验证，你可以在工程的examples\中查看对应的测试例程;

2.管脚连接：
管脚连接你可以在\lib\Config\DEV_Config.c中查看，这里也再重述一次：
2-CH_RS485_HAT      =>    Jetson Nano/RPI(BCM)
VCC                 ->    3.3V/5V
GND                 ->    GND
SCLK                ->    21
MISO                ->    24
MOSI                ->    20
CS                  ->    18
IRQ                 ->    24

3.添加驱动
    #在/boot/config.txt中:
    sudo nano /boot/config.txt
    
    #加入如下, int_pin根据实际焊接方式设置:
    dtoverlay=sc16is752-spi1,int_pin=24

    # 重启设备
    sudo reboot
    
    重启后，SC16IS752的驱动会加载到系统内核中此时可以运行ls /dev查看一下 将会多出如下设备
    ttySC0      ttySC1

4.安装库：
Raspberry Pi
    安装wiringPi
        1.	sudo apt-get install wiringpi
        2.	#对于树莓派4B可能需要进行升级：
        3.	cd /tmp
        4.	wget https://project-downloads.drogon.net/wiringpi-latest.deb
        5.	sudo dpkg -i wiringpi-latest.deb
        6.	gpio -v
        7.	# 运行gpio -v会出现2.52版本，如果没有出现说明安装出错

在Makefile文件中有
    # USELIB = USE_BCM2835_LIB
    # USELIB = USE_WIRINGPI_LIB
    USELIB = USE_DEV_LIB
Raspberry Pi 支持WIRINGPI和DEV_LIB

5.基本使用：
测试程序在examples\目录中查看
Run:
    make clean 
    make
    sudo ./main



