/**
 * @file    ServiceAccessPoint.h
 *
 * @brief   Declaration of class ServiceAccessPoint
 *
 * @note    This example code is free software: you can redistribute it and/or modify it.
 *
 *          This program is provided by IMST GmbH on an "AS IS" basis without
 *          any warranties in the hope that it will be useful.
 */

#ifndef __ServiceAccessPoint_H__
#define __ServiceAccessPoint_H__

#include "RadioHub/Utils/SerialMessage.h"

#include <QSerialPort>

class ServiceAccessPoint
{
public:
                                ServiceAccessPoint( uint8_t sapID, QSerialPort& port, int numWakeupChars = 0 );

    //<! handle incoming messages
    static bool                 OnDispatchMessage( SerialMessage& serialMsg, QJsonObject& result );

protected:

    //<! helpers for outgoing messages
    bool                        SendMessage( uint8_t reqID );
    bool                        SendMessage( SerialMessage& serialMsg );

    virtual bool                OnDecodeMessage( const SerialMessage& /* serialMsg */, QJsonObject& /* result */ ) { return false; }

protected:

    //<! SAP identifier
    uint8_t                     _SapID;

    //<! SerialPort used from Qt
    QSerialPort&                _Port;

    //<! wakeup chars for sleeping, power saving end nodes
    int                         _NumWakeupChars;

    //<! linked list for ServiceAccessPoints
    ServiceAccessPoint*         _Next;

    //<! first registered ServiceAccessPoint
    static ServiceAccessPoint*  _First;
};

#endif // __ServiceAccessPoint_H__
