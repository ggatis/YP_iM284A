'''
 @file    LoRaMshRouter.py
 
 @brief   Implementation of LoRaMeshRouter message encoders and decoders
 
 @note    This example code is free software: you can redistribute it and/or modify it.
 
          This program is provided by IMST GmbH on an "AS IS" basis without
          any warranties in the hope that it will be useful.
 '''

from enum import IntEnum

from RadioHub.ServiceAccessPoints.ServiceAccessPoint import ServiceAccessPoint
from RadioHub.Utils.SerialMessage import SerialMessage

from datetime import datetime, timezone
import time

import struct

class LoRaMeshRouter( ServiceAccessPoint ):
    # message identifier of DeviceManegemnt SAP
    class Msg_ID( IntEnum ):
        # DeviceEUI
        GetNetworkAddress_Req               =   0x01
        GetNetworkAddress_Rsp               =   0x02
        SetNetworkAddress_Req               =   0x03
        SetNetworkAddress_Rsp               =   0x04

        # Mode
        GetMode_Req                         =   0x11
        GetMode_Rsp                         =   0x12
        SetMode_Req                         =   0x13
        SetMode_Rsp                         =   0x14
        
        # router status
        GetLinkStatus_Req                   =   0x15
        GetLinkStatus_Rsp                   =   0x16    
        LinkStatusChange_Ind                =   0x18

        # routing info
        GetRoutingInfo_Req                  =   0x19
        GetRoutingInfo_Rsp                  =   0x1A

        # packet transmision
        SendPacket_Req                      =   0x21
        SendPacket_Rsp                      =   0x22

        # packet reception
        PacketReceived_Ind                  =   0x26

    # status codes of DeviceManagement SAP
    class Status_ID( IntEnum ):
        Ok                                  =   0x00
        Error                               =   0x01
        CommandNotSupported                 =   0x02
        WrongParameter                      =   0x03
        WrongApplicationMode                =   0x04
        NoMoreData                          =   0x05
        ApplicationBusy                     =   0x06
        WrongMessageLength                  =   0x07
        NVM_WriteError                      =   0x08
        NVM_ReadError                       =   0x09
        CommandRejected                     =   0x0A
        NoLink                              =   0x0B
        NoRoute                             =   0x0C
        WrongAddress                        =   0x0D
        NoBuffer                            =   0x0E
        TransmitQueueFull                   =   0x0F

    # dictionary with status codes
    StatusCodes = {
        Status_ID.Ok                        :   "ok",
        Status_ID.Error                     :   "error",
        Status_ID.CommandNotSupported       :   "command not supported",
        Status_ID.WrongParameter            :   "wrong parameter",
        Status_ID.WrongApplicationMode      :   "wrong application mode",
        Status_ID.NoMoreData                :   "no more data",
        Status_ID.ApplicationBusy           :   "application busy",
        Status_ID.WrongMessageLength        :   "wrong message length",
        Status_ID.NVM_WriteError            :   "NVM write error",
        Status_ID.NVM_ReadError             :   "NVM read error",
        Status_ID.CommandRejected           :   "command rejected",
        Status_ID.NoLink                    :   "no link",
        Status_ID.NoRoute                   :   "no route",
        Status_ID.WrongAddress              :   "wrong address",
        Status_ID.NoBuffer                  :   "no buffer",
        Status_ID.TransmitQueueFull         :   "transmit queue full",
    }

    # Modes
    class Mode_ID( IntEnum ):
        Off                                 =   0
        Router                              =   1
        Coordinator                         =   2

    # router modes: a mesh network must have only one single coordinator!
    Modes = {
        Mode_ID.Off                         :   "Off",
        Mode_ID.Router                      :   "Router",
        Mode_ID.Coordinator                 :   "Coordinator",
    }

    # RSSI Offset
    class RSSI_Offset( IntEnum ):
        Value                               =   64

    # send a simple request
    def defaultRequest( self, msg_id, params ):
        super().sendRequest( msg_id = msg_id )

    # set address parameters
    def setNetworkAddress( self, msg_id, params ):
        # encode Network-ID and DeviceEUI
        payload = bytearray( b'' )
        payload = self.appendHexString_LSB( payload, params[ "Network-ID" ] )
        payload = self.appendHexString_LSB( payload, params[ "Device-EUI" ] )
        super().sendRequest( msg_id = msg_id, payload = payload )

    # set mode
    def setMode( self, msg_id, params ):
        # reset link modes to zero
        mode_s  = params[ "Mode" ]
        mode = self.Mode_ID.Off
        
        if mode_s == "Router":
            mode = self.Mode_ID.Router
        elif mode_s == "Coordinator":    
            mode = self.Mode_ID.Coordinator

        payload = bytes( [ mode ] ) 
        super().sendRequest( msg_id = msg_id, payload = payload )

    def getRoutingInfo( self, msg_id, params ):
        payload = bytearray( b'' )
        payload = bytes( [ params[ "Index" ] ] ) + bytes( [ params[ "MaxItems" ] ] )
        super().sendRequest( msg_id = msg_id, payload = payload )

    def sendPacket( self, msg_id, params ):
        payload = bytearray( b'' )
 
        # tx options ( reserved for future )
        txOptions = 0x00
        payload = payload + bytes( [ txOptions ] )
 
        # destination EUI
        payload = self.appendHexString_LSB( payload, params[ "Destination-EUI" ] )
        # port
        payload = payload + bytes( [ params[ "Port" ] ] )
        # payload
        payload = self.appendHexString( payload, params[ "Payload" ] )
        super().sendRequest( msg_id = msg_id, payload = payload )

    def appendHexString( self, payload : bytearray, string : str ) -> bytearray:
            # replace delimters
            s = string.replace( '-', '' )
            # convert from Hex ASCII to bin
            data = bytearray.fromhex( s )
            payload = payload + data
            return payload

    def appendHexString_LSB( self, payload : bytearray, string : str ) -> bytearray:
            # replace delimters
            s = string.replace( '-', '' )
            # convert from Hex ASCII to bin
            data = bytearray.fromhex( s )
            payload = payload + data[::-1]
            return payload

    # dictinary with message encoders
    Encoders = {
        "getNetworkAddress"     :  { "ID" : Msg_ID.GetNetworkAddress_Req,       "Handler" : defaultRequest }, 
        "setNetworkAddress"     :  { "ID" : Msg_ID.SetNetworkAddress_Req,       "Handler" : setNetworkAddress }, 
        "getMode"               :  { "ID" : Msg_ID.GetMode_Req,                 "Handler" : defaultRequest },
        "setMode"               :  { "ID" : Msg_ID.SetMode_Req,                 "Handler" : setMode },
        "getLinkStatus"         :  { "ID" : Msg_ID.GetLinkStatus_Req,           "Handler" : defaultRequest },
        "getRoutingInfo"        :  { "ID" : Msg_ID.GetRoutingInfo_Req,          "Handler" : getRoutingInfo },
        "sendPacket"            :  { "ID" : Msg_ID.SendPacket_Req,              "Handler" : sendPacket }, 
    }    

    # default response decoder -> call super
    def decodeDefaultResponse( self, serialMsg : SerialMessage, result : dict ):
        super().decodeDefaultResponse( serialMsg, result = result )

    # decode Network Address response
    def decodeNetworkAddressResponse( self, serialMsg : SerialMessage, result : dict ):
        self.decodeDefaultResponse( serialMsg, result )
        # 2 bytes for Netowrk ID
        result[ "Network-ID" ]               = serialMsg.getHexString_LSB( SerialMessage.ResponseData_Index, 2 )
        # 8 bytes for 64 Bit DeviceEUI, LSB first
        result[ "Device-EUI" ]               = serialMsg.getHexString_LSB( SerialMessage.ResponseData_Index + 2, 8 )

    # decode Mode
    def decodeModeResponse( self, serialMsg : SerialMessage, result : dict ):
        self.decodeDefaultResponse( serialMsg, result )
        index            = SerialMessage.ResponseData_Index
        result[ "Mode" ] = self.Modes.get( serialMsg.getU8( index ), "unknown mode" )

    def decodeLinkStatusResponse( self, serialMsg: SerialMessage, result : dict ):
        self.decodeDefaultResponse( serialMsg, result )
        index                               = SerialMessage.ResponseData_Index
        result[ "Node Type" ]               = str( serialMsg.getU8( index ) )
        result[ "State" ]                   = str( serialMsg.getU8( index + 1 ) )
        result[ "Node Address" ]            = serialMsg.getHexString_LSB( index + 2, 2 )
        result[ "Rank" ]                    = str( serialMsg.getU8( index + 4 ) )
        result[ "Cell Size" ]               = str( serialMsg.getU8( index + 5 ) )
        result[ "Beacon Index" ]            = str( serialMsg.getU8( index + 6 ) )

    def decodeRoutingInfoResponse( self, serialMsg: SerialMessage, result : dict ):
        self.decodeDefaultResponse( serialMsg, result )
        index = SerialMessage.ResponseData_Index
        array = []

        # DeviceEUI(8) + NodeAddress(2) + RouterAddress(2) + NodeType(1) + State(1) + Rank(1) + BeaconIndex(1) + Visibility(1) + RSSI(1) + FW_Version(2)
        itemSize = 8 + 2 + 2 + 1 + 1 + 1 + 1 + 1 + 1 + 2
        len      = serialMsg.getPayloadLength()
        
        while ( ( index + itemSize ) <= len ):
            item = {}
            item[ "Device-EUI" ]        = serialMsg.getHexString_LSB( index, 8 )
            item[ "Local Address" ]     = serialMsg.getHexString_LSB( index + 8, 2 )
            item[ "Router Address" ]    = serialMsg.getHexString_LSB( index + 10, 2 )
            item[ "Node Type" ]         = str( serialMsg.getU8( index + 12 ) )
            item[ "State" ]             = str( serialMsg.getU8( index + 13 ) )
            item[ "Rank" ]              = str( serialMsg.getU8( index + 14 ) )
            item[ "Beacon Index" ]      = str( serialMsg.getU8( index + 15 ) )
            item[ "Visibility" ]        = str( serialMsg.getU8( index + 16 ) )
            item[ "RSSI" ]              = str( serialMsg.getI8( index + 17 ) - self.RSSI_Offset.Value ) + " dBm"
            item[ "FW Version" ]        = str( serialMsg.getU16( index + 18 ) )

            array.append( item )

            index = index + itemSize

        result[ "Mesh Router Nodes" ] = array
         
    # dictionary with response message decoders 
    ResponseDecoders = {
        Msg_ID.GetNetworkAddress_Rsp    : { "Name" : "getNetworkAddress response",      "Handler" : decodeNetworkAddressResponse },
        Msg_ID.SetNetworkAddress_Rsp    : { "Name" : "setNetworkAddress response",      "Handler" : decodeDefaultResponse },
        Msg_ID.GetMode_Rsp              : { "Name" : "getMode response",                "Handler" : decodeModeResponse },
        Msg_ID.SetMode_Rsp              : { "Name" : "setMode response",                "Handler" : decodeDefaultResponse },
        Msg_ID.GetLinkStatus_Rsp        : { "Name" : "getLinkStatus response",          "Handler" : decodeLinkStatusResponse },
        Msg_ID.GetRoutingInfo_Rsp       : { "Name" : "getRoutingInfo response",         "Handler" : decodeRoutingInfoResponse },
        Msg_ID.SendPacket_Rsp           : { "Name" : "sendPacket response",             "Handler" : decodeDefaultResponse },
    }

    # decode Link Status Change event
    def decodeLinkStatusChangeEvent( self, serialMsg : SerialMessage, result : dict ):
        index                           = SerialMessage.EventData_Index
        result[ "Node Type" ]           = str( serialMsg.getU8( index ) )
        result[ "State" ]               = str( serialMsg.getU8( index + 1 ) )
        result[ "Node Address" ]        = serialMsg.getHexString_LSB( index + 2, 2 )
        result[ "Rank" ]                = str( serialMsg.getU8( index + 4 ) )
        result[ "Cell Size" ]           = str( serialMsg.getU8( index + 5 ) )
        result[ "Beacon Index" ]        = str( serialMsg.getU8( index + 6 ) )

    # decode packet received event
    def decodePacketReceivedEvent( self, serialMsg : SerialMessage, result : dict ):
        index                           = SerialMessage.EventData_Index
        result[ "RSSI" ]                = str( serialMsg.getI8( index ) - self.RSSI_Offset.Value ) + " dBm"
        result[ "SNR" ]                 = str( serialMsg.getI8( index + 1 ) ) + " dB"
        result[ "Source-EUI" ]          = serialMsg.getHexString_LSB( index + 2, 8 )
        result[ "Port" ]                = str( serialMsg.getU8( index + 10 ) )
        result[ "Payload" ]             = serialMsg.getHexString( index + 11, -1 ) 

    # dictionary with event message decoders 
    EventDecoders = {
        Msg_ID.LinkStatusChange_Ind     : { "Name" : "Link Status change event",        "Handler" : decodeLinkStatusChangeEvent },
        Msg_ID.PacketReceived_Ind       : { "Name" : "Packet received event",           "Handler" : decodePacketReceivedEvent },
    }

    def __init__( self, client, serialDevice, sap_id ):
        super().__init__( client            = client,
                          serialDevice      = serialDevice, 
                          sap_id            = sap_id, 
                          responseDecoders  = self.ResponseDecoders, 
                          statusCodes       = self.StatusCodes,
                          encoders          = self.Encoders,
                          eventDecoders     = self.EventDecoders )
