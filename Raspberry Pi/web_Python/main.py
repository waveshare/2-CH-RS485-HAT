#Version 2.0
#Modified: 
#1. No need to manually modify the directory
#2. The servo button click is changed to continuous
#3. IP does not need to modify automatic acquisition



# -*- coding: UTF-8 -*- 
import threading
import SocketServer 
import RPi.GPIO as GPIO
from PCA9685 import PCA9685
from SocketServer import StreamRequestHandler as SRH  
from time import ctime  
import time 

import signal

import threading
import thread
import os
import sys

import socket

Road = os.getcwd()
Road = Road + "/mjpg"
print Road


os.chdir(Road)
from mjpg import camera

pwm = PCA9685()
pwm.setPWMFreq(50)
pwm.setRotationAngle(0, 20)
pwm.setRotationAngle(1, 20)


def get_host_ip():#get IP 
    try:
        s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        s.connect(('8.8.8.8', 80))
        ip = s.getsockname()[0]
    finally:
        s.close()
    return ip

print ("IP = "),
print get_host_ip()
print ("\r\n")

host = get_host_ip() 
port = 8000
addr = (host,port) 


VPulse = 20 
HPulse = 20
HStep = 0
VStep = 0

   
class Servers(SRH): 
    def handle(self): 
        global HStep,VStep,VPulse,HPulse
        print 'got connection from ',self.client_address  
        self.wfile.write('connection %s:%s at %s succeed!' % (host,port,ctime()))  
        VPulse = 0 
        HPulse = 0
        HStep = 0
        VStep = 0
        while True:  
            
            data = self.request.recv(1024)  
            if not data:   
                break  
            if data == "Stop":
                HStep = 0
                VStep = 0
                #print("Stop")
            elif data == "Up":
                VStep = -3
                print("Up")
            elif data == "Down":
                VStep = 3
                print("Down")
            elif data == "Left":
                HStep = 3
                print("Left")
            elif data == "Right":
                HStep = -3
                print("Right")
            else:
                value = 0
                try:
                    value = int(data)
                    if(value >= 0 and value <= 100):
                        print(value)
                except:
                    pass
            self.request.send(data) 
            
def f():
    pwm.exit_PCA9685()
    GPIO.cleanup()
    print "\nProgram end"
    sys.exit()
    
    
def timerfunc():  
    global t        #Notice: use global variable!
    t = threading.Timer(0.02, timerfunc)
    t.start()

try:
    signal.signal(signal.SIGINT, exit)
    signal.signal(signal.SIGTERM, f)
    t = threading.Timer(0.02, timerfunc)
    t.setDaemon(True)
    t.start()

    print 'server is running....'  
    server = SocketServer.ThreadingTCPServer(addr,Servers)
    time.sleep(0.1) 
    t1 = threading.Thread(target = server.serve_forever) 
    t2 = threading.Thread(target = camera.camera)
    t1.setDaemon(True)
    t2.setDaemon(True)
    t2.start()
    t1.start()

    while True:
        if(HStep != 0):
            HPulse += HStep
            if(HPulse >= 180): 
                HPulse = 180
                print ("\tRotation angle boundary warning!!!!")
            elif(HPulse <= 1):
                HPulse = 1
                print ("\tRotation angle boundary warning!!!!")
            pwm.setRotationAngle(1, HPulse)
            print("\tHPulse=%d"%HPulse)   

        elif(VStep != 0):
            VPulse += VStep
            if(VPulse >= 110): 
                VPulse = 110
                print ("\tRotation angle boundary warning!!!!")
            elif(VPulse <= 1):
                VPulse = 1
                print ("\tRotation angle boundary warning!!!!")
            pwm.setRotationAngle(0, VPulse)
            print("\tVPulse=%d"%VPulse)
        time.sleep(0.05) 

except:
    pwm.exit_PCA9685()
    #GPIO.cleanup()
    print "\nProgram end"
    exit()
