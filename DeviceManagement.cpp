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

//#include <QDateTime>

//<! map with status code strings
const QMap < uint8_t, std::string > DeviceManagement::_StatusCodes =
{
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
const QMap < uint8_t, std::string > DeviceManagement::_ModuleTypes =
{
    { 104,  "iM284A-XL" },
    { 109,  "iM891A-XL" },
    { 110,  "iU891A-XL" },
    { 163,  "iM881A-XL" }
};

//<! map with response & event names for HCI messages
const QMap < uint8_t , std::string > DeviceManagement::_EventNames =
{
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
const QMap < uint8_t , DeviceManagement::Handler > DeviceManagement::_Handlers =
{
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

    uint32_t  secondsSincePeriod = QDateTime::currentSecsSinceEpoch();

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

    QStringList optList = params[ "Options" ].toString().split( ", " );

    uint32_t mask    = 0;
    uint32_t options = 0;

    if ( optList.contains( "Trace = on" ) )
    {
        mask    |= SO_Trace;
        options |= SO_Trace;
    }
    else if ( optList.contains( "Trace = off" ) )
    {
        mask    |= SO_Trace;
    }

    if ( optList.contains( "Startup Event = on" ) )
    {
        mask    |= SO_StartupEvent;
        options |= SO_StartupEvent;
    }
    else if ( optList.contains( "Startup Event = off" ) )
    {
        mask    |= SO_StartupEvent;
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
        result.append( "Event", _EventNames.value( msgID, "unknown handler name" ) );
        //call message handler
        return ( this->*handler )( serialMsg, result );
    }
    //no handler found
    std::string 
    result.append( "Error", "unsupported MsgID: " + std::string::number( msgID ) + " received");
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
    if ( serialMsg.GetPayloadLength() >= 1 )
    {
        result[ "Status" ] = _StatusCodes.value( serialMsg.GetResponseStatus(), "error" );

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
DeviceManagement::OnStartupIndication( const SerialMessage& serialMsg, Dictionary& result )
{
    // check minimum payload length
    if ( serialMsg.GetPayloadLength() < ( ReservedInfo_Size + DeviceInfo_Size + FirmwareInfo_MinSize ) )
        return false;

#if ReservedInfo_Size > 0
    result[ "Reserved Info" ]   = serialMsg.GetHexString( SerialMessage::EventData_Index, ReservedInfo_Size );
#endif

    result[ "Device Info" ]     = DecodeDeviceInfo( serialMsg, SerialMessage::EventData_Index + ReservedInfo_Size );
    result[ "Firmware Info" ]   = DecodeFirmwareInfo( serialMsg, SerialMessage::EventData_Index + ReservedInfo_Size + DeviceInfo_Size );

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
DeviceManagement::OnDeviceInfoResponse( const SerialMessage& serialMsg, Dictionary& result )
{
    // check minimum response payload length
    if ( serialMsg.GetResponsePayloadLength() < DeviceInfo_Size )
        return false;

    uint8_t status = serialMsg.GetResponseStatus();
    result[ "Status" ] = _StatusCodes.value( status, "error" );

    if ( status == Ok )
    {
        result[ "Device Info" ] = DecodeDeviceInfo( serialMsg, SerialMessage::ResponseData_Index );
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
DeviceManagement::DecodeDeviceInfo( const SerialMessage& serialMsg, int index ) const
{
    Dictionary info;
    uint8_t moduleType      =   serialMsg.GetU8( index );
    info[ "Module Type" ]   =   _ModuleTypes.value( moduleType, "unknown module type:" + std::string::number( moduleType ) );
    info[ "Module ID" ]     =   std::string::number( serialMsg.GetU32( index + 1 ) );
    info[ "Product Type" ]  =   serialMsg.GetHexString( index + 5, 4 );
    info[ "Product ID" ]    =   serialMsg.GetHexString( index + 9, 4 );

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
DeviceManagement::OnFirmwareVersionResponse( const SerialMessage& serialMsg, Dictionary& result )
{
    // check minimum payload length
    if ( serialMsg.GetResponsePayloadLength() < FirmwareInfo_MinSize )
        return false;

    uint8_t status                  =   serialMsg.GetResponseStatus();
    result[ "Status" ]              =   _StatusCodes.value( status, "error" );

    if ( status == Ok )
    {
        result[ "Firmware Info" ]   =   DecodeFirmwareInfo( serialMsg, SerialMessage::ResponseData_Index );
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
DeviceManagement::DecodeFirmwareInfo( const SerialMessage& serialMsg, int index ) const
{
    Dictionary info;
    info[ "Version" ]           =   std::string::number( serialMsg.GetU8( index + 1 ) ) + "." + std::string::number( serialMsg.GetU8( index ) );
    info[ "Build Count" ]       =   std::string::number( serialMsg.GetU16( index + 2 ) );
    info[ "Build Date" ]        =   std::string( serialMsg.GetPayload( index + 4, 10 ) );
    info[ "Firmware Name" ]     =   std::string( serialMsg.GetPayload( index + 14 ) );

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
DeviceManagement::OnDateTimeResponse( const SerialMessage& serialMsg, Dictionary& result )
{
    // check minimum payload length
    if ( serialMsg.GetResponsePayloadLength() < ( 4 ) )
        return false;

    uint8_t status                      =   serialMsg.GetResponseStatus();
    result[ "Status" ]                  =   _StatusCodes.value( status, "error" );

    if ( status == Ok )
    {
        Dictionary info;
        info[ "Seconds since epoch" ]   =   std::string::number( serialMsg.GetU32( SerialMessage::ResponseData_Index ) );
        info[ "Date Time" ]             =   serialMsg.GetDateTime( SerialMessage::ResponseData_Index, "dd-MM-yyyy hh:mm:ss" );

        result[ "Date Time Info" ]      =   info;
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
DeviceManagement::OnSystemOptionsResponse( const SerialMessage& serialMsg, Dictionary& result )
{
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
