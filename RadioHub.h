/**
 * @file    RadioHub.h
 *
 * @brief   Declaration of class RadioHub
 *
 * @note    This example code is free software: you can redistribute it and/or modify it.
 *
 *          This program is provided by IMST GmbH on an "AS IS" basis without
 *          any warranties in the hope that it will be useful.
 */

#ifndef _RadioHub_H_
#define _RadioHub_H_

#include "SerialMessage.h"

//#include "ServiceAccessPoints/DeviceManagement/DeviceManagement.h"
//#include "ServiceAccessPoints/LoRaMeshRouter/LoRaMeshRouter.h"
//#include "ServiceAccessPoints/Trace/Trace.h"

#include "SlipDecoder.h"

//#include <QSerialPort>
//#include <QJsonObject>
//#include <QString>
#include "Dictionary.h"     //also "HardwareSerial.h", "ByteArray.h"


//<! top level interface class for radio module communication
//class RadioHub : public QObject
//               , public SlipDecoder::Client

class RadioHub : public SlipDecoder::Client {

public:
    // declaration of client interface
    class Client {
    public:
        //<! callback interface for decoded data
        virtual void    OnRadioHub_DataEvent( const Dictionary& /* result */ ){}
    };

private:
    //<! reference to client
    RadioHub::Client&   _Client;

    //<! DeviceManagement Service Access Point
    //DeviceManagement    _DeviceMgmt;

    //<! LoRaMeshRouter Service Access Point
    //LoRaMeshRouter      _LoRaMeshRouter;

    //<! Trace Service Access Point
    //Trace               _Trace;

    //<! SlipDecoder for incoming messages
    SlipDecoder         _SlipDecoder;

    //<! buffer for incoming messages
    SerialMessage       _RxMessage;

    //<! a serial port
    //QSerialPort         _Port;
    HardwareSerial&     _RadioSerial;

    //<! connection info
    Dictionary          _SerialInfo;

public:
                        RadioHub( RadioHub::Client& client, HardwareSerial& RadioSerial );

    //<! enable RadioHub for given portName
    //bool                Enable( const QString& portName );
    bool                Init( void );

    //<! accessor for DeviceManagement Service Access Point
    //DeviceManagement&   GetDeviceManagement() { return _DeviceMgmt; }

    //<! accessor for LoRa MeshRouter Service Access Point
    //LoRaMeshRouter&     GetLoRaMeshRouter() { return _LoRaMeshRouter; }

//public slots:
    //<! QSerialPort signal for available serial data
    void                OnSerialPort_ReadyRead( void );

    //getSerial
    HardwareSerial      GetSerial( void );


private:

    //<! process SLIP decoded HCI message
    void                OnSlipDecoder_MessageReady( const ByteArray& msg ) override;

};

#endif // _RadioHub_H_
