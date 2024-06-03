/**
 * @file    Trace.cpp
 *
 * @brief   Implementation of class Trace
 *
 * @note    This example code is free software: you can redistribute it and/or modify it.
 *
 *          This program is provided by IMST GmbH on an "AS IS" basis without
 *          any warranties in the hope that it will be useful.
 */

#include "Trace.h"

//<! map with response & event names for HCI messages
const QMap < uint8_t , QString > Trace::_EventNames =
{
    { Trace_Event_1,            "Trace Event #1" },
    { Trace_Event_5,            "Trace Event #5" },
    { Trace_Event_6,            "Trace Event #6" },
    { Trace_Event_7,            "Trace Event #7" },
    { Trace_Event_8,            "Trace Event #8" },
    { Trace_Event_9,            "Trace Event #9" },
    { Trace_Event_10,           "Trace Event #10" },
    { Trace_Event_11,           "Trace Event #11" }
};

//<! map with message handlers for HCI messages
const QMap < uint8_t , Trace::Handler > Trace::_Handlers =
{
    { Trace_Event_1,            &Trace::OnTraceEvent_1 },
    { Trace_Event_5,            &Trace::OnTraceEvent_5 },
    { Trace_Event_6,            &Trace::OnTraceEvent_6 },
    { Trace_Event_7,            &Trace::OnTraceEvent_7 },
    { Trace_Event_8,            &Trace::OnTraceEvent_8 },
    { Trace_Event_9,            &Trace::OnTraceEvent_9 },
    { Trace_Event_10,           &Trace::OnTraceEvent_10 },
    { Trace_Event_11,           &Trace::OnTraceEvent_11 },
};

/**
 * @brief   class constructor
 *
 * @param   port        a serial port
 */

Trace::Trace( QSerialPort& port )
     : ServiceAccessPoint( Trace::Sap_ID, port )
{
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
Trace::OnDecodeMessage( const SerialMessage& serialMsg, QJsonObject& result )
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

bool
Trace::OnTraceEvent_1( const SerialMessage& serialMsg, QJsonObject& result )
{
    result[ "Event ID" ]    = serialMsg.GetHexString_LSB( SerialMessage::EventData_Index, 2 );
    result[ "String" ]      = serialMsg.GetString( SerialMessage::EventData_Index + 2 );
    return true;
}

bool
Trace::OnTraceEvent_5( const SerialMessage& serialMsg, QJsonObject& result )
{
    result[ "Event ID" ]    = serialMsg.GetHexString_LSB( SerialMessage::EventData_Index, 2 );
    uint32_t p1             = serialMsg.GetU32( SerialMessage::EventData_Index + 2 );
    result[ "Param 1" ]     = QString::number( p1 ) + QString( "(%1)" ).arg( p1, 8, 16, QLatin1Char( '0' ) ).toUpper();
    return true;
}

bool
Trace::OnTraceEvent_6( const SerialMessage& serialMsg, QJsonObject& result )
{
    result[ "Event ID" ]    = serialMsg.GetHexString_LSB( SerialMessage::EventData_Index, 2 );
    uint32_t p1             = serialMsg.GetU32( SerialMessage::EventData_Index + 2 );
    uint32_t p2             = serialMsg.GetU32( SerialMessage::EventData_Index + 6 );
    result[ "Param 1" ]     = QString::number( p1 ) + QString( "(%1)" ).arg( p1, 8, 16, QLatin1Char( '0' ) ).toUpper();
    result[ "Param 2" ]     = QString::number( p2 ) + QString( "(%1)" ).arg( p2, 8, 16, QLatin1Char( '0' ) ).toUpper();
    return true;
}

bool
Trace::OnTraceEvent_7( const SerialMessage& serialMsg, QJsonObject& result )
{
    result[ "Event ID" ]    = serialMsg.GetHexString_LSB( SerialMessage::EventData_Index, 2 );
    uint32_t p1             = serialMsg.GetU32( SerialMessage::EventData_Index + 2 );
    uint32_t p2             = serialMsg.GetU32( SerialMessage::EventData_Index + 6 );
    uint32_t p3             = serialMsg.GetU32( SerialMessage::EventData_Index + 10 );
    result[ "Param 1" ]     = QString::number( p1 ) + QString( "(%1)" ).arg( p1, 8, 16, QLatin1Char( '0' ) ).toUpper();
    result[ "Param 2" ]     = QString::number( p2 ) + QString( "(%1)" ).arg( p2, 8, 16, QLatin1Char( '0' ) ).toUpper();
    result[ "Param 3" ]     = QString::number( p3 ) + QString( "(%1)" ).arg( p3, 8, 16, QLatin1Char( '0' ) ).toUpper();
    return true;
}

bool
Trace::OnTraceEvent_8( const SerialMessage& serialMsg, QJsonObject& result )
{
    result[ "Event ID" ]    = serialMsg.GetHexString_LSB( SerialMessage::EventData_Index, 2 );
    uint32_t p1             = serialMsg.GetU32( SerialMessage::EventData_Index + 2 );
    uint32_t p2             = serialMsg.GetU32( SerialMessage::EventData_Index + 6 );
    uint32_t p3             = serialMsg.GetU32( SerialMessage::EventData_Index + 10 );
    uint32_t p4             = serialMsg.GetU32( SerialMessage::EventData_Index + 14 );
    result[ "Param 1" ]     = QString::number( p1 ) + QString( "(%1)" ).arg( p1, 8, 16, QLatin1Char( '0' ) ).toUpper();
    result[ "Param 2" ]     = QString::number( p2 ) + QString( "(%1)" ).arg( p2, 8, 16, QLatin1Char( '0' ) ).toUpper();
    result[ "Param 3" ]     = QString::number( p3 ) + QString( "(%1)" ).arg( p3, 8, 16, QLatin1Char( '0' ) ).toUpper();
    result[ "Param 4" ]     = QString::number( p3 ) + QString( "(%1)" ).arg( p4, 8, 16, QLatin1Char( '0' ) ).toUpper();
    return true;
}

bool
Trace::OnTraceEvent_9( const SerialMessage& serialMsg, QJsonObject& result )
{
    result[ "Event ID" ]    = serialMsg.GetHexString_LSB( SerialMessage::EventData_Index, 2 );
    uint32_t p1             = serialMsg.GetU32( SerialMessage::EventData_Index + 2 );
    result[ "Param 1" ]     = QString::number( p1 ) + QString( "(%1)" ).arg( p1, 8, 16, QLatin1Char( '0' ) ).toUpper();
    result[ "String" ]      = serialMsg.GetString( SerialMessage::EventData_Index + 6 );
    return true;
}

bool
Trace::OnTraceEvent_10( const SerialMessage& serialMsg, QJsonObject& result )
{
    result[ "Event ID" ]    = serialMsg.GetHexString_LSB( SerialMessage::EventData_Index, 2 );
    uint32_t p1             = serialMsg.GetU32( SerialMessage::EventData_Index + 2 );
    uint32_t p2             = serialMsg.GetU32( SerialMessage::EventData_Index + 6 );
    result[ "Param 1" ]     = QString::number( p1 ) + QString( "(%1)" ).arg( p1, 8, 16, QLatin1Char( '0' ) ).toUpper();
    result[ "Param 2" ]     = QString::number( p2 ) + QString( "(%1)" ).arg( p2, 8, 16, QLatin1Char( '0' ) ).toUpper();
    result[ "String" ]      = serialMsg.GetString( SerialMessage::EventData_Index + 10 );
    return true;
}

bool
Trace::OnTraceEvent_11( const SerialMessage& serialMsg, QJsonObject& result )
{
    result[ "Module ID" ]   = QString::number( serialMsg.GetU8( SerialMessage::EventData_Index ) );
    result[ "State" ]       = QString::number( serialMsg.GetU8( SerialMessage::EventData_Index + 1 ) );
    result[ "Slot" ]        = QString::number( serialMsg.GetU16( SerialMessage::EventData_Index + 2 ) );
    result[ "Multiframe" ]  = QString::number( serialMsg.GetU16( SerialMessage::EventData_Index + 4 ) );
    result[ "String" ]      = serialMsg.GetString( SerialMessage::EventData_Index + 6, -1 );
    return true;
}
