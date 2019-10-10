/*****************************************************************************
* | File      	:   Readme_CN.txt
* | Author      :   Waveshare team
* | Function    :   Help with use
* | Info        :
*----------------
* |	This version:   V1.0
* | Date        :   2019-09-24
* | Info        :   在这里提供一个中文版本的使用文档, 以便你的快速使用
******************************************************************************/
这个文件是帮助您使用本例程。
在这里简略的描述本工程的使用：

1.基本信息：
本例程均在Raspberry Pi 3B+上进行了验证;
本例程使用2-CH_RS485_HAT模块进行了验证;
本模块提供两种驱动方式, 每种方式方便提供C和python两种例程。
    system_dev : 使用树莓派系统自带的驱动, 需要在/boot/config.txt添加必要的描述, 
                 来加载到系统内核中使用, 再通过串口库读取设备, 使用SPI1, 
                 使用system_dev直接将模块连接即可.
                
    user_dev   : 使用SP1读写寄存器方式驱动, 使用SPI0, 使用user_dev需要通过杜邦线连接
                 不能直接使用.


2.使用system_dev需要添加驱动
    #在/boot/config.txt中:
    sudo nano /boot/config.txt
    
    #加入如下, int_pin根据实际焊接方式设置(BCM编码):
    dtoverlay=sc16is752-spi1,int_pin=24

    # 重启设备
    sudo reboot
    
    重启后, SC16IS752的驱动会加载到系统内核中此时可以运行ls /dev查看一下 将会多出如下设备
    ttySC0      ttySC1


