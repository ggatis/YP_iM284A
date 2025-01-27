'''
 @file    main.py
 
 @brief   Implementation of a main menu for calling RadioHub requests
 
 @note    This example code is free software: you can redistribute it and/or modify it.
 
          This program is provided by IMST GmbH on an "AS IS" basis without
          any warranties in the hope that it will be useful.
 '''

import os
import sys
import time

import serial.tools.list_ports
import serial


from Utils.Keyboard import Keyboard

from RadioHub.RadioHub import RadioHub

# dictionary with radio commands
RadioCommands = {
    '1' : { "title" : "ping device",                "command" : "pingDevice" },
    '2' : { "title" : "read device information",    "command" : "getDeviceInfo" },
    '3' : { "title" : "read firmware version",      "command" : "getFirmwareInfo" },
    '4' : { "title" : "read date and time",         "command" : "getDateTime" },
    '5' : { "title" : "set date and time",          "command" : "setDateTime" },
    '6' : { "title" : "restart device",             "command" : "restartDevice" },
    '7' : { "title" : "read radio confiuration",    "command" : "getRadioConf" },
}

def main( argv ):

    os.system('cls')

    portName = serialPortByVendorID()

    if portName == None:
        print("\nPlease connect a radio module and/or install the required USB to UART bridge driver!\n")
        exit( None )

    print( "This application demonstrates the host controller message protocol for WiMOD radio modules provided by IMST.\n" )
    
    showMenu( portName )

    # create radioHub instance
    radioHub = RadioHub()

    # open serial port
    radioHub.open( portName, radioHub_Event )
    
    # create keyboard input device
    keyboard = Keyboard()

    run = True

    while run == True:

        if keyboard.kbhit():
            key = keyboard.getch()
            try: 
                item = RadioCommands[ key ]
            except:
                if ( ord( key ) == 27 ): #ESC
                    run = False
                elif ( key == ' ' ):
                    showMenu( portName )
            else:
                radioHub.deviceManagement.radioRequest( item[ "command" ] )

        # sleep a while for background packet handling by OS
        time.sleep( 0.1 )

        # process HCI receiver task
        radioHub.process()

    keyboard.set_normal_term()
    print( "goodbye" )

# find suitable serial port
def serialPortByVendorID() -> str:
    
    # FTDI, SILABS, STM
    supportedVendorIDs = [ "VID:PID=0403", "VID:PID=10C4", "VID:PID=0483" ]

    # get available serial ports
    ports = serial.tools.list_ports.comports(True)

    for port in ports:
        for vendorID in supportedVendorIDs:
            if ( vendorID in port.hwid ):
                return port.device
            
    return None

# show result from RadioHub
def radioHub_Event( result ):
    for key in result.keys():
        if ( key == "request" or key == "event" ):
            print("")
        show( key, result[ key ] )

# show main menu
def showMenu( portName ):
    print( '' )
    show( "used port", portName )
    print( '' )

    for key in RadioCommands:
        item = RadioCommands[ key ]
        show( key, item[ "title" ] )

    print( '' )
    show( "SPACE", "show this menu" )
    show( "ESC", "exit this application" )
    print( '' )
    show( "select a command", ">" )

# formatted output
def show( key : str, value : str ):
    print( key.rjust( 20, ' ' ) + " : " + value )

if __name__ == "__main__":
    main( sys.argv[ 1: ] )

