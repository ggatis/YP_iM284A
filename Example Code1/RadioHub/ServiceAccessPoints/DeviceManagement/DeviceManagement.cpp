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
#include <QDateTime>

//<! map with status code strings
const QMap < uint8_t, QString > DeviceManagement::_StatusCodes =
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
const QMap < uint8_t, QString > DeviceManagement::_ModuleTypes =
{
    { 103,  "iC281A" },
    { 104,  "iM284A-XL" },
    { 163,  "iM881A-XL" }
};

//<! map with event names for HCI messages
const QMap < uint8_t , QString > DeviceManagement::_EventNames =
{
    { Ping_Rsp,                 "ping device response" },
    { GetDeviceInfo_Rsp,        "get device info response" },
    { GetFirmwareVersion_Rsp,   "get firmware version response" },
    { GetDateTime_Rsp,          "get date time response" },
    { SetDateTime_Rsp,          "set date time response" },
    { RestartDevice_Rsp,        "restart device response" }
};

//<! map with message handlers for HCI messages
const QMap < uint8_t , DeviceManagement::Handler > DeviceManagement::_Handlers =
{
    { Ping_Rsp,                 &DeviceManagement::OnDefaultResponse },
    { GetDeviceInfo_Rsp,        &DeviceManagement::OnGetDeviceInfoResponse },
    { GetFirmwareVersion_Rsp,   &DeviceManagement::OnGetFirmwareVersionResponse },
    { GetDateTime_Rsp,          &DeviceManagement::OnGetDateTimeResponse },
    { SetDateTime_Rsp,          &DeviceManagement::OnDefaultResponse },
    { RestartDevice_Rsp,        &DeviceManagement::OnDefaultResponse }
};

/**
 * @brief   class constructor
 *
 * @param   port        a serial port
 */
DeviceManagement::DeviceManagement( QSerialPort& port )
                : ServiceAccessPoint( DeviceManagement_Sap, port )
{
}

/**
 * @brief   send "ping request"
 */
bool
DeviceManagement::OnPingDevice()
{
    return SendMessage( Ping_Req );
}

/**
 * @brief   send "get device information request"
 */
bool
DeviceManagement::OnGetDeviceInfo()
{
    return SendMessage( GetDeviceInfo_Req );
}

/**
 * @brief   send "get firmware version request"
 */
bool
DeviceManagement::OnGetFirmwareVersion()
{
    return SendMessage( GetFirmwareVersion_Req );
}

/**
 * @brief   send "get date time request"
 */
bool
DeviceManagement::OnGetDateTime()
{
    return SendMessage( GetDateTime_Req );
}

/**
 * @brief   send "set date time request"
 */
bool
DeviceManagement::OnSetDateTime()
{
    SerialMessage msg( DeviceManagement_Sap, SetDateTime_Req );

    uint32_t  secondsSincePeriod = QDateTime::currentSecsSinceEpoch();

    msg.Append( secondsSincePeriod );

    return SendMessage( msg );
}

/**
 * @brief   send "restart device request"
 */
bool
DeviceManagement::OnRestartDevice()
{
    return SendMessage( RestartDevice_Req );
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
DeviceManagement::OnDecodeMessage( const SerialMessage& serialMsg, QJsonObject& result )
{
    uint8_t msgID = serialMsg.GetMsgID();

    // find handler by message ID
    Handler handler = _Handlers.value( msgID, nullptr );
    if ( handler != nullptr )
    {
        // get handler event name
        result[ "Event" ] = QString( _EventNames.value( msgID, "unknown handler" ) );

        // call message handler
        return (this->*handler)( serialMsg, result );
    }
    // nothing decoced
    return false;
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
DeviceManagement::OnDefaultResponse( const SerialMessage& serialMsg, QJsonObject& result )
{
    // check minimum payload length
    if ( serialMsg.GetPayloadLength() >= 1 )
    {
        result[ "Status" ] = _StatusCodes.value( serialMsg.GetResponseStatus(), "error" );

        return true;
    }
    return false;
}

/**
 * @brief   decode device information response
 *
 * @param   serialMsg       incoming HCI message
 *
 * @param   result          decoded data in Json format
 *
 * @return  true/false
 */
bool
DeviceManagement::OnGetDeviceInfoResponse( const SerialMessage& serialMsg, QJsonObject& result )
{
    // check minimum payload length
    if ( serialMsg.GetPayloadLength() >= ( 1 + 1 + 4 ) )
    {
        uint8_t status = serialMsg.GetResponseStatus();

        result[ "Status" ] = _StatusCodes.value( status, "error" );

        if ( status == Ok )
        {
            int index               =   SerialMessage::Status_Index + 1;
            uint8_t moduleType      =   serialMsg.GetU8( index++ );
            uint32_t moduleID       =   serialMsg.GetU32( index ); index += 4;

            QJsonObject info;
            info[ "Module Type" ]   =   _ModuleTypes.value( moduleType, "unknown module type" );
            info[ "Module ID" ]     =   QString::number( moduleID, 16 );

            result[ "Device Info" ] =   info;
        }
        return true;
    }
    return false;
}

/**
 * @brief   decode firmware version response
 *
 * @param   serialMsg       incoming HCI message
 *
 * @param   result          decoded data in Json format
 *
 * @return  true/false
 */
bool
DeviceManagement::OnGetFirmwareVersionResponse( const SerialMessage& serialMsg, QJsonObject& result )
{
    // check minimum payload length
    if ( serialMsg.GetPayloadLength() >= ( 1 + 1 + 1 + 2 + 10 ) )
    {
        uint8_t status = serialMsg.GetResponseStatus();

        result[ "Status" ] = _StatusCodes.value( status, "error" );

        if ( status == Ok )
        {
            int index               =   SerialMessage::Status_Index + 1;
            uint8_t  minorVersion   =   serialMsg.GetU8( index++ );
            uint8_t  majorVersion   =   serialMsg.GetU8( index++ );
            uint16_t  buildCount    =   serialMsg.GetU16( index ); index += 2;
            QByteArray buildDate    =   serialMsg.GetPayload( index, 10 ); index += 10;
            QByteArray name         =   serialMsg.GetPayload( index );

            QJsonObject info;
            info[ "Version" ]       =   QString::number( majorVersion ) + "." + QString::number( minorVersion );
            info[ "Build Count" ]   =   QString::number ( buildCount );
            info[ "Build Date" ]    =   QString( buildDate );
            info[ "Firmware Name" ] =   QString( name );

            result[ "Firmware Info" ] = info;
        }
        return true;
    }
    return false;
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
DeviceManagement::OnGetDateTimeResponse( const SerialMessage& serialMsg, QJsonObject& result )
{
    // check minimum payload length
    if ( serialMsg.GetPayloadLength() >= ( 1 + 4 ) )
    {
        result[ "Status" ]      =   _StatusCodes.value( serialMsg.GetResponseStatus(), "error" );

        uint32_t seconds        =   serialMsg.GetU32( SerialMessage::Status_Index + 1 );

        QJsonObject info;
        info[ "Seconds since epoch" ] =   QString::number( seconds );

        QDateTime dt;
        dt.setSecsSinceEpoch( seconds );
        dt.setTimeSpec( Qt::LocalTime );

        info[ "Date Time" ]     =   dt.toString( "dd-MM-yyyy hh:mm:ss" );

        result[ "Date Time Info" ] =    info;

        return true;
    }
    return false;
}
