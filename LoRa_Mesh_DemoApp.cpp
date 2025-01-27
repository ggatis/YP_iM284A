/**
 * @file    LoRaMesh_DemoApp.cpp
 *
 * @brief   Implementation of class LoRaMesh_DemoApp
 *
 * @note    This example code is free software: you can redistribute it and/or modify it.
 *
 *          This program is provided by IMST GmbH on an "AS IS" basis without
 *          any warranties in the hope that it will be useful.
 */

#include "Dictionary.h"
#include "LoRa_Mesh_DemoApp.h"
#include "HardwareSerial.h"
//#include "Utils/Console.h"

//#include <QCoreApplication>
//#include <QSerialPortInfo>

/**
 * @brief   class constructor of main application
 * @param   console     reference to a console
 */

//LoRaMesh_DemoApp::LoRaMesh_DemoApp( HardwareSerial& RadioSerial, USBSerial& HMISerial ) :
LoRaMesh_DemoApp::LoRaMesh_DemoApp( HardwareSerial& RadioSerial ) :
    RadioHub            ( *this, RadioSerial ),
    //QString             _PortName;
    //_HMISerial          ( HMISerial ),
    _Network_ID         ( (char*)"00-2A" ),
    _DeviceEUI_Node_A   ( (char*)"01-aa-aa-aa-02-aa-aa-aa" ),
    _DeviceEUI_Node_B   ( (char*)"03-bb-bb-bb-04-bb-bb-bb" ),
    _User_Port          ( 21 ),
    _Payload_for_Node_A ( (char*)"AA-01-02-02-04-05-06-07-08-AA" ),
    _Payload_for_Node_B ( (char*)"BB-01-02-02-04-05-06-07-08-09-0A-0B-0C-0D-0E-0F-BB" ) {

    printf("\r\nThis application demonstrates the host controller message protocol for WiMOD radio modules provided by IMST.\r\n");
    printf("Please connect a WiMOD radio module or WiMOD USB Stick.\r\n");
    printf("OOpsy - it's too late for this already. Connect it and restart the microcontroller if needed!\r\n");
    printf("Don't forget to install the required USB to UART Bridge driver.\r\n");
    printf("Actually if you are reading this then don't worry about that driver!\r\n\r\n");

}


void
LoRaMesh_DemoApp::print( void ) {
    Dictionary DemoInfo;
    DemoInfo.append( "Network_ID",          _Network_ID );
    DemoInfo.append( "DeviceEUI_Node_A",    _DeviceEUI_Node_A );
    DemoInfo.append( "DeviceEUI_Node_B",    _DeviceEUI_Node_B );
    DemoInfo.append( "User_Port",           "21" );
    DemoInfo.append( "Payload_for_Node_A",  _Payload_for_Node_A );
    DemoInfo.append( "Payload_for_Node_B",  _Payload_for_Node_B );
    DemoInfo.print();
}


/**
 *  @brief  some command handler
 */
void
LoRaMesh_DemoApp::OnExit( void ) {
    printf("Seriously?\r\n");
    //QCoreApplication::quit();
}

void
LoRaMesh_DemoApp::OnPingDevice( void ) {
    GetDeviceManagement().OnPingDevice();
}

void
LoRaMesh_DemoApp::OnGetDeviceInformation( void ) {
    GetDeviceManagement().OnGetDeviceInfo();
}

void
LoRaMesh_DemoApp::OnGetFirmwareVersion( void ) {
    GetDeviceManagement().OnGetFirmwareVersion();
}

void
LoRaMesh_DemoApp::OnGetDateTime( void ) {
    GetDeviceManagement().OnGetDateTime();
}

void
LoRaMesh_DemoApp::OnSetDateTime( void ) {
    GetDeviceManagement().OnSetDateTime();
}

void
LoRaMesh_DemoApp::OnRestartDevice( void ) {
    GetDeviceManagement().OnRestartDevice();
}

void
LoRaMesh_DemoApp::OnGetSystemOptions( void ) {
    GetDeviceManagement().OnGetSystemOptions();
}

void
LoRaMesh_DemoApp::OnSetSystemOptions( void ) {
    //QJsonObject params;
    Dictionary params;

    //params[ "Options" ] = "Trace = off, Startup Event = on";
    params.append("Options", "Trace = off, Startup Event = on");
    //_RadioHub.GetDeviceManagement().OnSetSystemOptions( params );
    GetDeviceManagement().OnSetSystemOptions( params );

}

// LoRaMesh Router Commands

void
LoRaMesh_DemoApp::OnGetNetworkAddress( void )
{
    //_RadioHub.GetLoRaMeshRouter().OnGetNetworkAddress();
}

void
LoRaMesh_DemoApp::OnSetNetworkAddress_A( void )
{
    //QJsonObject params;

    //params[ "Network-ID" ]  =   _Network_ID;
    //params[ "Device-EUI" ]  =   _DeviceEUI_Node_A;

    //_RadioHub.GetLoRaMeshRouter().OnSetNetworkAddress( params );
}

void
LoRaMesh_DemoApp::OnSetNetworkAddress_B( void ) {
    //QJsonObject params;

    //params[ "Network-ID" ]  =   _Network_ID;
    //params[ "Device-EUI" ]  =   _DeviceEUI_Node_B;

    //_RadioHub.GetLoRaMeshRouter().OnSetNetworkAddress( params );
}

void
LoRaMesh_DemoApp::OnGetMode( void ) {
    //_RadioHub.GetLoRaMeshRouter().OnGetMode();
}

void
LoRaMesh_DemoApp::OnDisableRouter( void ) {
    //QJsonObject params;

    //params[ "Mode" ]  = "Off";

    //_RadioHub.GetLoRaMeshRouter().OnSetMode( params );
}

void
LoRaMesh_DemoApp::OnEnableRouter( void ) {
    //QJsonObject params;

    //params[ "Mode" ]  = "Router";

    //_RadioHub.GetLoRaMeshRouter().OnSetMode( params );
}

void
LoRaMesh_DemoApp::OnEnableCoordinator( void ) {
    //QJsonObject params;

    //params[ "Mode" ]  = "Coordinator";

    //_RadioHub.GetLoRaMeshRouter().OnSetMode( params );
}

void
LoRaMesh_DemoApp::OnGetLinkStatus( void ) {
    //_RadioHub.GetLoRaMeshRouter().OnGetLinkStatus();
}

void
LoRaMesh_DemoApp::OnGetRoutingInfo( void ) {
    //QJsonObject params;

    //// start index
    //params[ "Index" ]       = 0;

    //// read MaxItems per request
    //params[ "MaxItems" ]    = 4;

    //_RadioHub.GetLoRaMeshRouter().OnGetRoutingInfo( params );
}

void
LoRaMesh_DemoApp::OnSendPacketToNode_A( void ) {
    //QJsonObject params;

    //params[ "Destination-EUI" ] = _DeviceEUI_Node_A;
    //params[ "Port" ]            = _User_Port;
    //params[ "Payload" ]         = _Payload_for_Node_A;

    //_RadioHub.GetLoRaMeshRouter().OnSendPacket( params );
}

void
LoRaMesh_DemoApp::OnSendPacketToNode_B( void ) {
    //QJsonObject params;

    //params[ "Destination-EUI" ] = _DeviceEUI_Node_B;
    //params[ "Port" ]            = _User_Port;
    //params[ "Payload" ]         = _Payload_for_Node_B;

    //_RadioHub.GetLoRaMeshRouter().OnSendPacket( params );
}

void
LoRaMesh_DemoApp::TestRadioSerialMonitor( void ) {
    printf("Sending ZZZ\r\n");
    GetSerial().write("ZZZZZ");
    //Serial1.write("YYYYY");
    printf("Done!\r\n");
}


/**
 * @brief   print results for incoming radio events and response
 *
 * @param   result      decoded data from radio module in Json format
 */

//void
//LoRaMesh_DemoApp::OnRadioHub_DataEvent( const QJsonObject& result )
//void
//LoRaMesh_DemoApp::OnRadioHub_DataEvent( const ByteArray& result ) {
void
LoRaMesh_DemoApp::OnRadioHub_DataEvent( const Dictionary& result ) {

    //debug-vvv
    printf("LoRaMesh_DemoApp::OnRadioHub_DataEvent\r\n");
    //debug-^^^

    const char* key0 = "Event";
    const char* key1 = "Status";
    const char* keys[] = { key0, key1 };

#if 1
    //A
    Dictionary  data = result;

    for ( uint8_t i = 0; i < ( sizeof( keys ) / sizeof( keys[0] ) ); i++ ) {

        const char*     key = keys[i];
        const uint8_t*  keydata = data.contains( (const uint8_t*)key );

        if ( keydata ) {
            //_HMISerial.print( key );
            printf( key );
            //_HMISerial.print( " : " );
            printf(" : ");
            //_HMISerial.print( (const char*)keydata );
            printf( (const char*)keydata );
            data.remove( (const uint8_t*)key );
        }
    }
    data.print();
    printf("\r\n");

#else
    //B
    result.print( keys, ( sizeof( keys ) / sizeof( keys[0] ) ) );
    result.print( keys, ( sizeof( keys ) / sizeof( keys[0] ) ), true );

#endif
}
