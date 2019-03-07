# -*- coding: UTF-8 -*- 
import threading
import SocketServer 
import RPi.GPIO as GPIO
from PCA9685 import PCA9685
from SocketServer import StreamRequestHandler as SRH  
from time import ctime  
import time 

import thread
import os
import sys

o_path = os.getcwd()
sys.path.append(o_path)
os.chdir('/home/pi/RaspberryPi/web_Python/mjpg')
from mjpg import camera

pwm = PCA9685()
pwm.setPWMFreq(50)
pwm.setRotationAngle(0, 0)
pwm.setRotationAngle(1, 0)

host = '192.168.1.135'  
port = 8000
addr = (host,port) 
     
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
                print("Stop")
            elif data == "Forward":
                print("Forward")
            elif data == "Backward":
                print("Backward")
            elif data == "TurnLeft":
                print("TurnLeft")
            elif data == "TurnRight":
                print("TurnRight")
            elif data == "Up":
                VStep = -1
            elif data == "Down":
                VStep = 1
            elif data == "Left":
                HStep = 1
            elif data == "Right":
                HStep = -1
            else:
                value = 0
                try:
                    value = int(data)
                    if(value >= 0 and value <= 100):
                        print(value)
                except:
                    print("Command error")
            print data   
            #print "recv from ", self.client_address[0]  
            self.request.send(data) 
            
            #timerfunc()
            if(HStep != 0):
                HPulse += HStep
                if(HPulse >= 179): 
                    HPulse = 179
                elif(HPulse <= 1):
                    HPulse = 1
                pwm.setRotationAngle(1, HPulse)
                # print("          HPulse=", HPulse)   
            
            elif(VStep != 0):
                VPulse += VStep
                if(VPulse >= 80): 
                    VPulse = 80
                elif(VPulse <= 1):
                    VPulse = 1
                pwm.setRotationAngle(0, VPulse)
                # print("          HPulse=", VPulse)
             
def timerfunc():  
    global t        #Notice: use global variable!
    t = threading.Timer(0.02, timerfunc)
    t.start()
try:  
    t = threading.Timer(0.02, timerfunc)
    t.setDaemon(True)
    t.start()

    print 'server is running....'  
    server = SocketServer.ThreadingTCPServer(addr,Servers)
    time.sleep(0.1) 
    thread.start_new_thread(server.serve_forever, ()) 
    thread.start_new_thread(camera.camera())
    #server.serve_forever()

    #os.system('./start.sh') 

    while True: 
        time.sleep(1) 

except:
    pwm.exit_PCA9685()
    GPIO.cleanup()
    print "\nProgram end"
    exit()
