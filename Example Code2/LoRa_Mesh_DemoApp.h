/**
 * @file    LoRa_Mesh_DemoApp.h
 *
 * @brief   Declaration of class LoRaMesh_DemoApp
 *
 * @note    This example code is free software: you can redistribute it and/or modify it.
 *
 *          This program is provided by IMST GmbH on an "AS IS" basis without
 *          any warranties in the hope that it will be useful.
 */

#ifndef __LoRa_Mesh_DemoApp_H__
#define __LoRa_Mesh_DemoApp_H__

#include "Utils/Console.h"
#include "RadioHub/RadioHub.h"

//<! example application which demonstrates the message exchange with WiMOD radio modules provided by IMST.
class LoRaMesh_DemoApp : public QObject
                       , public Console::KeyEventHandler
                       , public RadioHub::Client

{
public:
                           LoRaMesh_DemoApp         ( Console& console );

    //<! enable application
    void                   Enable                   ( const QString& portName );

private:

    //<! callback for keyboard inputevents
    void                   OnKeyEvent               ( int keyCode ) override;

    //<! special key coded
    enum KeyCodes
    {
        Key_Escape  =   27,
        Key_Space   =   32
    };

private:

    //<! commands related to this application
    void                    OnExit                  ();
    void                    OnShowMenu              ();

    //<! commands for DeviceManagement messages
    void                    OnPingDevice            ();

    void                    OnGetDeviceInformation  ();
    void                    OnGetFirmwareVersion    ();

    void                    OnGetDateTime           ();
    void                    OnSetDateTime           ();

    void                    OnRestartDevice         ();

    void                    OnGetSystemOptions      ();
    void                    OnSetSystemOptions      ();

    //<! commands for LoRaMesh Router messages
    void                    OnGetNetworkAddress     ();
    void                    OnSetNetworkAddress_A   ();
    void                    OnSetNetworkAddress_B   ();

    void                    OnGetMode               ();
    void                    OnDisableRouter         ();
    void                    OnEnableRouter          ();
    void                    OnEnableCoordinator     ();

    void                    OnGetLinkStatus         ();
    void                    OnGetRoutingInfo        ();

    void                    OnSendPacketToNode_A    ();
    void                    OnSendPacketToNode_B    ();

    //<! callback for incoming radio data eventa
    void                    OnRadioHub_DataEvent    ( const QJsonObject& result ) override;

private:

    void                    timerEvent              ( QTimerEvent* ev ) override;

    QString                 FindPortByVendorID      ();

private:

    //<! console for keyboard input and simple character output
    Console&                _Console;

    //<! RadioHub for communication with radio modules
    RadioHub                _RadioHub;

    //<! name of used serial port
    QString                 _PortName;


    //<! some application parameters
    QString                 _Network_ID;
    QString                 _DeviceEUI_Node_A;
    QString                 _DeviceEUI_Node_B;
    int                     _User_Port;
    QString                 _Payload_for_Node_A;
    QString                 _Payload_for_Node_B;

    //<! timer for port discovery
    int                     _TimerID;

    //<! prototype for menu handler
    typedef void (LoRaMesh_DemoApp::*FuncPtr)();

    //<! menu item
    struct MenuItem
    {
        //<! key code
        int                 KeyCode;

        //<! text for key
        const char*         KeyText;

        //<! text for menu item
        const char*         Text;

        //<! handler for menu action
        FuncPtr             Handler;
    };

    //<! main menu
    static const MenuItem   _MainMenu[];
};

#endif // __LoRa_Mesh_DemoApp_H__
