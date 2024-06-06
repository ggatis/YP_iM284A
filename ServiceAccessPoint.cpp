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
#include "SlipEncoder.h"


//<! init static members
ServiceAccessPoint* ServiceAccessPoint::_First = nullptr;


/**
 * @brief   class constructor
 *
 * @param   sapID   ID of this service access point
 *
 * @param   port    a serial port
 */
ServiceAccessPoint::ServiceAccessPoint( uint8_t sapID, HardwareSerial& port, int numWakeupChars )
                  : _SapID          ( sapID )
                  , _Port           ( port )
                  , _NumWakeupChars ( numWakeupChars ) {
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
ServiceAccessPoint::SendMessage( uint8_t reqID ) {
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
ServiceAccessPoint::SendMessage( SerialMessage& serialMsg ) {

    ByteArray  outputData;

    //calculate and append CRCC16
    serialMsg.Append_CRC16();

    //simple solution for systems with enough RAM?
    //prepend wakeup chars
    for ( uint8_t i = 0; i < _NumWakeupChars; i++ ) {
        outputData.append( SlipEncoder::Begin );
    }

    SlipEncoder::Encode( outputData, serialMsg );

    // encode SLIP frame and forward to port
    uint16_t output_size = outputData.count();
    if ( output_size <= _Port.availableForWrite() ) {
        return ( _Port.write( outputData.data(), (size_t)output_size ) == output_size );
    }
    uint16_t offset = 0;
    //add some TO and SLEEP?
    while ( offset < output_size ) {
        uint16_t available = _Port.availableForWrite();
        if ( available )
            offset += _Port.write( outputData.data() + offset, (size_t)available );
    }

    return true;
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
ServiceAccessPoint::OnDispatchMessage( SerialMessage& serialMsg, Dictionary& result ) {

    // check CRC first
    if ( false == serialMsg.CheckCRC16() )
        return false;

    serialMsg.RemoveCRC16();

    ServiceAccessPoint* sap = _First;

    uint8_t sapID = serialMsg.GetSapID();

    // find SAP
    while ( sap != nullptr ) {

        // SAP found ?
        if ( sap->_SapID == sapID ) {
            // yes, call message decoder
            return sap->OnDecodeMessage( serialMsg, result );
        }

        // try next one
        sap = sap->_Next;
    }
    return false;
}
