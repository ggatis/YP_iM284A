/**
 * @file    LoRaMeshRouter.cpp
 *
 * @brief   Implementation of class LoRaMeshRouter
 *
 * @note    This example code is free software: you can redistribute it and/or modify it.
 *
 *          This program is provided by IMST GmbH on an "AS IS" basis without
 *          any warranties in the hope that it will be useful.
 */

#include "LoRaMeshRouter.h"

//<! map with status code strings
const QMap < uint8_t, QString > LoRaMeshRouter::_StatusCodes =
{
    { Ok,                       "ok" },
    { Error,                    "error" },
    { CommandNotSupported,      "command not supported" },
    { WrongParameter,           "wrong parameter" },
    { WrongApplicationMode,     "wrong application mode" },
    { NoMoreData,               "no more data" },
    { ApplicationBusy,          "application busy" },
    { WrongMsgLength,           "wrong message length" },
    { NVM_WriteError,           "NVM write error" },
    { NVM_ReadError,            "NVM read error" },
    { CommandRejected,          "command rejected" },
    { NoLink,                   "no link" },
    { NoRoute,                  "no route " },
    { WrongAddress,             "wrong address" },
    { NoBuffer,                 "no buffer" },
    { TxQueueFull,              "transmit queue full" }
};

//<! map with module type strings
const QMap < uint8_t, QString > LoRaMeshRouter::_Modes =
{
    { Mode_Off,                 "Off" },
    { Mode_Router,              "Router" },
    { Mode_Coordinator,         "Coordinator" }
};

//<! map with response & event names for HCI messages
const QMap < uint8_t , QString > LoRaMeshRouter::_EventNames =
{
    { GetNetworkAddress_Rsp,    "get Network Address response" },
    { SetNetworkAddress_Rsp,    "set Network Address response" },
    { GetMode_Rsp,              "get Mode response" },
    { SetMode_Rsp,              "set Mode response" },
    { GetLinkStatus_Rsp,        "get Link Status response" },
    { GetRoutingInfo_Rsp,       "get Routing Info response" },
    { SendPacket_Rsp,           "send packet response" },
    { LinkStatusChange_Ind,     "link status change event" },
    { PacketReceived_Ind,       "packet received event" },
};

//<! map with message handlers for HCI messages
const QMap < uint8_t , LoRaMeshRouter::Handler > LoRaMeshRouter::_Handlers =
{
    // response handlers
    { GetNetworkAddress_Rsp,    &LoRaMeshRouter::OnGetNetworkAddressResponse },
    { SetNetworkAddress_Rsp,    &LoRaMeshRouter::OnDefaultResponse },
    { GetMode_Rsp,              &LoRaMeshRouter::OnGetModeResponse },
    { SetMode_Rsp,              &LoRaMeshRouter::OnDefaultResponse },
    { GetLinkStatus_Rsp,        &LoRaMeshRouter::OnGetLinkStatusResponse },
    { GetRoutingInfo_Rsp,       &LoRaMeshRouter::OnGetRoutingInfoResponse },
    { SendPacket_Rsp,           &LoRaMeshRouter::OnDefaultResponse },
    // event handlers
    { LinkStatusChange_Ind,     &LoRaMeshRouter::OnLinkStatusChangeEvent },
    { PacketReceived_Ind,       &LoRaMeshRouter::OnPacketReceivedEvent },
};

/**
 * @brief   class constructor
 *
 * @param   port        a serial port
 */

LoRaMeshRouter::LoRaMeshRouter( QSerialPort& port )
              : ServiceAccessPoint( LoRaMeshRouter::Sap_ID, port )
{
}

bool
LoRaMeshRouter::OnGetNetworkAddress()
{
    return SendMessage( GetNetworkAddress_Req );
}

bool
LoRaMeshRouter::OnSetNetworkAddress( const QJsonObject& params )
{
    SerialMessage   msg( LoRaMeshRouter::Sap_ID, SetNetworkAddress_Req );

    if ( msg.AppendHexString_LSB( params.value( "Network-ID" ).toString() ) != 2 )
        return false;

    if ( msg.AppendHexString_LSB( params.value( "Device-EUI" ).toString() ) != 8 )
        return false;

    return SendMessage( msg );
}

bool
LoRaMeshRouter::OnGetMode()
{
    return SendMessage( GetMode_Req );
}

bool
LoRaMeshRouter::OnSetMode( const QJsonObject& params )
{
    SerialMessage   msg( LoRaMeshRouter::Sap_ID, SetMode_Req );

    uint8_t mode = _Modes.key( params.value( "Mode" ).toString(), Mode_Invalid );

    if ( mode == Mode_Invalid )
        return false;

    msg.Append( mode );

    return SendMessage( msg );
}

bool
LoRaMeshRouter::OnGetLinkStatus()
{
    return SendMessage( GetLinkStatus_Req );
}

bool
LoRaMeshRouter::OnGetRoutingInfo( const QJsonObject& params )
{
    uint8_t index = static_cast< uint8_t > ( params.value( "Index" ).toInt( 0xFF ) );
    if ( index == 0xFF )
        return false;

    uint8_t maxItems = static_cast< uint8_t > ( params.value( "MaxItems" ).toInt( 1 ) );
    if ( maxItems > MaxNode_Items )
        maxItems = MaxNode_Items;

    SerialMessage   msg( LoRaMeshRouter::Sap_ID, GetRoutingInfo_Req );
    msg.Append( index );
    msg.Append( maxItems );
    return SendMessage( msg );
}

bool
LoRaMeshRouter::OnSendPacket( const QJsonObject& params )
{
    SerialMessage   msg( LoRaMeshRouter::Sap_ID, SendPacket_Req );

    // Tx Options ( reserved for future usage )
    uint8_t txOptions = 0;
    msg.Append( txOptions );

    // Destination EUI
    if ( msg.AppendHexString_LSB( params.value( "Destination-EUI" ).toString() ) != 8 )
        return false;

    // Port
    uint8_t port = static_cast< uint8_t > ( params.value( "Port" ).toInt( 1 ) );
    msg.Append( port );

    // Payload
    if ( msg.AppendHexString( params.value( "Payload" ).toString() ) < 1 )
        return false;

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
LoRaMeshRouter::OnDecodeMessage( const SerialMessage& serialMsg, QJsonObject& result )
{
    uint8_t msgID = serialMsg.GetMsgID();

    // find handler by message ID
    Handler handler = _Handlers.value( msgID, nullptr );
    if ( handler != nullptr )
    {
        // get handler event name
        result[ "Event" ] = QString( _EventNames.value( msgID, "unknown handler name" ) );

        // call message handler
        return ( this->*handler )( serialMsg, result );
    }
    // no handler found
    result[ "Error" ] = "unsupported MsgID: " + QString::number( msgID ) + " received";
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
LoRaMeshRouter::OnDefaultResponse( const SerialMessage& serialMsg, QJsonObject& result )
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
 * @brief   decode Network Address response
 *
 * @param   serialMsg       incoming HCI message
 *
 * @param   result          decoded data in Json format
 *
 * @return  true/false
 */

bool
LoRaMeshRouter::OnGetNetworkAddressResponse( const SerialMessage& serialMsg, QJsonObject& result )
{
    // check minimum payload length
    if ( serialMsg.GetResponsePayloadLength() < NetworkAddress_Size )
        return false;

    // 2 bytes for Netowrk ID, LSB first
    result[ "Network-ID" ]  = serialMsg.GetHexString_LSB( SerialMessage::ResponseData_Index, 2 );

    // 8 bytes for 64 Bit DeviceEUI, LSB first
    result[ "Device-EUI" ]  = serialMsg.GetHexString_LSB( SerialMessage::ResponseData_Index + 2, 8 );

    return true;
}

/**
 * @brief   decode Mode response
 *
 * @param   serialMsg       incoming HCI message
 *
 * @param   result          decoded data in Json format
 *
 * @return  true/false
 */

bool
LoRaMeshRouter::OnGetModeResponse( const SerialMessage& serialMsg, QJsonObject& result )
{
    // check minimum payload length
    if ( serialMsg.GetResponsePayloadLength() < ( 1 ) )
        return false;

    result[ "Mode" ] = _Modes.value( serialMsg.GetU8( SerialMessage::ResponseData_Index ), "unknown mode" );

    return true;
}

/**
 * @brief   decode Link Status response
 *
 * @param   serialMsg       incoming HCI message
 *
 * @param   result          decoded data in Json format
 *
 * @return  true/false
 */

bool
LoRaMeshRouter::OnGetLinkStatusResponse( const SerialMessage& serialMsg, QJsonObject& result )
{
    // check minimum payload length
    if ( serialMsg.GetResponsePayloadLength() < LinkStatus_Size )
        return false;

    result[ "Node Type" ]       = QString::number( serialMsg.GetU8( SerialMessage::ResponseData_Index ) );
    result[ "State" ]           = QString::number( serialMsg.GetU8( SerialMessage::ResponseData_Index + 1 ) );
    result[ "Node Address" ]    = serialMsg.GetHexString_LSB( SerialMessage::ResponseData_Index + 2, 2 );
    result[ "Rank" ]            = QString::number( serialMsg.GetU8( SerialMessage::ResponseData_Index + 4 ) );
    result[ "Cell Size" ]       = QString::number( serialMsg.GetU8( SerialMessage::ResponseData_Index + 5 ) );
    result[ "Beacon Index" ]    = QString::number( serialMsg.GetU8( SerialMessage::ResponseData_Index + 6 ) );

    return true;
}

/**
 * @brief   decode Routing Information response
 *
 * @param   serialMsg       incoming HCI message
 *
 * @param   result          decoded data in Json format
 *
 * @return  true/false
 */

bool
LoRaMeshRouter::OnGetRoutingInfoResponse( const SerialMessage& serialMsg, QJsonObject& result )
{
    QJsonArray  array;

    int index   = SerialMessage::ResponseData_Index;
    int len     = serialMsg.GetTotalLength();

    while ( ( index + RoutingInfo_Size ) <= len )
    {
        QJsonObject item;
        item[ "Device-EUI" ]        = serialMsg.GetHexString_LSB( index, 8 );
        item[ "Local Address" ]     = serialMsg.GetHexString_LSB( index + 8, 2 );
        item[ "Router Address" ]    = serialMsg.GetHexString_LSB( index + 10, 2 );
        item[ "Node Type" ]         = QString::number( serialMsg.GetU8( index + 12 ) );
        item[ "State" ]             = QString::number( serialMsg.GetU8( index + 13 ) );
        item[ "Rank" ]              = QString::number( serialMsg.GetU8( index + 14 ) );
        item[ "Beacon Index" ]      = QString::number( serialMsg.GetU8( index + 15 ) );
        item[ "Visibility" ]        = QString::number( serialMsg.GetU8( index + 16 ) );
        item[ "RSSI" ]              = QString::number( serialMsg.GetI8( index + 17 ) - RSSI_Offset ) + " dBm";
        item[ "FW Version" ]        = QString::number( serialMsg.GetU16( index + 18 ) );

        array.append( item );

        index = index + RoutingInfo_Size;
    }

    result[ "Mesh Router Nodes" ] = array;

    return true;
}

/**
 * @brief   decode Link Status change event
 *
 * @param   serialMsg       incoming HCI message
 *
 * @param   result          decoded data in Json format
 *
 * @return  true/false
 */

bool
LoRaMeshRouter::OnLinkStatusChangeEvent( const SerialMessage& serialMsg, QJsonObject& result )
{
    // check minimum payload length
    if ( serialMsg.GetPayloadLength() < LinkStatus_Size )
        return false;

    result[ "Node Type" ]       = QString::number( serialMsg.GetU8( SerialMessage::EventData_Index ) );
    result[ "State" ]           = QString::number( serialMsg.GetU8( SerialMessage::EventData_Index + 1 ) );
    result[ "Node Address" ]    = serialMsg.GetHexString_LSB( SerialMessage::EventData_Index + 2, 2 );
    result[ "Rank" ]            = QString::number( serialMsg.GetU8( SerialMessage::EventData_Index + 4 ) );
    result[ "Cell Size" ]       = QString::number( serialMsg.GetU8( SerialMessage::EventData_Index + 5 ) );
    result[ "Beacon Index" ]    = QString::number( serialMsg.GetU8( SerialMessage::EventData_Index + 6 ) );

    return true;
}

/**
 * @brief   decode packet received event
 *
 * @param   serialMsg       incoming HCI message
 *
 * @param   result          decoded data in Json format
 *
 * @return  true/false
 */

bool
LoRaMeshRouter::OnPacketReceivedEvent( const SerialMessage& serialMsg, QJsonObject& result )
{
    // check minimum payload length
    if ( serialMsg.GetPayloadLength() < PacketInfo_MinSize )
        return false;

    result[ "RSSI" ]        =   QString::number( serialMsg.GetI8( SerialMessage::EventData_Index ) - RSSI_Offset ) + " dBm";
    result[ "SNR" ]         =   QString::number( serialMsg.GetI8( SerialMessage::EventData_Index + 1 ) ) + " dB";
    result[ "Source-EUI" ]  =   serialMsg.GetHexString_LSB( SerialMessage::EventData_Index + 2, 8 );
    result[ "Port" ]        =   QString::number( serialMsg.GetU8( SerialMessage::EventData_Index + 10 ) );
    result[ "Payload" ]     =   serialMsg.GetHexString( SerialMessage::EventData_Index + 11 );

    return true;
}
