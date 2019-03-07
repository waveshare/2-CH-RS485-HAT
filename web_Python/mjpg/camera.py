# -*- coding: UTF-8 -*- 
import os
import sys



def camera():
    o_path = os.getcwd()
    #sys.path.append(o_path)
    os.chdir(o_path)
    os.system('./start.sh') 
    # print "zzzzz"
    


