/**
 * @file    DeviceManagement.h
 *
 * @brief   Declaration of class DeviceManagement
 *
 * @note    This example code is free software: you can redistribute it and/or modify it.
 *
 *          This program is provided by IMST GmbH on an "AS IS" basis without
 *          any warranties in the hope that it will be useful.
 */

#ifndef __DeviceManagement_H__
#define __DeviceManagement_H__


#include "RadioHub/ServiceAccessPoints/ServiceAccessPoint.h"

#include <QJsonObject>
#include <QMap>

//<! ServiceaccessPoint class for DeviceManagement HCI messages
class DeviceManagement : public ServiceAccessPoint
{
public:

    enum Sap_Identifier
    {
        DeviceManagement_Sap    =   0x01
    };

    enum Message_Identifier
    {
        Ping_Req                =   0x01,
        Ping_Rsp                =   0x02,

        GetDeviceInfo_Req       =   0x03,
        GetDeviceInfo_Rsp       =   0x04,

        GetFirmwareVersion_Req  =   0x05,
        GetFirmwareVersion_Rsp  =   0x06,

        RestartDevice_Req       =   0x07,
        RestartDevice_Rsp       =   0x08,

        SetDateTime_Req         =   0x0D,
        SetDateTime_Rsp         =   0x0E,
        GetDateTime_Req         =   0x0F,
        GetDateTime_Rsp         =   0x10
    };

    enum Status_Codes
    {
        Ok                      =   0x00,
        Error                   =   0x01,
        CommandNotSupported     =   0x02,
        WrongParameter          =   0x03,
        WrongApplicationMode    =   0x04,
        Reserved                =   0x05,
        ApplicationBusy         =   0x06,
        WrongMessageLength      =   0x07,
        NVM_WriteError          =   0x08,
        NVM_ReadError           =   0x09,
        CommandRejected         =   0x0A
    };


                                        DeviceManagement( QSerialPort& port );

    /**
     * @brief   send ping request
     */
    bool                                OnPingDevice();

    /**
     * @brief   send "get device information"
     */
    bool                                OnGetDeviceInfo();

    /**
     * @brief   send "get firmware version"
     */
    bool                                OnGetFirmwareVersion();

    /**
     * @brief   send "get date time"
     */
    bool                                OnGetDateTime();

    /**
     * @brief   send "set date time"
     */
    bool                                OnSetDateTime();


    /**
     * @brief   send "restart device"
     */
    bool                                OnRestartDevice();

private:

    /**
     * @brief   decoder interface for incoming messages
     */
    bool                                OnDecodeMessage             ( const SerialMessage& serialMsg, QJsonObject& result ) override;

    /**
     * @brief   message decoder
     */
    bool                                OnDefaultResponse           ( const SerialMessage& serialMsg, QJsonObject& result );
    bool                                OnGetDeviceInfoResponse     ( const SerialMessage& serialMsg, QJsonObject& result );
    bool                                OnGetFirmwareVersionResponse( const SerialMessage& serialMsg, QJsonObject& result );
    bool                                OnGetDateTimeResponse       ( const SerialMessage& serialMsg, QJsonObject& result );

private:

    //<! message decoder prototype
    typedef bool (DeviceManagement::*Handler)( const SerialMessage& serialMsg, QJsonObject& response );

    //<! map with status code strings
    static const QMap < uint8_t, QString >  _StatusCodes;

    //<! map with module types
    static const QMap < uint8_t, QString >  _ModuleTypes;

    //<! map with message handler debug info
    static const QMap < uint8_t, QString >  _EventNames;

    //<! map with message handlers
    static const QMap < uint8_t, Handler >  _Handlers;
};

#endif // __DeviceManagement_H__
