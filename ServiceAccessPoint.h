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

#ifndef _ServiceAccessPoint_H_
#define _ServiceAccessPoint_H_

#include "SerialMessage.h"
#include "Dictionary.h"

//#include <QSerialPort>

class ServiceAccessPoint {

public:
                                ServiceAccessPoint( uint8_t sapID, HardwareSerial& port, int numWakeupChars = 0 );

    //<! handle incoming messages
    static bool                 OnDispatchMessage( SerialMessage& serialMsg, Dictionary& result );

protected:

    //<! helpers for outgoing messages
    bool                        SendMessage( uint8_t reqID );
    bool                        SendMessage( SerialMessage& serialMsg );

    virtual bool                OnDecodeMessage( const SerialMessage& /* serialMsg */, Dictionary& /* result */ ) { return false; }

protected:

    //<! SAP identifier
    uint8_t                     _SapID;

    //<! SerialPort used from Qt
    HardwareSerial&             _Port;

    //<! wakeup chars for sleeping, power saving end nodes
    int                         _NumWakeupChars;

    //<! linked list for ServiceAccessPoints
    ServiceAccessPoint*         _Next;

    //<! first registered ServiceAccessPoint
    static ServiceAccessPoint*  _First;
};

#endif // _ServiceAccessPoint_H_
