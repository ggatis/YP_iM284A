/**
 * @file    Trace.h
 *
 * @brief   Declaration of class Trace
 *          - Message decoders for Trace HCI messages
 *
 * @note    This example code is free software: you can redistribute it and/or modify it.
 *
 *          This program is provided by IMST GmbH on an "AS IS" basis without
 *          any warranties in the hope that it will be useful.
 */

#ifndef __Trace_H__
#define __Trace_H__

#include "RadioHub/ServiceAccessPoints/ServiceAccessPoint.h"

#include <QJsonObject>
#include <QJsonArray>
#include <QMap>

class Trace : public ServiceAccessPoint

{
public:

    // HCI Sap ID
    enum SapIdentifier : uint8_t
    {
        Sap_ID                 =   0xA0
    };

    // HCI Message IDs
    enum MessageIdentifier : uint8_t
    {
        Trace_Event_1           =   0x01,
        Trace_Event_5           =   0x05,
        Trace_Event_6           =   0x06,
        Trace_Event_7           =   0x07,
        Trace_Event_8           =   0x08,
        Trace_Event_9           =   0x09,
        Trace_Event_10          =   0x0A,
        Trace_Event_11          =   0x0B
    };

                            Trace( QSerialPort& port );

private:

    /**
     * @brief   decoder interface for incoming messages
     */
    bool                    OnDecodeMessage             ( const SerialMessage& serialMsg, QJsonObject& result ) override;

    /**
     * @brief   message decoder
     */
    bool                    OnTraceEvent_1              ( const SerialMessage& serialMsg, QJsonObject& result );
    bool                    OnTraceEvent_5              ( const SerialMessage& serialMsg, QJsonObject& result );
    bool                    OnTraceEvent_6              ( const SerialMessage& serialMsg, QJsonObject& result );
    bool                    OnTraceEvent_7              ( const SerialMessage& serialMsg, QJsonObject& result );
    bool                    OnTraceEvent_8              ( const SerialMessage& serialMsg, QJsonObject& result );
    bool                    OnTraceEvent_9              ( const SerialMessage& serialMsg, QJsonObject& result );
    bool                    OnTraceEvent_10             ( const SerialMessage& serialMsg, QJsonObject& result );
    bool                    OnTraceEvent_11             ( const SerialMessage& serialMsg, QJsonObject& result );

    //<! message decoder prototype
    typedef bool (Trace::*Handler)( const SerialMessage& serialMsg, QJsonObject& response );

    //<! map with message handler debug info
    static const QMap < uint8_t, QString >  _EventNames;

    //<! map with message handlers
    static const QMap < uint8_t, Handler >  _Handlers;
};

#endif // __Trace_H__
