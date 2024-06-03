'''
 @file    Trace.py
 
 @brief   Implementation of several trace decoders
 
 @note    This example code is free software: you can redistribute it and/or modify it.
 
          This program is provided by IMST GmbH on an "AS IS" basis without
          any warranties in the hope that it will be useful.
 '''

from enum import IntEnum

from RadioHub.ServiceAccessPoints.ServiceAccessPoint import ServiceAccessPoint
from RadioHub.Utils.SerialMessage import SerialMessage

class Trace( ServiceAccessPoint ):
    # message identifier of DeviceManegemnt SAP
    class Msg_ID( IntEnum ):
        TraceEvent_1            =   0x01
        TraceEvent_5            =   0x05
        TraceEvent_6            =   0x06
        TraceEvent_7            =   0x07
        TraceEvent_8            =   0x08
        TraceEvent_9            =   0x09
        TraceEvent_10           =   0x0A
        TraceEvent_11           =   0x0B

    def decodeTraceEvent_1( self, serialMsg : SerialMessage, result : dict ):
        result[ "Event ID" ]    = hex( serialMsg.getU16( SerialMessage.EventData_Index ) )
        result[ "String" ]      = serialMsg.getString( SerialMessage.EventData_Index + 2, -1 )

    def decodeTraceEvent_5( self, serialMsg : SerialMessage, result : dict ):
        result[ "Event ID" ]    = hex( serialMsg.getU16( SerialMessage.EventData_Index ) )
        p1 = serialMsg.getU32( SerialMessage.EventData_Index + 2 )
        result[ "Param 1" ]     = str( p1 ) + "(" +  hex( p1 ) + ")"

    def decodeTraceEvent_6( self, serialMsg : SerialMessage, result : dict ):
        result[ "Event ID" ]    = hex( serialMsg.getU16( SerialMessage.EventData_Index ) )
        p1 = serialMsg.getU32( SerialMessage.EventData_Index + 2 )
        p2 = serialMsg.getU32( SerialMessage.EventData_Index + 6 )
        result[ "Param 1" ]     = str( p1 ) + "(" +  hex( p1 ) + ")"
        result[ "Param 2" ]     = str( p2 ) + "(" +  hex( p2 ) + ")"

    def decodeTraceEvent_7( self, serialMsg : SerialMessage, result : dict ):
        result[ "Event ID" ]    = hex( serialMsg.getU16( SerialMessage.EventData_Index ) )
        p1 = serialMsg.getU32( SerialMessage.EventData_Index + 2 )
        p2 = serialMsg.getU32( SerialMessage.EventData_Index + 6 )
        p3 = serialMsg.getU32( SerialMessage.EventData_Index + 10 )
        result[ "Param 1" ]     = str( p1 ) + "(" +  hex( p1 ) + ")"
        result[ "Param 2" ]     = str( p2 ) + "(" +  hex( p2 ) + ")"
        result[ "Param 3" ]     = str( p3 ) + "(" +  hex( p3 ) + ")"

    def decodeTraceEvent_8( self, serialMsg : SerialMessage, result : dict ):
        result[ "Event ID" ]    = hex( serialMsg.getU16( SerialMessage.EventData_Index ) )
        p1 = serialMsg.getU32( SerialMessage.EventData_Index + 2 )
        p2 = serialMsg.getU32( SerialMessage.EventData_Index + 6 )
        p3 = serialMsg.getU32( SerialMessage.EventData_Index + 10 )
        p4 = serialMsg.getU32( SerialMessage.EventData_Index + 14 )
        result[ "Param 1" ]     = str( p1 ) + "(" +  hex( p1 ) + ")"
        result[ "Param 2" ]     = str( p2 ) + "(" +  hex( p2 ) + ")"
        result[ "Param 3" ]     = str( p3 ) + "(" +  hex( p3 ) + ")"
        result[ "Param 4" ]     = str( p3 ) + "(" +  hex( p4 ) + ")"

    def decodeTraceEvent_9( self, serialMsg : SerialMessage, result : dict ):
        result[ "Event ID" ]    = hex( serialMsg.getU16( SerialMessage.EventData_Index ) )
        p1 = serialMsg.getU32( SerialMessage.EventData_Index + 2 )
        result[ "Param 1" ]     = str( p1 ) + "(" +  hex( p1 ) + ")"
        result[ "String" ]      = serialMsg.getString( SerialMessage.EventData_Index + 6, -1 )

    def decodeTraceEvent_10( self, serialMsg : SerialMessage, result : dict ):
        result[ "Event ID" ]    = hex( serialMsg.getU16( SerialMessage.EventData_Index ) )
        p1 = serialMsg.getU32( SerialMessage.EventData_Index + 2 )
        p2 = serialMsg.getU32( SerialMessage.EventData_Index + 6 )
        result[ "Param 1" ]     = str( p1 ) + "(" +  hex( p1 ) + ")"
        result[ "Param 2" ]     = str( p2 ) + "(" +  hex( p2 ) + ")"
        result[ "String" ]      = serialMsg.getString( SerialMessage.EventData_Index + 10, -1 )

    def decodeTraceEvent_11( self, serialMsg : SerialMessage, result : dict ):
        result[ "Module ID" ]   = str( serialMsg.getU8( SerialMessage.EventData_Index ) )
        result[ "State" ]       = str( serialMsg.getU8( SerialMessage.EventData_Index + 1 ) )
        result[ "Slot" ]        = str( serialMsg.getU16( SerialMessage.EventData_Index + 2 ) )
        result[ "Multiframe" ]  = str( serialMsg.getU16( SerialMessage.EventData_Index + 4 ) )
        result[ "String" ]      = serialMsg.getString( SerialMessage.EventData_Index + 6, -1 )
        
    # dictinary with message decoders 
    EventDecoders = {
        Msg_ID.TraceEvent_1       : { "Name" : "trace event #1",        "Handler" : decodeTraceEvent_1 },
        Msg_ID.TraceEvent_5       : { "Name" : "trace event #5",        "Handler" : decodeTraceEvent_5 },
        Msg_ID.TraceEvent_6       : { "Name" : "trace event #6",        "Handler" : decodeTraceEvent_6 },
        Msg_ID.TraceEvent_7       : { "Name" : "trace event #7",        "Handler" : decodeTraceEvent_7 },
        Msg_ID.TraceEvent_8       : { "Name" : "trace event #8",        "Handler" : decodeTraceEvent_8 },
        Msg_ID.TraceEvent_9       : { "Name" : "trace event #9",        "Handler" : decodeTraceEvent_9 },
        Msg_ID.TraceEvent_10      : { "Name" : "trace event #10",       "Handler" : decodeTraceEvent_10 },
        Msg_ID.TraceEvent_11      : { "Name" : "trace event #11",       "Handler" : decodeTraceEvent_11 },
    }

    def __init__( self, client, serialDevice, sap_id ):
        super().__init__( client        = client,
                          serialDevice  = serialDevice, 
                          sap_id        = sap_id, 
                          eventDecoders = self.EventDecoders )
