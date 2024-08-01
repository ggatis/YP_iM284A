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

#ifndef _LoRa_Mesh_DemoApp_H_
#define _LoRa_Mesh_DemoApp_H_


#include "Dictionary.h"     //also "ByteArray.h"

//#include "Utils/Console.h"
#include "RadioHub.h"


//<! example application which demonstrates the message exchange with WiMOD radio modules provided by IMST.
//class LoRaMesh_DemoApp : public QObject
//                       , public Console::KeyEventHandler
//                       , public RadioHub::Client
//class LoRaMesh_DemoApp : public RadioHub::Client {
class LoRaMesh_DemoApp : public RadioHub, public RadioHub::Client {
//class LoRaMesh_DemoApp {
public:
    //                        LoRaMesh_DemoApp         ( Console& console );
    //                        LoRaMesh_DemoApp        ( HardwareSerial& RadioSerial, USBSerial& HMISerial );
                            LoRaMesh_DemoApp        ( HardwareSerial& RadioSerial );

private:

    //<! callback for keyboard inputevents
    //void                   OnKeyEvent               ( int keyCode ) override;

    //<! special key coded
    //enum KeyCodes
    //{
    //    Key_Escape  =   27,
    //    Key_Space   =   32
    //};


    //void                    timerEvent              ( QTimerEvent* ev ) override;

    //QString                 FindPortByVendorID      ();

    //<! console for keyboard input and simple character output
    //Console&                _Console;

    //<! RadioHub for communication with radio modules
    //not instanced but inherited
    //RadioHub                _RadioHub;

    //<! name of used serial port
    //QString                 _PortName;
    //HardwareSerial&         _RadioSerial;
    //USBSerial&              _HMISerial;

    //<! some application parameters
    char*                   _Network_ID;
    char*                   _DeviceEUI_Node_A;
    char*                   _DeviceEUI_Node_B;
    int                     _User_Port;
    char*                   _Payload_for_Node_A;
    char*                   _Payload_for_Node_B;

    //<! timer for port discovery
    //int                     _TimerID;

    //<! prototype for menu handler
    //typedef void (LoRaMesh_DemoApp::*FuncPtr)();

    //<! menu item
    //struct MenuItem {
    //    //<! key code
    //    int                 KeyCode;
    //
    //    //<! text for key
    //    const char*         KeyText;
    //
    //    //<! text for menu item
    //    const char*         Text;
    //
    //    //<! handler for menu action
    //    FuncPtr             Handler;
    //};

    //<! main menu
    //static const MenuItem   _MainMenu[];

public:
    void                    print                   ( void );

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

    void                    TestRadioSerialMonitor  ();

    //<! callback for incoming radio data eventa
    void                    OnRadioHub_DataEvent    ( const Dictionary& result ) override;

};

#endif // _LoRa_Mesh_DemoApp_H_
