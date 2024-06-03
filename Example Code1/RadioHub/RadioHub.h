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

#ifndef __RadioHub_H__
#define __RadioHub_H__

#include "ServiceAccessPoints/DeviceManagement/DeviceManagement.h"
#include "Utils/SlipDecoder.h"

#include <QSerialPort>
#include <QJsonObject>
#include <QString>

//<! top level interface class for radio module communication
class RadioHub : public QObject
               , public SlipDecoder::Client
{
    // we weed the "ReadyRead" slot of QSerialPort for reception purpose
    Q_OBJECT

public:

    // declaration of client interface
    class Client
    {
    public:
        //<! callback interface for decoded data
        virtual void    OnRadioHub_DataEvent( const QJsonObject& /* result */ ){}

    };

                        RadioHub( RadioHub::Client& client );

    //<! enable RadioHub for given portName
    bool                Enable( const QString& portName );

    //<! accessor for DeviceManagement SAP
    DeviceManagement&   GetDeviceManagement() { return _DeviceMgmt; }

public slots:

    //<! QSerialPort signal for available serial data
    void                OnSerialPort_ReadyRead();

private:

    //<! process SLIP decoded HCI message
    void                OnSlipDecoder_MessageReady( const QByteArray& msg ) override;

private:

    //<! reference to client
    RadioHub::Client&   _Client;

    //<! DeviceManagement SAP
    DeviceManagement    _DeviceMgmt;

    //<! SlipDecoder for incoming messages
    SlipDecoder         _SlipDecoder;

    //<! buffer for incoming messages
    SerialMessage       _RxMessage;

    //<! a serial port
    QSerialPort         _Port;

    //<! connection info
    QJsonObject         _DeviceInfo;
};

#endif // __RadioHub_H__
