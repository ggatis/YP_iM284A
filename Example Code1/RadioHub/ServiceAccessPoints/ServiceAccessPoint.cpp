/**
 * @file    ServiceAccessPoint.cpp
 *
 * @brief   Implementation of class ServiceAccessPoint
 *
 * @note    This example code is free software: you can redistribute it and/or modify it.
 *
 *          This program is provided by IMST GmbH on an "AS IS" basis without
 *          any warranties in the hope that it will be useful.
 */

#include "ServiceAccessPoint.h"
#include "RadioHub/Utils/SlipEncoder.h"

//<! init static members
ServiceAccessPoint* ServiceAccessPoint::_First = nullptr;

/**
 * @brief   class constructor
 *
 * @param   sapID   ID of this service access point
 *
 * @param   port    a serial port
 */
ServiceAccessPoint::ServiceAccessPoint( uint8_t sapID, QSerialPort& port )
                  : _SapID  ( sapID )
                  , _Port   ( port )
{
    // add to linked list
    _Next = _First;
    _First = this;
}

/**
 * @brief   send a simple command without payload
 *
 * @param   reqID       ID of outgoing command
 *
 * @return  true/false
 */
bool
ServiceAccessPoint::SendMessage( uint8_t reqID )
{
    SerialMessage   msg( _SapID, reqID );

    return SendMessage( msg );
}

/**
 * @brief   send a messsage
 *
 * @param   serialMsg   outgoing message
 *
 * @return  true/false
 */
bool
ServiceAccessPoint::SendMessage( SerialMessage& serialMsg )
{
    QByteArray  outputData;

    // calculate and append CRCC16
    serialMsg.Append_CRC16();

    // encode SLIP frame and forward via port
    return ( _Port.write( SlipEncoder::Encode( outputData, serialMsg ) ) == outputData.count() );
}

/**
 * @brief   check crc and forward message to corresponding service access point
 *
 * @param   serialMsg   incoming message
 *
 * @param   result      decoded data in Json fromat
 *
 * @return  true/false
 */
bool
ServiceAccessPoint::OnDispatchMessage( SerialMessage& serialMsg, QJsonObject& result )
{
    // check CRC first
    if ( serialMsg.CheckCRC16() == false )
        return false;

    serialMsg.RemoveCRC16();

    ServiceAccessPoint* sap = _First;

    uint8_t sapID = serialMsg.GetSapID();

    // find SAP
    while ( sap != nullptr )
    {
        if ( sap->_SapID == sapID )
        {
            return sap->OnDecodeMessage( serialMsg, result );
        }

        // try next one
        sap = sap->_Next;
    }
    return false;
}
