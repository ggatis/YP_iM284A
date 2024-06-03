'''
 @file    RadioHub.py
 
 @brief   Implementation of class RadioHub as main interface to further service access points like "DeviceManagement"
 
 @note    This example code is free software: you can redistribute it and/or modify it.
 
          This program is provided by IMST GmbH on an "AS IS" basis without
          any warranties in the hope that it will be useful.
 '''
 
import serial
import sliplib.slip

from RadioHub.ServiceAccessPoints.ServiceAccessPoint import ServiceAccessPoint_ID
from RadioHub.ServiceAccessPoints.DeviceManagement import DeviceManagement
from RadioHub.ServiceAccessPoints.Trace import Trace
from RadioHub.Utils.SerialMessage import SerialMessage

class RadioHub:

    def __init__( self ):
        self.serialDevice = None
        self.slip = None
        self.deviceManagement = None
        self.serviceAccessPoints = {}

    def open( self, portName, client ) -> bool:

        # save client    
        self.client = client

         # open serial port
        self.serialDevice = serial.Serial( port = portName, baudrate = 115200, timeout = 0 )
        
        # create SLIP encoder/decoder
        self.slip = sliplib.slip.Driver()

        # create DeviceManagement SAP
        self.deviceManagement  = DeviceManagement( client = client, serialDevice = self.serialDevice, sap_id = ServiceAccessPoint_ID.DeviceMgmt ) 
        # and add to dictionary
        self.serviceAccessPoints[ ServiceAccessPoint_ID.DeviceMgmt ] = self.deviceManagement

        # create and add Trace SAP
        self.serviceAccessPoints[ ServiceAccessPoint_ID.Trace ] = Trace( client = client, serialDevice = self.serialDevice, sap_id = ServiceAccessPoint_ID.Trace )
        
        return True

    # perform message receiver tasks
    def process( self ):

        # read data from serial device    
        data = self.serialDevice.read( 100 )

        if ( len( data ) > 0 ):
            
            # get slip decoded messages
            messages = self.slip.receive( data )        
            
            # decode HCI messages
            for msg in messages:
                serialMsg = SerialMessage()

                # decde byte stream and check crc
                if ( serialMsg.decode( msg ) == True ):
                    # dispatch HCI message
                    result = {}
                    try:
                        # find sap
                        sap = self.serviceAccessPoints[ serialMsg.sap_id ]
                    except:
                        # error
                        result[ "error" ] = "unknown service access point called: " + hex( serialMsg.sap_id )     
                    else:
                        # call message decoder
                        sap.decodeMessage( serialMsg = serialMsg, result = result )
                    
                    # notify client about results    
                    self.client( result )