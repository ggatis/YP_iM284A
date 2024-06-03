'''
 @file    ServiceAccessPoint.py
 
 @brief   This class is a base class for all service access points. It provides basic 
          dispatching functions for incoming and outgoing HCI messages.
 
 @note    This example code is free software: you can redistribute it and/or modify it.
 
          This program is provided by IMST GmbH on an "AS IS" basis without
          any warranties in the hope that it will be useful.
 '''

import sliplib.slip

from enum import IntEnum

from RadioHub.Utils.SerialMessage import SerialMessage

# identifier of available SAPs
class ServiceAccessPoint_ID( IntEnum ):
        DeviceMgmt      =   0x01
        LoRaMeshRouter  =   0x0A
        Trace           =   0xA0

class ServiceAccessPoint:

    def __init__( self, client, serialDevice, sap_id, responseDecoders = None, statusCodes = None, encoders = None, eventDecoders = None ):
        self.client = client
        self.serialDevice = serialDevice
        self.slip = sliplib.slip.Driver()
        self.sap_id = sap_id
        self.responseDecoders = responseDecoders
        self.statusCodes = statusCodes
        self.encoders = encoders
        self.eventDecoders = eventDecoders
        
    def radioRequest( self, encoderName, params ):
        result = {}
        try:
            encoder = self.encoders[ encoderName ]
        except:
            result[ "Error" ] = "unhandled encoder called: " + encoderName
        else:
            result[ "Request" ] = encoderName + " request"

            # call handler of derived class
            encoder[ "Handler" ]( self, encoder[ "ID" ], params )

            # notify client
            self.client( result )

    def sendRequest( self, msg_id, payload = b'' ):
        serialMessage = SerialMessage( sap_id = self.sap_id, msg_id = msg_id, payload = payload )
        self.sendMessage( serialMessage )

    def sendMessage( self, serialMessage : SerialMessage ):
        self.serialDevice.write( self.slip.send( serialMessage.encode() ) )

    def decodeMessage( self, serialMsg : SerialMessage, result : dict ):
        try:
            decoder = self.responseDecoders[ serialMsg.msg_id ]
        except:
            try:    
                decoder = self.eventDecoders[ serialMsg.msg_id ]
            except:
                result[ "Error" ] = "unhandled message identifier received: " + hex( serialMsg.msg_id )
            else:
                result[ "Event" ] = decoder[ "Name" ]
    
                # call handler of derived class
                decoder[ "Handler" ]( self, serialMsg = serialMsg, result = result )
        else:
            result[ "Response" ] = decoder[ "Name" ]

            # call handler of derived class
            decoder[ "Handler" ]( self, serialMsg = serialMsg, result = result )

    def decodeDefaultResponse( self, serialMsg : SerialMessage, result : dict ):
        result[ "Status" ] = self.statusCodes.get( serialMsg.getStatus(), "unknown status code" )
        