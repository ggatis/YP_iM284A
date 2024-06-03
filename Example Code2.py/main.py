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
from datetime import datetime, timezone
import time

from RadioHub.RadioHub import RadioHub

# dictionary with radio Commands
RadioCommands = {
    # Device Manangement Commands
    '-' : { "Title" : "Device Management Services" },
    '1' : { "Title" : "ping Device",                "Params": False, "Service" : "DeviceMgmt.pingDevice" },
    '2' : { "Title" : "read Device Information",    "Params": False, "Service" : "DeviceMgmt.getDeviceInfo" },
    '3' : { "Title" : "read Firmware Version",      "Params": False, "Service" : "DeviceMgmt.getFirmwareInfo" },
    '4' : { "Title" : "read Date and Time",         "Params": False, "Service" : "DeviceMgmt.getDateTime" },
    '5' : { "Title" : "set Date and Time",          "Params": True,  "Service" : "DeviceMgmt.setDateTime" },
    '6' : { "Title" : "restart Device",             "Params": False, "Service" : "DeviceMgmt.restartDevice" },
    '7' : { "Title" : "get System Options",         "Params": False, "Service" : "DeviceMgmt.getSystemOptions" },
    '8' : { "Title" : "enable Trace Output",        "Params": True,  "Service" : "enableTraceOutput" },
    '9' : { "Title" : "disable Trace Output",       "Params": True,  "Service" : "disableTraceOutput" },
    # LoraMeshRouter Commands
    '+' : { "Title" : "LoRa Mesh Router Services" },
    'a' : { "Title" : "get Network Address",        "Params": False, "Service" : "LoRaMeshRouter.getNetworkAddress" },
    'b' : { "Title" : "set Network Address(A)",     "Params": True,  "Service" : "setNetworkAddress_A" },
    'c' : { "Title" : "set Network Address(B)",     "Params": True,  "Service" : "setNetworkAddress_B" },
    'd' : { "Title" : "get Mode",                   "Params": False, "Service" : "LoRaMeshRouter.getMode" },
    'e' : { "Title" : "disable Node",               "Params": True,  "Service" : "disableNode" },
    'f' : { "Title" : "enable Router",              "Params": True,  "Service" : "enableRouter" },
    'g' : { "Title" : "enable Coordinator",         "Params": True,  "Service" : "enableCoordinator" },
    'h' : { "Title" : "get Link Status",            "Params": False, "Service" : "LoRaMeshRouter.getLinkStatus" },
    'i' : { "Title" : "get Routing Info",           "Params": True,  "Service" : "getRoutingInfo" },
    'j' : { "Title" : "send Packet to Node(A)",     "Params": True,  "Service" : "sendPacket to Node A" },
    'k' : { "Title" : "send Packet to Node(B)",     "Params": True,  "Service" : "sendPacket to Node B" },
 }

# Network ID
Network_ID = "00-2A"

# Device EUI for Node(A)
DeviceEUI_Node_A = "01-aa-aa-aa-02-aa-aa-aa"

# test payload
Payload_for_Node_A = "AA-01-02-02-04-05-06-07-08-AA"

# Device EUI for Node(B)
DeviceEUI_Node_B = "03-bb-bb-bb-04-bb-bb-bb"

# test payload
Payload_for_Node_B = "BB-01-02-02-04-05-06-07-08-09-0A-0B-0C-0D-0E-0F-BB"

# a user port range( 1 - 127 ), reserved( 0, 128 .. 255 )
User_Port = 21 

def main( argv ):

    os.system( 'cls' )
    
    portName = serialPortByVendorID()

    if portName == None:
        print( "\nPlease connect a radio module and/or install the required USB to UART bridge driver!\n" )
        exit( None )

    print(  "This application demonstrates the host controller message protocol for WiMOD radio modules provided by IMST.\n" )
    
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
                service = item[ "Service" ]
                if item[ "Params" ] == True:
                    # get parameters for request from this module first
                    # use same service name on application and raduiHub 
                    apiRequest( radioHub, service )
                else:    
                    # perform simple radio request without parameters
                    params = {}
                    radioHub.radioRequest( service, params )

        # sleep a while for background packet handling by OS
        time.sleep( 0.1 )
 
        # process HCI receiver task
        radioHub.process()

    keyboard.set_normal_term()
    print( "goodbye" )

# set date and time to current date time
def setDateTime( radioHub, service ):
    params = {}
    params[ "SecondsSinceEpoch" ]  = int( datetime.now( timezone.utc ).timestamp() )
    radioHub.radioRequest( service, params )

# enable trace output
def enableTraceOutput( radioHub, service ):
    params =  {}
    params[ "Options" ] = "Trace = on"
    radioHub.radioRequest( "DeviceMgmt.setSystemOptions", params )

# disable trace output
def disableTraceOutput( radioHub, service ):
    params =  {}
    params[ "Options" ] = "Trace = off"
    radioHub.radioRequest( "DeviceMgmt.setSystemOptions", params )

# set Network-ID and Device-EUI for a node "a"
def setNetworkAddress_A( radioHub, service ):
    params = {}
    params[ "Network-ID" ] = Network_ID
    params[ "Device-EUI" ] = DeviceEUI_Node_A 
    radioHub.radioRequest( "LoRaMeshRouter.setDeviceEUI", params )

# set Network-ID and Device-EUI for a node "b"
def setNetworkAddress_B( radioHub, service ):
    params = {}
    params[ "Network-ID" ] = Network_ID
    params[ "Device-EUI" ] = DeviceEUI_Node_B  
    radioHub.radioRequest( "LoRaMeshRouter.setDeviceEUI", params )

# disable router/coordinator
def disableNode( radioHub, service ):
    params = {}
    params[ "Mode" ] = "Off"
    radioHub.radioRequest( "LoRaMeshRouter.setMode", params )

# enable router
def enableRouter( radioHub, service ):
    params = {}
    params[ "Mode" ] = "Router"
    radioHub.radioRequest( "LoRaMeshRouter.setMode", params )

# enable coordinator
def enableCoordinator( radioHub, service ):
    params = {}
    params[ "Mode" ] = "Coordinator"
    radioHub.radioRequest( "LoRaMeshRouter.setMode", params )

# get routing info
def getRoutingInfo( radioHub, service ):
    params = {}
    # start index for table
    params[ "Index" ]           = 0
    # read MaxItems per request/response
    params[ "MaxItems" ]        = 4
    radioHub.radioRequest( "LoRaMeshRouter.getRoutingInfo", params )

# send a packet to Node A
def sendPacket_to_Node_A( radioHub, service ):
    params = {}
    params[ "Destination-EUI" ] = DeviceEUI_Node_A
    params[ "Port" ]            = User_Port
    params[ "Payload" ]         = Payload_for_Node_A 
    radioHub.radioRequest( "LoRaMeshRouter.sendPacket", params )

# send a packet to Node B
def sendPacket_to_Node_B( radioHub, service ):
    params = {}
    params[ "Destination-EUI" ] = DeviceEUI_Node_B
    params[ "Port" ]            = User_Port
    params[ "Payload" ]         = Payload_for_Node_B 
    radioHub.radioRequest( "LoRaMeshRouter.sendPacket", params )

# table with services which require addtional parameters
Service = {
    "DeviceMgmt.setDateTime"                : setDateTime,
    "enableTraceOutput"                     : enableTraceOutput,
    "disableTraceOutput"                    : disableTraceOutput,
    "setNetworkAddress_A"                   : setNetworkAddress_A,  
    "setNetworkAddress_B"                   : setNetworkAddress_B,  
    "disableNode"                           : disableNode,
    "enableRouter"                          : enableRouter,
    "enableCoordinator"                     : enableCoordinator,
    "getRoutingInfo"                        : getRoutingInfo,
    "sendPacket to Node A"                  : sendPacket_to_Node_A,
    "sendPacket to Node B"                  : sendPacket_to_Node_B,
}

# handle api requests with parameters
def apiRequest( radioHub, radioService ):
    result = {}
    try:
        service = Service[ radioService ]
    except:
        result[ "Error" ] = "unhandled service called: " + radioService
        radioHub_Event( result )
    else:
        service( radioHub, radioService )    
        
# find suitable serial port
def serialPortByVendorID() -> str:
    
    # FTDI, SILABS, STM, Cypress ( for iU881'A-XL )
    supportedVendorIDs = [ "VID:PID=0403", "VID:PID=10C4", "VID:PID=0483", "VID_04B4&PID_0003" ]

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
        if ( key == "Request" or key == "Event" ):
            print( "" )

        value = result[ key ]
        if type( value ) == str:
            showItem( key, value )
        elif type( value ) == list:
            showList( key, value )

# show main menu
def showMenu( portName ):
    print( '' )
    showItem( "used port", portName )
    print( '' )

    for key in RadioCommands:
        item = RadioCommands[ key ]
        if key == '-' or key == '+':
            showLine()
            showTitle( item[ "Title" ] )
            showLine()
        else:    
            showItem( key, item[ "Title" ] )

    showLine()
    showItem( "SPACE", "show this menu" )
    showItem( "ESC", "exit this application" )
    showLine()
    print( ' ' )
    showItem( "select a command", ">" )

# formatted output
def showItem( key : str, value : str ):
    print( key.rjust( 32, ' ' ) + " : " + value )

# formatted output
def showList( key : str, values : list ):
    print( key.rjust( 32, ' ' ) )

    for i in range( len( values ) ):
        item = values[ i ]

        for key_2 in item.keys():
            showItem( key_2, item[ key_2] )
        
        print( "" )

# formatted output
def showTitle( title : str ):
    print( title.rjust( 32, ' ' )  )

def showLine():
    print( "-" * 80 )

if __name__ == "__main__":
    main( sys.argv[ 1: ] )

