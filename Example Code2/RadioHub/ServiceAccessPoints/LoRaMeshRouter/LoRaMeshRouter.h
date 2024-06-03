/**
 * @file    LoRaMeshRouter.h
 *
 * @brief   Declaration of class LoRaMeshRouter
 *          - Message encoders and decoders for LoRaMeshRouter HCI messages
 *
 * @note    This example code is free software: you can redistribute it and/or modify it.
 *
 *          This program is provided by IMST GmbH on an "AS IS" basis without
 *          any warranties in the hope that it will be useful.
 */

#ifndef __LoRa_MeshRouter_H__
#define __LoRa_MeshRouter_H__

#include "RadioHub/ServiceAccessPoints/ServiceAccessPoint.h"

#include <QJsonObject>
#include <QJsonArray>
#include <QMap>

//<! ServiceAccessPoint class for LoRaMeshRouter HCI messages
class LoRaMeshRouter : public ServiceAccessPoint
{
public:

    // HCI Sap ID
    enum SapIdentifier : uint8_t
    {
        Sap_ID                 =   0x0A
    };

    // HCI Message IDs
    enum MessageIdentifier : uint8_t
    {
        GetNetworkAddress_Req   =   0x01,
        GetNetworkAddress_Rsp   =   0x02,
        SetNetworkAddress_Req   =   0x03,
        SetNetworkAddress_Rsp   =   0x04,

        GetMode_Req             =   0x11,
        GetMode_Rsp             =   0x12,
        SetMode_Req             =   0x13,
        SetMode_Rsp             =   0x14,

        GetLinkStatus_Req       =   0x15,
        GetLinkStatus_Rsp       =   0x16,
        LinkStatusChange_Ind    =   0x18,

        GetRoutingInfo_Req      =   0x19,
        GetRoutingInfo_Rsp      =   0x1A,

        SendPacket_Req          =   0x21,
        SendPacket_Rsp          =   0x22,
        PacketReceived_Ind      =   0x26,
    };

    // HCI Status Codes
    enum StatusCodes : uint8_t
    {
        Ok                      =   0x00,
        Error                   =   0x01,
        CommandNotSupported     =   0x02,
        WrongParameter          =   0x03,
        WrongApplicationMode    =   0x04,
        NoMoreData              =   0x05,
        ApplicationBusy         =   0x06,
        WrongMsgLength          =   0x07,
        NVM_WriteError          =   0x08,
        NVM_ReadError           =   0x09,
        CommandRejected         =   0x0A,
        NoLink                  =   0x0B,
        NoRoute                 =   0x0C,
        WrongAddress            =   0x0D,
        NoBuffer                =   0x0E,
        TxQueueFull             =   0x0F
    };

    enum Modes : uint8_t
    {
        Mode_Off                =   0,
        Mode_Router             =   1,
        Mode_Coordinator        =   2,
        Mode_Invalid            =   0xFF
    };

    enum
    {
        MaxNode_Items           =   8,
        RSSI_Offset             =   64
    };

    enum
    {
        // NetworkID(2) + DeviceEUI(8)
        NetworkAddress_Size     =   ( 2 + 8 ),

        // NodeType(1) + State(1) + NodeAddress(2) + Rank(1) + CellSize(1) + BeaconIndex(1)
        LinkStatus_Size         =   ( 1 + 1 + 2 + 1 + 1 + 1 ),

        // DeviceEUI(8) + NodeAddress(2) + RouterAddress(2) + NodeType(1) + State(1) + Rank(1) + BeaconIndex(1) + Visibility(1) + RSSI(1) + FW_Version(2)
        RoutingInfo_Size        =  ( 8 + 2 + 2 + 1 + 1 + 1 + 1 + 1 + 1 + 2 ),

        // RSSI(1) + SNR(1) + SourceEUI(8) + Port(1) + Payload(> 1)
        PacketInfo_MinSize      =  ( 1 + 1 + 8 + 1 + 1 )
    };

                                        LoRaMeshRouter              ( QSerialPort& port );

    /**
     * @brief   read Device EUI
     */
    bool                                OnGetNetworkAddress         ();

    /**
     * @brief   set Device EUI
     */
    bool                                OnSetNetworkAddress         ( const QJsonObject& params );

    /**
     * @brief   get Mode
     */
    bool                                OnGetMode                   ();

    /**
     * @brief   set Mode
     */
    bool                                OnSetMode                   ( const QJsonObject& params );

    /**
     * @brief   read Link Status
     */
    bool                                OnGetLinkStatus             ();

    /**
     * @brief   read Node Information
     */
    bool                                OnGetRoutingInfo            ( const QJsonObject& params );

    /**
     * @brief   send a data packet
     */
    bool                                OnSendPacket                ( const QJsonObject& params );

private:

    /**
     * @brief   decoder interface for incoming messages
     */
    bool                                OnDecodeMessage             ( const SerialMessage& serialMsg, QJsonObject& result ) override;

    /**
     * @brief   message decoder
     */
    bool                                OnDefaultResponse           ( const SerialMessage& serialMsg, QJsonObject& result );
    bool                                OnGetNetworkAddressResponse ( const SerialMessage& serialMsg, QJsonObject& result );
    bool                                OnGetModeResponse           ( const SerialMessage& serialMsg, QJsonObject& result );
    bool                                OnGetLinkStatusResponse     ( const SerialMessage& serialMsg, QJsonObject& result );
    bool                                OnGetRoutingInfoResponse    ( const SerialMessage& serialMsg, QJsonObject& result );
    bool                                OnLinkStatusChangeEvent     ( const SerialMessage& serialMsg, QJsonObject& result );
    bool                                OnPacketReceivedEvent       ( const SerialMessage& serialMsg, QJsonObject& result );

    //<! message decoder prototype
    typedef bool (LoRaMeshRouter::*Handler)( const SerialMessage& serialMsg, QJsonObject& response );

    //<! map with status code strings
    static const QMap < uint8_t, QString >  _StatusCodes;

    //<! map with modes
    static const QMap < uint8_t, QString >  _Modes;

    //<! map with message handler debug info
    static const QMap < uint8_t, QString >  _EventNames;

    //<! map with message handlers
    static const QMap < uint8_t, Handler >  _Handlers;
};

#endif // __LoRa_MeshRouter_H__
