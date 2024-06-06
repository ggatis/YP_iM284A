/**
 * @file    DeviceManagement.h
 *
 * @brief   Declaration of class DeviceManagement
 *          - Message encoders and decoders for Device Managment HCI messages
 *
 * @note    This example code is free software: you can redistribute it and/or modify it.
 *
 *          This program is provided by IMST GmbH on an "AS IS" basis without
 *          any warranties in the hope that it will be useful.
 */

#ifndef _Device_Management_H_
#define _Device_Management_H_

#include "ServiceAccessPoint.h"

//#include <QJsonObject>
#include "Dictionary.h"

//#include <QMap>
#include "aMap.h"

//<! ServiceAccessPoint class for Device Management HCI messages
class DeviceManagement : public ServiceAccessPoint {

public:

    //HCI Sap ID
    enum SapIdentifier : uint8_t {
        Sap_ID                  =   0x01
    };

    //HCI Message IDs
    enum MessageIdentifier : uint8_t {
        Startup_Ind             =   0x00,
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
        GetDateTime_Rsp         =   0x10,

        SetSystemOptions_Req    =   0xF7,
        SetSystemOptions_Rsp    =   0xF8,
        GetSystemOptions_Req    =   0xF9,
        GetSystemOptions_Rsp    =   0xFA
    };

    //HCI Status Codes
    enum StatusCodes : uint8_t {
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

    //System Options
    enum SystemOptions : uint32_t {
        SO_APS                  =   ( 1u << 0 ),
        SO_Trace                =   ( 1u << 1 ),
        SO_RTC                  =   ( 1u << 2 ),
        SO_WatchDog             =   ( 1u << 3 ),
        SO_StartupEvent         =   ( 1u << 4 )
    };

    enum
    {
        ReservedInfo_Size       =   0,                 //currently not supported
        DeviceInfo_Size         =   ( 1 + 4 + 4 + 4 ), //ModuleType(1) + ModuleID(4) + ProductType(4) + ProductID(4)
        FirmwareInfo_MinSize    =   ( 2 + 2 + 10 + 1 ) //Fimrware Version(2) + BuildCount(2) +  BuildDate(10) + FirmwareName( > 1 )
    };

                                        DeviceManagement            ( HardwareSerial& port );

    /**
     * @brief   send ping request
     */
    bool                                OnPingDevice                ();

    /**
     * @brief   send "get device information"
     */
    bool                                OnGetDeviceInfo             ();

    /**
     * @brief   send "get firmware version"
     */
    bool                                OnGetFirmwareVersion        ();

    /**
     * @brief   send "get date time"
     */
    bool                                OnGetDateTime               ();

    /**
     * @brief   send "set date time"
     */
    bool                                OnSetDateTime               ();

    /**
     * @brief   send "restart device"
     */
    bool                                OnRestartDevice             ();

    /**
     * @brief   send "set system options"
     */
    bool                                OnSetSystemOptions          ( const Dictionary& params );

    /**
     * @brief   send "get system options"
     */
    bool                                OnGetSystemOptions          ();

private:

    /**
     * @brief   decoder interface for incoming messages
     */
    bool                                OnDecodeMessage             ( const SerialMessage& serialMsg, Dictionary& result ) override;

    /**
     * @brief   message decoder
     */
    bool                                OnDefaultResponse           ( const SerialMessage& serialMsg, Dictionary& result );
    bool                                OnStartupIndication         ( const SerialMessage& serialMsg, Dictionary& result );
    bool                                OnDeviceInfoResponse        ( const SerialMessage& serialMsg, Dictionary& result );
    bool                                OnFirmwareVersionResponse   ( const SerialMessage& serialMsg, Dictionary& result );
    bool                                OnDateTimeResponse          ( const SerialMessage& serialMsg, Dictionary& result );
    bool                                OnSystemOptionsResponse     ( const SerialMessage& serialMsg, Dictionary& result );

    Dictionary                          DecodeDeviceInfo            ( const SerialMessage& serialMsg, int index ) const;
    Dictionary                          DecodeFirmwareInfo          ( const SerialMessage& serialMsg, int index ) const;

private:

    //<! message decoder prototype
    typedef bool (DeviceManagement::*Handler)( const SerialMessage& serialMsg, Dictionary& response );

    //<! map with status code strings
    static const aMap < uint8_t, std::string >  _StatusCodes;

    //<! map with module types
    static const aMap < uint8_t, std::string >  _ModuleTypes;

    //<! map with message handler debug info
    static const aMap < uint8_t, std::string >  _EventNames;

    //<! map with message handlers
    static const aMap < uint8_t, Handler >  _Handlers;
};

#endif // _Device_Management_H_
