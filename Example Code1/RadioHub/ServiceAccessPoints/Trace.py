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
        index = SerialMessage.Status_Index
        result[ "event ID" ]    = hex( serialMsg.getU16( index ) )
        result[ "string" ]      = serialMsg.getString( index + 2, -1 )

    def decodeTraceEvent_5( self, serialMsg : SerialMessage, result : dict ):
        index = SerialMessage.Status_Index
        result[ "event ID" ]    = hex( serialMsg.getU16( index ) )
        p1 = serialMsg.getU32( index + 2 )
        result[ "param 1" ]     = str( p1 ) + "(" +  hex( p1 ) + ")"

    def decodeTraceEvent_6( self, serialMsg : SerialMessage, result : dict ):
        index = SerialMessage.Status_Index
        result[ "event ID" ]    = hex( serialMsg.getU16( index ) )
        p1 = serialMsg.getU32( index + 2 )
        p2 = serialMsg.getU32( index + 6 )
        result[ "param 1" ]     = str( p1 ) + "(" +  hex( p1 ) + ")"
        result[ "param 2" ]     = str( p2 ) + "(" +  hex( p2 ) + ")"

    def decodeTraceEvent_7( self, serialMsg : SerialMessage, result : dict ):
        index = SerialMessage.Status_Index
        result[ "event ID" ]    = hex( serialMsg.getU16( index ) )
        p1 = serialMsg.getU32( index + 2 )
        p2 = serialMsg.getU32( index + 6 )
        p3 = serialMsg.getU32( index + 10 )
        result[ "param 1" ]     = str( p1 ) + "(" +  hex( p1 ) + ")"
        result[ "param 2" ]     = str( p2 ) + "(" +  hex( p2 ) + ")"
        result[ "param 3" ]     = str( p3 ) + "(" +  hex( p3 ) + ")"

    def decodeTraceEvent_8( self, serialMsg : SerialMessage, result : dict ):
        index = SerialMessage.Status_Index
        result[ "event ID" ]    = hex( serialMsg.getU16( index ) )
        p1 = serialMsg.getU32( index + 2 )
        p2 = serialMsg.getU32( index + 6 )
        p3 = serialMsg.getU32( index + 10 )
        p4 = serialMsg.getU32( index + 14 )
        result[ "param 1" ]     = str( p1 ) + "(" +  hex( p1 ) + ")"
        result[ "param 2" ]     = str( p2 ) + "(" +  hex( p2 ) + ")"
        result[ "param 3" ]     = str( p3 ) + "(" +  hex( p3 ) + ")"
        result[ "param 4" ]     = str( p3 ) + "(" +  hex( p4 ) + ")"

    def decodeTraceEvent_9( self, serialMsg : SerialMessage, result : dict ):
        index = SerialMessage.Status_Index
        result[ "event ID" ]    = hex( serialMsg.getU16( index ) )
        p1 = serialMsg.getU32( index + 2 )
        result[ "param 1" ]     = str( p1 ) + "(" +  hex( p1 ) + ")"
        result[ "string" ]      = serialMsg.getString( index + 6, -1 )

    def decodeTraceEvent_10( self, serialMsg : SerialMessage, result : dict ):
        index = SerialMessage.Status_Index
        result[ "event ID" ]    = hex( serialMsg.getU16( index ) )
        p1 = serialMsg.getU32( index + 2 )
        p2 = serialMsg.getU32( index + 6 )
        result[ "param 1" ]     = str( p1 ) + "(" +  hex( p1 ) + ")"
        result[ "param 2" ]     = str( p2 ) + "(" +  hex( p2 ) + ")"
        result[ "string" ]      = serialMsg.getString( index + 10, -1 )

    def decodeTraceEvent_11( self, serialMsg : SerialMessage, result : dict ):
        index = SerialMessage.Status_Index
        result[ "event ID" ]    = hex( serialMsg.getU8( index ) )
        result[ "state" ]       = hex( serialMsg.getU8( index + 1 ) )
        result[ "slot" ]        = hex( serialMsg.getU16( index + 2 ) )
        result[ "multiframe" ]  = hex( serialMsg.getU16( index + 4 ) )
        result[ "string" ]      = serialMsg.getString( index + 6, -1 )
        
    # dictinary with message decoders 
    EventDecoders = {
        Msg_ID.TraceEvent_1       : { "name" : "trace event #1",        "handler" : decodeTraceEvent_1 },
        Msg_ID.TraceEvent_5       : { "name" : "trace event #5",        "handler" : decodeTraceEvent_5 },
        Msg_ID.TraceEvent_6       : { "name" : "trace event #6",        "handler" : decodeTraceEvent_6 },
        Msg_ID.TraceEvent_7       : { "name" : "trace event #7",        "handler" : decodeTraceEvent_7 },
        Msg_ID.TraceEvent_8       : { "name" : "trace event #8",        "handler" : decodeTraceEvent_8 },
        Msg_ID.TraceEvent_9       : { "name" : "trace event #9",        "handler" : decodeTraceEvent_9 },
        Msg_ID.TraceEvent_10      : { "name" : "trace event #10",       "handler" : decodeTraceEvent_10 },
        Msg_ID.TraceEvent_11      : { "name" : "trace event #11",       "handler" : decodeTraceEvent_11 },
    }

    def __init__( self, client, serialDevice, sap_id ):
        super().__init__( client        = client,
                          serialDevice  = serialDevice, 
                          sap_id        = sap_id, 
                          eventDecoders = self.EventDecoders )
