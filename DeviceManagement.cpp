/**
 * @file    DeviceManagement.cpp
 *
 * @brief   Implementation of class DeviceManagement
 *
 * @note    This example code is free software: you can redistribute it and/or modify it.
 *
 *          This program is provided by IMST GmbH on an "AS IS" basis without
 *          any warranties in the hope that it will be useful.
 */

#include "DeviceManagement.h"
#include <string>
#include <cstring>  //strstr

//#include <QDateTime>
#include "CurrentTime.h"


//<! map with status code strings
//const aMap < uint8_t, std::string > DeviceManagement::_StatusCodes = {
const aMap < uint8_t, const char* > DeviceManagement::_StatusCodes = {
    { Ok,                       "ok" },
    { Error,                    "error" },
    { CommandNotSupported,      "command not supported" },
    { WrongParameter,           "wrong parameter" },
    { WrongApplicationMode,     "wrong application mode" },
    { Reserved,                 "reserved" },
    { ApplicationBusy,          "application busy" },
    { WrongMessageLength,       "wrong message length" },
    { NVM_WriteError,           "NVM write error" },
    { NVM_ReadError,            "NVM read error" },
    { CommandRejected,          "command rejected" }
};

//<! map with module type strings
//const aMap < uint8_t, std::string > DeviceManagement::_ModuleTypes = {
const aMap < uint8_t, const char* > DeviceManagement::_ModuleTypes = {
    { 104,  "iM284A-XL" },
    { 109,  "iM891A-XL" },
    { 110,  "iU891A-XL" },
    { 163,  "iM881A-XL" }
};

//<! map with response & event names for HCI messages
//const aMap < uint8_t, std::string > DeviceManagement::_EventNames = {
const aMap < uint8_t, const char* > DeviceManagement::_EventNames = {
    { Startup_Ind,              "startup indication" },
    { Ping_Rsp,                 "ping device response" },
    { GetDeviceInfo_Rsp,        "get device info response" },
    { GetFirmwareVersion_Rsp,   "get firmware version response" },
    { GetDateTime_Rsp,          "get date time response" },
    { SetDateTime_Rsp,          "set date time response" },
    { RestartDevice_Rsp,        "restart device response" },
    { SetSystemOptions_Rsp,     "set system options response" },
    { GetSystemOptions_Rsp,     "get system options response" }
};

//<! map with message handlers for HCI messages
const aMap < uint8_t , DeviceManagement::Handler > DeviceManagement::_Handlers = {
    { Startup_Ind,              &DeviceManagement::OnStartupIndication },
    { Ping_Rsp,                 &DeviceManagement::OnDefaultResponse },
    { GetDeviceInfo_Rsp,        &DeviceManagement::OnDeviceInfoResponse },
    { GetFirmwareVersion_Rsp,   &DeviceManagement::OnFirmwareVersionResponse },
    { GetDateTime_Rsp,          &DeviceManagement::OnDateTimeResponse },
    { SetDateTime_Rsp,          &DeviceManagement::OnDefaultResponse },
    { RestartDevice_Rsp,        &DeviceManagement::OnDefaultResponse },
    { SetSystemOptions_Rsp,     &DeviceManagement::OnDefaultResponse },
    { GetSystemOptions_Rsp,     &DeviceManagement::OnSystemOptionsResponse }
};

/**
 * @brief   class constructor
 *
 * @param   port        a serial port
 */
DeviceManagement::DeviceManagement( HardwareSerial& port )
                : ServiceAccessPoint( DeviceManagement::Sap_ID, port ) {
}


/**
 * @brief   send "ping request"
 */
bool
DeviceManagement::OnPingDevice() {
    return SendMessage( Ping_Req );
}


/**
 * @brief   send "get device information request"
 */
bool
DeviceManagement::OnGetDeviceInfo() {
    return SendMessage( GetDeviceInfo_Req );
}


/**
 * @brief   send "get firmware version request"
 */
bool
DeviceManagement::OnGetFirmwareVersion() {
    return SendMessage( GetFirmwareVersion_Req );
}


/**
 * @brief   send "get date time request"
 */
bool
DeviceManagement::OnGetDateTime() {
    return SendMessage( GetDateTime_Req );
}


/**
 * @brief   send "set date time request"
 */
bool
DeviceManagement::OnSetDateTime() {
    SerialMessage msg( DeviceManagement::Sap_ID, SetDateTime_Req );

    //uint32_t  secondsSincePeriod = QDateTime::currentSecsSinceEpoch();
    uint32_t  secondsSincePeriod = getCurrentTimeInSeconds();

    msg.Append( secondsSincePeriod );

    return SendMessage( msg );
}


/**
 * @brief   send "restart device request"
 */
bool
DeviceManagement::OnRestartDevice() {
    return SendMessage( RestartDevice_Req );
}


/**
 * @brief   send "get system options request"
 */
bool
DeviceManagement::OnGetSystemOptions() {
    return SendMessage( GetSystemOptions_Req );
}


/**
 * @brief   send "set system options request"
 */
bool
DeviceManagement::OnSetSystemOptions( const Dictionary& params ) {

    SerialMessage msg( DeviceManagement::Sap_ID, SetSystemOptions_Req );

    uint32_t    mask    = 0;
    uint32_t    options = 0;

    char*       data    = nullptr;

    static const char* Trace_on     = "Trace = on";
    static const char* Trace_off    = "Trace = off";
    static const char* Start_on     = "Startup Event = on";
    static const char* Start_off    = "Startup Event = off";

    if ( ( data = (char*)params.contains("Options") ) ) {
        if ( strstr( data, Trace_on ) ) {

            mask    |= SO_Trace;
            options |= SO_Trace;

        } else if ( strstr( data, Trace_off ) ) {

            mask    |= SO_Trace;

        }
        if ( strstr( data, Start_on ) ) {

            mask    |= SO_StartupEvent;
            options |= SO_StartupEvent;

        } else if ( strstr( data, Start_off ) ) {

            mask    |= SO_StartupEvent;

        }
    }

    msg.Append( mask );
    msg.Append( options );

    return SendMessage( msg );

}


/**
 * @brief   find message handler and decode convert message into json format
 *
 * @param   serialMsg       incoming HCI message
 *
 * @param   result          decoded data in Json format
 *
 * @return  true/false
 */
bool
DeviceManagement::OnDecodeMessage( const SerialMessage& serialMsg, Dictionary& result ) {

    uint8_t msgID = serialMsg.GetMsgID();

    //find handler by message ID
    Handler handler = _Handlers.value( msgID, nullptr );
    if ( handler != nullptr ) {
        //get handler event name
        result.append("Event",
            _EventNames.value( msgID, "unknown handler name" ) );
        //call message handler
        return ( this->*handler )( serialMsg, result );
    }
    //no handler found
    
    result.append  ("Error", "unsupported MsgID: ");
    result.appendU8( msgID );
    result.append  (" received");
    return true;
}


/**
 * @brief   decode default response, HCI status only
 *
 * @param   serialMsg       incoming HCI message
 *
 * @param   result          decoded data in Json format
 *
 * @return  true/false
 */
bool
DeviceManagement::OnDefaultResponse( const SerialMessage& serialMsg, Dictionary& result ) {
    // check minimum payload length
    if ( serialMsg.GetPayloadLength() >= 1 ) {
        result.append("Status",
            _StatusCodes.value( serialMsg.GetResponseStatus(), "error" ) );
        return true;
    }
    return false;
}

/**
 * @brief   decode startup indication
 *
 * @param   serialMsg       incoming HCI message
 *
 * @param   result          decoded data in Json format
 *
 * @return  true/false
 */
bool
DeviceManagement::OnStartupIndication( const SerialMessage& serialMsg, Dictionary& result ) {
    // check minimum payload length
    if ( serialMsg.GetPayloadLength() < ( ReservedInfo_Size + DeviceInfo_Size + FirmwareInfo_MinSize ) )
        return false;

#if ReservedInfo_Size > 0
    result.append("Reserved Info",
        serialMsg.GetHexString( SerialMessage::EventData_Index, ReservedInfo_Size ) );
#endif
    result.append("Device Info",
        DecodeDeviceInfo( serialMsg, SerialMessage::EventData_Index + ReservedInfo_Size ) );
    result.append("Firmware Info",
        DecodeFirmwareInfo( serialMsg, SerialMessage::EventData_Index + ReservedInfo_Size + DeviceInfo_Size ) );

    return true;
}


/**
 * @brief   decode get device information response
 *
 * @param   serialMsg       incoming HCI message
 *
 * @param   result          decoded data in Json format
 *
 * @return  true/false
 */
bool
DeviceManagement::OnDeviceInfoResponse( const SerialMessage& serialMsg, Dictionary& result ) {
    // check minimum response payload length
    if ( serialMsg.GetResponsePayloadLength() < DeviceInfo_Size )
        return false;

    uint8_t status = serialMsg.GetResponseStatus();
    result.append("Status", _StatusCodes.value( status, "error" ) );

    if ( Ok == status ) {
        result.append("Device Info",
            DecodeDeviceInfo( serialMsg, SerialMessage::ResponseData_Index ) );
    }
    return true;
}


/**
 * @brief   decode Device Info Field
 *
 * @param   serialMsg       incoming HCI message
 * @param   index           index to device info field
 *
 * @return  Dictionary including decoded data
 */
Dictionary
DeviceManagement::DecodeDeviceInfo( const SerialMessage& serialMsg, int index ) const {
    Dictionary info;
    uint8_t moduleType      =   serialMsg.GetU8( index );
    //info.append("Module Type",
    //    _ModuleTypes.value( moduleType, "unknown module type:" + std::to_string( moduleType ) ) );
    info.append("Module Type",
        _ModuleTypes.value( moduleType, "unknown module type: " ) );
    info.appendU8( moduleType );
    //info.append("Module ID",    std::to_string( serialMsg.GetU32( index + 1 ) ) );
    info.append("Module ID",    serialMsg.GetU32( index + 1 ) );
    info.append("Product Type", serialMsg.GetHexString( index + 5, 4 ) );
    info.append("Product ID",   serialMsg.GetHexString( index + 9, 4 ) );

    return info;
}


/**
 * @brief   decode get firmware version response
 *
 * @param   serialMsg       incoming HCI message
 *
 * @param   result          decoded data in Json format
 *
 * @return  true/false
 */
bool
DeviceManagement::OnFirmwareVersionResponse( const SerialMessage& serialMsg, Dictionary& result ) {
    // check minimum payload length
    if ( serialMsg.GetResponsePayloadLength() < FirmwareInfo_MinSize )
        return false;

    uint8_t status                  =   serialMsg.GetResponseStatus();
    result.append("Status", _StatusCodes.value( status, "error" ) );

    if ( Ok == status ) {
        result.append("Firmware Info",
            DecodeFirmwareInfo( serialMsg, SerialMessage::ResponseData_Index ) );
    }
    return true;
}


/**
 * @brief   decode Firmware Info Field
 *
 * @param   serialMsg       incoming HCI message
 * @param   index           index to firmware info field
 *
 * @return  Dictionary including decoded data
 */
Dictionary
DeviceManagement::DecodeFirmwareInfo( const SerialMessage& serialMsg, int index ) const {
    Dictionary info;
    info.append("Version",
        std::to_string( serialMsg.GetU8( index + 1 ) ) + 
            "." + std::to_string( serialMsg.GetU8( index ) ) );
    info.append("Build Count",
        std::to_string( serialMsg.GetU16( index + 2 ) ) );
    info.append("Build Date", 
        serialMsg.GetData( index + 4 ), 10 );
    info.append("Firmware Name",
        serialMsg.GetData( index + 14 ) );

    return info;
}


/**
 * @brief   decode get date time response
 *
 * @param   serialMsg       incoming HCI message
 *
 * @param   result          decoded data in Json format
 *
 * @return  true/false
 */
bool
DeviceManagement::OnDateTimeResponse( const SerialMessage& serialMsg, Dictionary& result ) {
    // check minimum payload length
    if ( serialMsg.GetResponsePayloadLength() < ( 4 ) )
        return false;

    uint8_t status                      =   serialMsg.GetResponseStatus();
    result.append("Status",             _StatusCodes.value( status, "error" ) );

    if ( Ok == status ) {
        //Dictionary info;
        
        result.append(
            "Date Time Info.Seconds since epoch",
            std::to_string( serialMsg.GetU32( SerialMessage::ResponseData_Index ) ) );
        result.append(
            "Date Time Info.Date Time",
            serialMsg.GetDateTime( SerialMessage::ResponseData_Index, "dd-MM-yyyy hh:mm:ss" ) );
        //result[ "Date Time Info" ]      =   info;

    }
    return true;
}

/**
 * @brief   decode get system options response
 *
 * @param   serialMsg       incoming HCI message
 *
 * @param   result          decoded data in Json format
 *
 * @return  true/false
 */

bool
DeviceManagement::OnSystemOptionsResponse( const SerialMessage& serialMsg, Dictionary& result ) {
    //check minimum payload length sID+mID+CRC16
    if ( serialMsg.GetResponsePayloadLength() < ( 4 ) )
        return false;

    uint8_t status                      =   serialMsg.GetResponseStatus();
    result.append( "Status", _StatusCodes.value( status, "error" ) );

    if ( Ok == status ) {

        uint32_t options                =   serialMsg.GetU32( SerialMessage::ResponseData_Index );

        result.append("System Options.Options.APS",
            ( options & SO_APS )            ? "on" : "off");
        result.append("System Options.Options.Trace",
            ( options & SO_Trace )          ? "on" : "off");
        result.append("System Options.Options.RTC",
            ( options & SO_RTC )            ? "on" : "off");
        result.append("System Options.Options.WatchDog",
            ( options & SO_WatchDog )       ? "on" : "off");
        result.append("System Options.Options.Startup Event",
            ( options & SO_StartupEvent )   ? "on" : "off");

    }
    return true;
}
