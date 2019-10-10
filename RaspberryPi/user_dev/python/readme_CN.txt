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
SCLK                ->    SCLK (11)
MISO                ->    MISO (9)
MOSI                ->    MOSI (10)
CS                  ->    8
IRQ                 ->    25
EN1                 ->    27
EN2                 ->    22


3.安装库：
    sudo apt-get update
    sudo apt-get install python-pip
    sudo apt-get install python-pil
    sudo pip install RPi.GPIO
    sudo pip install spidev

或

    sudo apt-get update
    sudo apt-get install python3-pip
    sudo apt-get install python3-pil
    sudo pip3 install RPi.GPIO
    sudo pip install spidev

    
4.基本使用：
测试程序在examples\目录中查看
例程main.py为通道1接收字符串以'\n'结束，然后发送接收到的字符串。
例程test.py为通道1和通道2互发数据()。
python2
    sudo python main.py

python3
    sudo python3 main.py