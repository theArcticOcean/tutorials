#!/usr/bin/python3
# -*- coding: utf-8 -*-

import os
import sys
import subprocess
import shutil

def getFolderSize( start_path ):
    total_size = 0
    for dirpath, dirnames, filenames in os.walk(start_path):
        for f in filenames:
            filePath = os.path.join(dirpath, f)
            # skip if it is symbolic link
            if not os.path.islink(filePath):
                total_size += os.path.getsize(filePath)
    return total_size

def get_process_id(name):
    child = subprocess.Popen(["pgrep","-f",name],stdout=subprocess.PIPE,shell=False)
    response = child.communicate()[0]
    return response

if __name__ == '__main__':
    pid1 = get_process_id("pdfToPNGs.py")
    if (not pid1):
        print( "no target pid to kill, go to work" )
    else:
        print( "error: we are handling, no more cpu resources to use" )
        exit( 1 )

    filePath = sys.argv[1]  #uploads/example.pdf
    # clear big folder: uploads
    strList = filePath.split("/")
    if strList.__len__() > 0 and getFolderSize( strList[0] ) > 1024*1024*50:
        shutil.rmtree( strList[0] )
        os.mkdir( strList[0] )
