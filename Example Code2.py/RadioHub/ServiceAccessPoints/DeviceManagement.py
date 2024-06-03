'''
 @file    DeviceManagement.py
 
 @brief   Implementation of DeviceManagement message encoders and decoders
 
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

class DeviceManagement( ServiceAccessPoint ):
    # message identifier of DeviceManegemnt SAP
    class Msg_ID( IntEnum ):
        Startup_Ind                     =   0x00
        PingDevice_Req                  =   0x01
        PingDevice_Rsp                  =   0x02
        GetDeviceInfo_Req               =   0x03
        GetDeviceInfo_Rsp               =   0x04
        GetFirmwareInfo_Req             =   0x05
        GetFirmwareInfo_Rsp             =   0x06
        RestartDevice_Req               =   0x07
        RestartDevice_Rsp               =   0x08
        SetDateTime_Req                 =   0x0D
        SetDateTime_Rsp                 =   0x0E
        GetDateTime_Req                 =   0x0F
        GetDateTime_Rsp                 =   0x10
        SetSystemOptions_Req            =   0xF7
        SetSystemOptions_Rsp            =   0xF8
        GetSystemOptions_Req            =   0xF9
        GetSystemOptions_Rsp            =   0xFA

    # status codes of DeviceManagement SAP
    class Status_ID( IntEnum ):
        Ok                              =   0x00
        Error                           =   0x01
        CommandNotSupported             =   0x02
        WrongParameter                  =   0x03
        WrongApplicationMode            =   0x04
        Reserved                        =   0x05
        ApplicationBusy                 =   0x06
        WrongMessageLength              =   0x07
        NVM_WriteError                  =   0x08
        NVM_ReadError                   =   0x09
        CommandRejected                 =   0x0A

    # dictionary with status codes
    StatusCodes = {
        Status_ID.Ok                    :   "ok",
        Status_ID.Error                 :   "error",
        Status_ID.CommandNotSupported   :   "command not supported",
        Status_ID.WrongParameter        :   "wrong parameter",
        Status_ID.WrongApplicationMode  :   "wrong application mode",
        Status_ID.Reserved              :   "reserved",
        Status_ID.ApplicationBusy       :   "application busy",
        Status_ID.WrongMessageLength    :   "wrong message length",
        Status_ID.NVM_WriteError        :   "NVM write error",
        Status_ID.NVM_ReadError         :   "NVM read error",
        Status_ID.CommandRejected       :   "Command rejected",
    }

    # module IDs for IMST's radio modules
    class Module_ID( IntEnum ):
        iM284A_XL                       =   104
        iM891A_XL                       =   109
        iU891A_XL                       =   110
        iM881A_XL                       =   163

    ModuleTypes = {
        Module_ID.iM284A_XL             :   "iM284A-XL",
        Module_ID.iM881A_XL             :   "iM881A-XL",
        Module_ID.iM891A_XL             :   "iM891A-XL",
        Module_ID.iU891A_XL             :   "iU891A-XL",
    }

    # system options
    class SystemOptions ( IntEnum ):
        APS                             =   0x01
        Trace                           =   0x02
        RTC                             =   0x04
        WatchDog                        =   0x08
        StartupEvent                    =   0x10    

    # send a simple request
    def defaultRequest( self, msg_id, params ):
        super().sendRequest( msg_id = msg_id )

    # send encoded date time
    def setDateTimeRequest( self, msg_id, params ):
        payload = struct.pack('<I', params[ "SecondsSinceEpoch" ] )  
        super().sendRequest( msg_id = msg_id, payload = payload )

    # send system options
    def setSystemOptions( self, msg_id, params ):

        optList = params[ "Options" ].split( ", " )

        mask = 0
        options = 0

        # APS
        if "APS = on" in optList:
            options = options + self.SystemOptions.APS
            mask    = mask    + self.SystemOptions.APS
        if "APS = off" in optList:
            mask = mask + self.SystemOptions.APS
        
        # Trace        
        if "Trace = on" in optList:
            options = options + self.SystemOptions.Trace
            mask    = mask    + self.SystemOptions.Trace
        if "Trace = off" in optList:
            mask = mask + self.SystemOptions.Trace

        # RTC
        if "RTC = on" in optList:
            options = options + self.SystemOptions.RTC
            mask    = mask    + self.SystemOptions.RTC
        if "RTC = off" in optList:
            mask = mask + self.SystemOptions.RTC

        # WatchDog
        if "WatchDog = on" in optList:
            options = options + self.SystemOptions.WatchDog
            mask    = mask    + self.SystemOptions.WatchDog
        if "WatchDog = off" in optList:
            mask = mask + self.SystemOptions.WatchDog

        # StartupEvent
        if "StartupEvent = on" in optList:
            options = options + self.SystemOptions.StartupEvent
            mask    = mask    + self.SystemOptions.StartupEvent
        if "StartupEvent = off" in optList:
            mask = mask + self.SystemOptions.StartupEvent

        payload = struct.pack('<I', mask ) + struct.pack('<I', options )  

        super().sendRequest( msg_id = msg_id, payload = payload )

    # dictinary with message encoders
    Encoders = {
        "pingDevice"            :  { "ID" : Msg_ID.PingDevice_Req ,         "Handler" : defaultRequest },
        "getDeviceInfo"         :  { "ID" : Msg_ID.GetDeviceInfo_Req ,      "Handler" : defaultRequest },
        "getFirmwareInfo"       :  { "ID" : Msg_ID.GetFirmwareInfo_Req ,    "Handler" : defaultRequest },
        "getDateTime"           :  { "ID" : Msg_ID.GetDateTime_Req ,        "Handler" : defaultRequest },
        "setDateTime"           :  { "ID" : Msg_ID.SetDateTime_Req ,        "Handler" : setDateTimeRequest },
        "restartDevice"         :  { "ID" : Msg_ID.RestartDevice_Req ,      "Handler" : defaultRequest },
        "setSystemOptions"      :  { "ID" : Msg_ID.SetSystemOptions_Req ,   "Handler" : setSystemOptions },
        "getSystemOptions"      :  { "ID" : Msg_ID.GetSystemOptions_Req ,   "Handler" : defaultRequest },
    }    

    # default response decoder -> call super
    def decodeDefaultResponse( self, serialMsg : SerialMessage, result : dict ):
        super().decodeDefaultResponse( serialMsg, result = result )

    # decode device info response
    def decodeDeviceInfoResponse( self, serialMsg : SerialMessage, result : dict ):
        self.decodeDefaultResponse( serialMsg, result )
        index                       = SerialMessage.ResponseData_Index
        moduleType                  = serialMsg.getU8( index )
        result[ "Module Type" ]     = self.ModuleTypes.get( moduleType, "unknown module type:" + str( moduleType ) )
        result[ "Module ID" ]       = str(  serialMsg.getU32( index + 1 ) )
        result[ "Product Type" ]    = serialMsg.getHexString( index + 5, 4 )
        result[ "Product ID" ]      = serialMsg.getHexString( index + 9, 4 )

    # decode firmware version response
    def decodeFirmwareVersionResponse( self, serialMsg : SerialMessage, result : dict ):
        self.decodeDefaultResponse( serialMsg, result )
        index                       = SerialMessage.ResponseData_Index
        result[ "Version" ]         = str( serialMsg.getU8( index + 1 ) ) + '.' + str( serialMsg.getU8( index ) )
        result[ "Build Count" ]     = str( serialMsg.getU16( index + 2 ) )
        result[ "Build Date" ]      = serialMsg.getString( index + 4, 10 )
        result[ "Firmware Name" ]   = serialMsg.getString( index + 14, -1 )

    # decode get date and time response
    def decodeDateTimeResponse( self, serialMsg : SerialMessage, result : dict ):
        self.decodeDefaultResponse( serialMsg, result )
        index                       = SerialMessage.ResponseData_Index
        dateTime                    = serialMsg.getDateTime( index )
        result[ "Date Time" ]       = time.strftime( '%d.%m.%Y %H:%M:%S', dateTime )

    def decodeSystemOptionsResponse( self, serialMsg : SerialMessage, result : dict ):
        self.decodeDefaultResponse( serialMsg, result )
        options                           =   serialMsg.getU32( SerialMessage.ResponseData_Index )
        result[ "Options.APS" ]           =   "on"  if ( options & self.SystemOptions.APS ) else "off"
        result[ "Options.Trace" ]         =   "on"  if ( options & self.SystemOptions.Trace ) else "off"
        result[ "Options.RTC" ]           =   "on"  if ( options & self.SystemOptions.RTC ) else "off"
        result[ "Options.WatchDog" ]      =   "on"  if ( options & self.SystemOptions.WatchDog ) else "off"
        result[ "Options.StartupEvent" ]  =   "on"  if ( options & self.SystemOptions.StartupEvent ) else "off"

    # dictionary with response message decoders 
    ResponseDecoders = {
        Msg_ID.PingDevice_Rsp       : { "Name" : "pingDevice response",         "Handler" : decodeDefaultResponse },
        Msg_ID.GetDeviceInfo_Rsp    : { "Name" : "getDeviceInfo response",      "Handler" : decodeDeviceInfoResponse },
        Msg_ID.GetFirmwareInfo_Rsp  : { "Name" : "geFirmwareVersion response",  "Handler" : decodeFirmwareVersionResponse },
        Msg_ID.GetDateTime_Rsp      : { "Name" : "getDateTime response",        "Handler" : decodeDateTimeResponse },
        Msg_ID.SetDateTime_Rsp      : { "Name" : "setDateTime response",        "Handler" : decodeDefaultResponse },
        Msg_ID.RestartDevice_Rsp    : { "Name" : "restartDevice response",      "Handler" : decodeDefaultResponse },
        Msg_ID.GetSystemOptions_Rsp : { "Name" : "getSystemOptions response",   "Handler" : decodeSystemOptionsResponse },  
        Msg_ID.SetSystemOptions_Rsp : { "Name" : "setSystemOptions response",   "Handler" : decodeDefaultResponse },
    }

    # decode startup indication
    def decodeStartupIndication( self, serialMsg : SerialMessage, result : dict ):
        index                       = SerialMessage.EventData_Index

        #result[ "Reserved" ]        = serialMsg.getHexString( index, 4 )
        
        result[ "Module Type" ]     = self.ModuleTypes.get( serialMsg.getU8( index + 0 ), "unknown module type" )
        result[ "Module ID" ]       = str(  serialMsg.getU32( index + 1 ) )
        result[ "Product Type" ]    = serialMsg.getHexString( index + 5, 4 )
        result[ "Product ID" ]      = serialMsg.getHexString( index + 9, 4 )

        result[ "Version" ]         = str( serialMsg.getU8( index + 14 ) ) + '.' + str( serialMsg.getU8( index + 13 ) )
        result[ "Build Count" ]     = str( serialMsg.getU16( index + 15 ) )
        result[ "Build Date" ]      = serialMsg.getString( index + 17, 10 )
        result[ "Firmware Name" ]   = serialMsg.getString( index + 27, -1 )


    # dictionary with event message decoders 
    EventDecoders = {
        Msg_ID.Startup_Ind          : { "Name" : "startup indication",          "Handler" : decodeStartupIndication },
    }

    def __init__( self, client, serialDevice, sap_id ):
        super().__init__( client            = client,
                          serialDevice      = serialDevice, 
                          sap_id            = sap_id, 
                          responseDecoders  = self.ResponseDecoders, 
                          statusCodes       = self.StatusCodes,
                          encoders          = self.Encoders,
                          eventDecoders     = self.EventDecoders )
