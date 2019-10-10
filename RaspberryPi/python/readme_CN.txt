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
本例程使用2-CH RS485 HAT模块进行了验证，你可以在工程的examples\中查看对应的测试例程;

2.管脚连接：
管脚连接你可以在\lib\waveshare_2_CH_RS485_HAT\config.py中查看，这里也再重述一次：
2-CH_RS485_HAT      =>    RPI(BCM)
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
    sudo apt-get update
    sudo apt-get install python-pip
    sudo apt-get install python-pil
    sudo pip install RPi.GPIO
    sudo apt-get install python-serial
    sudo pip install serial
    sudo pip install pyserial
或

    sudo apt-get update
    sudo apt-get install python3-pip
    sudo apt-get install python3-pil
    sudo pip3 install RPi.GPIO
    sudo apt-get install python3-serial
    sudo pip3 install serial
    sudo pip3 install pyserial
    
5.基本使用：
测试程序在examples\目录中查看
例程main.py为通道1接收字符串以'\n'结束，然后发送接收到的字符串。
例程test.py为通道1和通道2互发数据()。
python2
    sudo python main.py

python3
    sudo python3 main.py