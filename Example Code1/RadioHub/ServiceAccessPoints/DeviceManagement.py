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
        PingDevice_Req          =   0x01
        PingDevice_Rsp          =   0x02
        GetDeviceInfo_Req       =   0x03
        GetDeviceInfo_Rsp       =   0x04
        GetFirmwareInfo_Req     =   0x05
        GetFirmwareInfo_Rsp     =   0x06
        RestartDevice_Req       =   0x07
        RestartDevice_Rsp       =   0x08
        SetDateTime_Req         =   0x0D
        SetDateTime_Rsp         =   0x0E
        GetDateTime_Req         =   0x0F
        GetDateTime_Rsp         =   0x10

    # status codes of DeviceManagement SAP
    class Status_ID( IntEnum ):
        Ok                      =   0x00
        Error                   =   0x01
        CommandNotSupported     =   0x02
        WrongParameter          =   0x03
        WrongApplicationMode    =   0x04
        Reserved                =   0x05
        ApplicationBusy         =   0x06
        WrongMessageLength      =   0x07
        NVM_WriteError          =   0x08
        NVM_ReadError           =   0x09
        CommandRejected         =   0x0A

    class Module_ID( IntEnum ):
        iC281A                  =   103
        iM284A_XL               =   104
        iM881A_XL               =   163

    # send a simple request
    def defaultRequest( self, msg_id ):
        super().sendRequest( msg_id = msg_id )

    # send encoded date time
    def setDateTimeRequest( self, msg_id ):
        secondsSinceEpoch = int( datetime.now( timezone.utc ).timestamp() )
        payload = struct.pack('<I', secondsSinceEpoch )  
        super().sendRequest( msg_id = msg_id, payload = payload )

    # dictinary with message encoders
    Encoders = {
        "pingDevice"            :  { "ID" : Msg_ID.PingDevice_Req ,         "handler" : defaultRequest },
        "getDeviceInfo"         :  { "ID" : Msg_ID.GetDeviceInfo_Req ,      "handler" : defaultRequest },
        "getFirmwareInfo"       :  { "ID" : Msg_ID.GetFirmwareInfo_Req ,    "handler" : defaultRequest },
        "getDateTime"           :  { "ID" : Msg_ID.GetDateTime_Req ,        "handler" : defaultRequest },
        "setDateTime"           :  { "ID" : Msg_ID.SetDateTime_Req ,        "handler" : setDateTimeRequest },
        "restartDevice"         :  { "ID" : Msg_ID.RestartDevice_Req ,      "handler" : defaultRequest },
    }    

    ModuleTypes = {
        Module_ID.iC281A    :   "iC281A",
        Module_ID.iM284A_XL :   "iM284A-XL",
        Module_ID.iM881A_XL :   "iM881A-XL",
    }

    # default response decoder -> call super
    def decodeDefaultResponse( self, serialMsg : SerialMessage, result : dict ):
        super().decodeDefaultResponse( serialMsg, result = result )

    # decode date and time response
    def decodeDeviceInfoResponse( self, serialMsg : SerialMessage, result : dict ):
        self.decodeDefaultResponse( serialMsg, result )
        index = SerialMessage.Status_Index + 1
        moduleType = serialMsg.getU8( index )
        moduleID = serialMsg.getU32( index + 1 )
        result[ "module type" ] = self.ModuleTypes.get( moduleType, "unknown module type" )
        result[ "module ID" ] = hex( moduleID )

    # decode firmware version response
    def decodeFirmwareVersionResponse( self, serialMsg : SerialMessage, result : dict ):
        self.decodeDefaultResponse( serialMsg, result )
        index = SerialMessage.Status_Index + 1
        minor = serialMsg.getU8( index )
        major = serialMsg.getU8( index + 1 )
        result[ "version" ]         = str( major ) + '.' + str( minor )
        result[ "build count" ]     = str( serialMsg.getU16( index + 2 ) )
        result[ "build date" ]      = serialMsg.getString( index + 4, 10 )
        result[ "firmware name" ]   = serialMsg.getString( index + 14, -1 )

    # decode get date and time response
    def decodeDateTimeResponse( self, serialMsg : SerialMessage, result : dict ):
        self.decodeDefaultResponse( serialMsg, result )
        index = SerialMessage.Status_Index + 1
        dateTime = serialMsg.getDateTime( index )
        result[ "date time" ] =  time.strftime( '%d.%m.%Y %H:%M:%S', dateTime )

    # dictinary with message decoders 
    ResponseDecoders = {
        Msg_ID.PingDevice_Rsp       : { "name" : "pingDevice response",         "handler" : decodeDefaultResponse },
        Msg_ID.GetDeviceInfo_Rsp    : { "name" : "getDeviceInfo response",      "handler" : decodeDeviceInfoResponse },
        Msg_ID.GetFirmwareInfo_Rsp  : { "name" : "getFirmwareVersion response", "handler" : decodeFirmwareVersionResponse },
        Msg_ID.GetDateTime_Rsp      : { "name" : "getDateTime response",        "handler" : decodeDateTimeResponse },
        Msg_ID.SetDateTime_Rsp      : { "name" : "setDateTime response",        "handler" : decodeDefaultResponse },
        Msg_ID.RestartDevice_Rsp    : { "name" : "restartDevice response",      "handler" : decodeDefaultResponse },
    }

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

    def __init__( self, client, serialDevice, sap_id ):
        super().__init__( client            = client,
                          serialDevice      = serialDevice, 
                          sap_id            = sap_id, 
                          responseDecoders  = self.ResponseDecoders, 
                          statusCodes       = self.StatusCodes,
                          encoders          = self.Encoders )
