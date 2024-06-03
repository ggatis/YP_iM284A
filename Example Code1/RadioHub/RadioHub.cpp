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
#include <QSerialPortInfo>

/**
 * @brief   class constructor
 *
 * @param   client      reference to client
 */
RadioHub::RadioHub      ( RadioHub::Client& client )
        : _Client       ( client )
        , _DeviceMgmt   ( _Port )
        , _SlipDecoder  ( this )
{
    // connect to serial port for ready read events
    connect( &_Port, SIGNAL( readyRead() ), this, SLOT( OnSerialPort_ReadyRead() ) );
}

/**
 * @brief   enable RadioHub for communication with connected radio module
 *
 * @param   portName    name of serial port, e.g.: "ttyUSB", "COM5"
 *
 * @return  true/false
 */
bool
RadioHub::Enable( const QString& portName )
{
    // open serial port
    if ( _Port.isOpen() == true )
        return false;

    // reset SLIP decoder
    _SlipDecoder.Reset();

    // clear serial message buffer
    _RxMessage.clear();

    // configure port name
    _Port.setPortName( portName );

    // open port for read and write access
    if ( _Port.open( QIODevice::ReadWrite ) == false )
        return false;

    _Port.setBaudRate( QSerialPort::Baud115200 );
    _Port.setDataBits( QSerialPort::Data8 );
    _Port.setStopBits( QSerialPort::OneStop );
    _Port.setParity( QSerialPort::NoParity );
    _Port.setFlowControl( QSerialPort::NoFlowControl );

    QJsonObject info;
    info[ "Port" ]      = _Port.portName();
    info[ "Baudrate" ]  = QString::number( QSerialPort::Baud115200 ) + " bps";

    QSerialPortInfo pi = QSerialPortInfo( _Port );
    info[ "Manufacturer" ] = pi.manufacturer();

    // init object
    _DeviceInfo[ "Connection" ] = info;

    return true;
}

/**
 * @brief   process available bytes from serial port
 */
void
RadioHub::OnSerialPort_ReadyRead()
{
    // get number of received bytes
    quint64 numBytes =  _Port.bytesAvailable();

    if ( numBytes > 0 )
    {
        // pass incoming byte stream to SLIP decoder
        _SlipDecoder.Decode( _RxMessage, _Port.readAll() );
    }
}

/**
 * @brief   process SLIP decoded HCI messages
 */
void
RadioHub::OnSlipDecoder_MessageReady( const QByteArray& /* msg */ )
{
    // HCI message is aligned in _RxMessage, we can ignore incoming param "msg" here

    QJsonObject result;

    // pass message to message decoder and convert message content into human readable JsonObject
    if ( ServiceAccessPoint::OnDispatchMessage( _RxMessage, result ) )
    {
        _Client.OnRadioHub_DataEvent( result );
    }
}
