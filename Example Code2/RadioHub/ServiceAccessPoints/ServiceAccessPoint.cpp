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
ServiceAccessPoint::ServiceAccessPoint( uint8_t sapID, QSerialPort& port, int numWakeupChars )
                  : _SapID          ( sapID )
                  , _Port           ( port )
                  , _NumWakeupChars ( numWakeupChars )
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

#ifdef  SlipEncoder_BufferLess_Version
    // included here to demonstrate possible solution for controllers with less RAM
    // must be a "global" object, NOT a local object on STACK !!!
    SlipEncoder encoder;

    encoder.SetInput( &serialMsg, _NumWakeupChars );

    // simulate UART Tx Ready interrupts
    bool done = false;
    while ( done == false )
    {
        // get next tx byte from encoder
        int16_t nextData = encoder.GetEncodedByte();

        // done ?
        if ( nextData >= 0 )
        {
            // put lower 8-Bit in UARTs transmit register and "wait" for next interrupt
            outputData.append( (uint8_t)( nextData & 0xFF ) );
        }
        else
        {
            // last byte transmitted
            done = true;

            // wait for last "UART Tx shift register empty" interrupt
            // and update encoder state for next message
            encoder.OnCompleteIndication();
        }
    }

#else
    // simple solution for systems with enough RAM

    // prepend wakeup chars
    if ( _NumWakeupChars > 0 )
    {
        outputData.append(
                outputData.append( _NumWakeupChars, SlipEncoder::Begin )
            );
    }

    SlipEncoder::Encode( outputData, serialMsg );

#endif

    // encode SLIP frame and forward to port
    return ( _Port.write( outputData ) == outputData.count() );
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
        // SAP found ?
        if ( sap->_SapID == sapID )
        {
            // yes, call message decoder
            return sap->OnDecodeMessage( serialMsg, result );
        }

        // try next one
        sap = sap->_Next;
    }
    return false;
}
