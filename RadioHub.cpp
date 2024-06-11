/**
 * @file    RadioHub.cpp
 *
 * @brief   Implementation of class RadioHub
 *
 * @note    This example code is free software: you can redistribute it and/or modify it.
 *
 *          This program is provided by IMST GmbH on an "AS IS" basis without
 *          any warranties in the hope that it will be useful.
 */

#include "RadioHub.h"
#include "printSTDstring.h"
//#include <QSerialPortInfo>

/**
 * @brief   class constructor
 *
 * @param   client      reference to client
 */
RadioHub::RadioHub( RadioHub::Client& client, HardwareSerial& RadioSerial )
        : _Client           ( client )
        , _RadioSerial      ( RadioSerial )
        , _DeviceMgmt       ( RadioSerial )
//        , _LoRaMeshRouter   ( RadioSerial )
//        , _Trace            ( RadioSerial )
        , _SlipDecoder      ( this ) {

    //connect to serial port for ready read events
    //connect( &_Port, SIGNAL( readyRead() ), this, SLOT( OnSerialPort_ReadyRead() ) );

}

/**
 * @brief   initializes the RadioHub for communication with connected radio module
 *
 * @param   -
 *
 * @return  true/false
 */
bool
RadioHub::Init( void ) {
    //IMHO should check if module is attached!!!

    _RadioSerial.end();
    _RadioSerial.begin( 115200, SERIAL_8N1 );
    //_Port.setFlowControl( QSerialPort::NoFlowControl );

    _SerialInfo.append("Port", "Serial1");
    _SerialInfo.append("Baudrate", "115200 bps");
    _SerialInfo.append("Config", "SERIAL_8N1");
    _SerialInfo.append("Manufacturer", "STM");

    //clear serial message buffer
    _RxMessage.clear();

    //reset SLIP decoder
    _SlipDecoder.Reset();

    return true;
}

/**
 * @brief   return assigned serial port
 */
HardwareSerial
RadioHub::GetSerial( void ) {
    return _RadioSerial;
}

/**
 * @brief   process available bytes from serial port
 */
void
RadioHub::OnSerialPort_ReadyRead( void ) {

#if 1
    //A
    while ( 0 < _RadioSerial.available() ) {
        //pass incoming byte stream to SLIP decoder
        //_SlipDecoder.Decode( _RxMessage, _Port.readAll() );
        _SlipDecoder.Decode( _RxMessage, _RadioSerial.read() );
    }
#else
    //B
    _SlipDecoder.Decode( _RxMessage, _RadioSerial.read );
#endif
}

/**
 * @brief   process SLIP decoded HCI messages
 */
void
RadioHub::OnSlipDecoder_MessageReady( const ByteArray& msg ) {

    //HCI message is available in _RxMessage, we can ignore incoming param "msg" here
    //since it points to the same _RxMessage
    printSTDstring( _RxMessage.GetHexString() );

    Dictionary result;

    //pass message to message decoder and convert message content into human readable JsonObject
    if ( ServiceAccessPoint::OnDispatchMessage( _RxMessage, result ) ) {
        _Client.OnRadioHub_DataEvent( result );
    } else {
        printSTDstring("No dispachers for: ");
        printSTDstring( _RxMessage.GetHexString() );
    }

}
