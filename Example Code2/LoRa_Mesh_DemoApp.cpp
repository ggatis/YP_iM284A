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

#include "LoRa_Mesh_DemoApp.h"
#include "Utils/Console.h"

#include <QCoreApplication>
#include <QSerialPortInfo>

//<! table with main menu
const LoRaMesh_DemoApp::MenuItem
LoRaMesh_DemoApp::_MainMenu[] =
{
    { '-',          nullptr,    "Device Management Services",               nullptr },
    { '1',          "1",        "ping target device",                       &LoRaMesh_DemoApp::OnPingDevice },
    { '2',          "2",        "get Device Information",                   &LoRaMesh_DemoApp::OnGetDeviceInformation },
    { '3',          "3",        "get Firmware Version",                     &LoRaMesh_DemoApp::OnGetFirmwareVersion },
    { '4',          "4",        "get Date and Time",                        &LoRaMesh_DemoApp::OnGetDateTime },
    { '5',          "5",        "set Date and Time",                        &LoRaMesh_DemoApp::OnSetDateTime },
    { '6',          "6",        "restart Target Device",                    &LoRaMesh_DemoApp::OnRestartDevice },
    { '7',          "7",        "get System Options",                       &LoRaMesh_DemoApp::OnGetSystemOptions },
    { '8',          "8",        "set System Options",                       &LoRaMesh_DemoApp::OnSetSystemOptions },
    { '-',          nullptr,    "LoRa Mesh Router",                         nullptr },
    { 'a',          "a",        "get Network Address",                      &LoRaMesh_DemoApp::OnGetNetworkAddress },
    { 'b',          "b",        "set Network Address(A)",                   &LoRaMesh_DemoApp::OnSetNetworkAddress_A },
    { 'c',          "c",        "set Network Address(B)",                   &LoRaMesh_DemoApp::OnSetNetworkAddress_B },
    { 'd',          "d",        "get Mode",                                 &LoRaMesh_DemoApp::OnGetMode },
    { 'e',          "e",        "disable Router",                           &LoRaMesh_DemoApp::OnDisableRouter },
    { 'f',          "f",        "enable Router",                            &LoRaMesh_DemoApp::OnEnableRouter },
    { 'g',          "g",        "enable Coordinator",                       &LoRaMesh_DemoApp::OnEnableCoordinator },
    { 'h',          "h",        "get Link Status",                          &LoRaMesh_DemoApp::OnGetLinkStatus },
    { 'i',          "i",        "get Routing Info",                         &LoRaMesh_DemoApp::OnGetRoutingInfo },
    { 'j',          "j",        "send Packet to Node(A)",                   &LoRaMesh_DemoApp::OnSendPacketToNode_A },
    { 'k',          "k",        "send Packet to Node(B)",                   &LoRaMesh_DemoApp::OnSendPacketToNode_B },
    { '-',          nullptr,    nullptr,                                    nullptr },
    { Key_Space,    "SPACE",    "show this menu",                           &LoRaMesh_DemoApp::OnShowMenu },
    { Key_Escape,   "Escape",   "exit application",                         &LoRaMesh_DemoApp::OnExit }
};

/**
 * @brief   class constructor of main application
 * @param   console     reference to a console
 */

LoRaMesh_DemoApp::LoRaMesh_DemoApp( Console& console )
             : _Console             ( console )
             , _RadioHub            ( *this )
             , _PortName            ( "none" )
             , _Network_ID          ( "00-2A" )
             , _DeviceEUI_Node_A    ( "01-aa-aa-aa-02-aa-aa-aa" )
             , _DeviceEUI_Node_B    ( "03-bb-bb-bb-04-bb-bb-bb" )
             // a user port range( 1 - 127 ), reserved( 0, 128 .. 255 )
             , _User_Port           ( 21 )
             , _Payload_for_Node_A  ( "AA-01-02-02-04-05-06-07-08-AA" )
             , _Payload_for_Node_B  ( "BB-01-02-02-04-05-06-07-08-09-0A-0B-0C-0D-0E-0F-BB" )
             , _TimerID ( -1 )
{
    _Console.ClearScreen();
    _Console.Print( "This application demonstrates the host controller message protocol for WiMOD radio modules provided by IMST.\n" );
    _Console.Print( "Please connect a WiMOD radio module or WiMOD USB Stick.\n" );
    _Console.Print( "Don't forget to install the required USB to UART Bridge driver.\n\n" );
}

/**
 * @brief   enable application
 *
 * @param   portName    name of serial port, e.g.: "ttyUSB", "COM1"
 */

void
LoRaMesh_DemoApp::Enable( const QString& portName )
{
    // register for keyboard input events
    _Console.RegisterKeyEventHandler( this );

    if ( ( portName.isEmpty() == false ) && _RadioHub.Enable( portName ) )
    {
        _PortName = portName;

        OnShowMenu();

        return;
    }

    // start port discovery timer
    _TimerID = startTimer( 100 );
}

/**
 * @brief handle keyboard events from console
 *
 * @param keyCode
 */
void
LoRaMesh_DemoApp::OnKeyEvent( int keyCode )
{
    for ( int i = 0; i < (int)( sizeof( _MainMenu ) / sizeof( MenuItem ) ); i++ )
    {
        const MenuItem* item = &_MainMenu[ i ];

        if ( ( item->Handler != nullptr ) && ( item->KeyCode == keyCode ) )
        {
            _Console.Print( "" );
            _Console.Print( "Command", item->Text );

            FuncPtr handler = item->Handler;

            // call handler
            ( this->*handler )();

            break;
        }
    }
}

/**
 * @brief   show the main menu
 */
void
LoRaMesh_DemoApp::OnShowMenu()
{
    _Console.Print( "" );
    _Console.Print( "Used Port", _PortName );
    _Console.Print( "" );

    for ( int i = 0; i < (int)( sizeof( _MainMenu ) / sizeof( MenuItem ) ); i++ )
    {
        const MenuItem* item = &_MainMenu[ i ];

        if ( item->KeyText == nullptr )
        {
            _Console.PrintLine();
            if ( item->Text )
            {
                _Console.Print( item->Text, "" );
                _Console.PrintLine();
            }
        }
        else
        {
            _Console.Print( item->KeyText, item->Text );
        }
    }

    _Console.PrintLine();
    _Console.Print( "" );
    _Console.Print( "Select a command", ">\n" );
}

/**
 *  @brief  some command handler
 */
void
LoRaMesh_DemoApp::OnExit()
{
    _Console.Print( "\ngoodbye!\n" );
    QCoreApplication::quit();
}

void
LoRaMesh_DemoApp::OnPingDevice()
{
    _RadioHub.GetDeviceManagement().OnPingDevice();
}

void
LoRaMesh_DemoApp::OnGetDeviceInformation()
{
    _RadioHub.GetDeviceManagement().OnGetDeviceInfo();
}

void
LoRaMesh_DemoApp::OnGetFirmwareVersion()
{
    _RadioHub.GetDeviceManagement().OnGetFirmwareVersion();
}

void
LoRaMesh_DemoApp::OnGetDateTime()
{
    _RadioHub.GetDeviceManagement().OnGetDateTime();
}

void
LoRaMesh_DemoApp::OnSetDateTime()
{
    _RadioHub.GetDeviceManagement().OnSetDateTime();
}

void
LoRaMesh_DemoApp::OnRestartDevice()
{
    _RadioHub.GetDeviceManagement().OnRestartDevice();
}

void
LoRaMesh_DemoApp::OnGetSystemOptions()
{
    _RadioHub.GetDeviceManagement().OnGetSystemOptions();
}

void
LoRaMesh_DemoApp::OnSetSystemOptions()
{
    QJsonObject params;

    params[ "Options" ] = "Trace = off, Startup Event = on";

    _RadioHub.GetDeviceManagement().OnSetSystemOptions( params );
}

// LoRaMesh Router Commands

void
LoRaMesh_DemoApp::OnGetNetworkAddress()
{
    _RadioHub.GetLoRaMeshRouter().OnGetNetworkAddress();
}

void
LoRaMesh_DemoApp::OnSetNetworkAddress_A()
{
    QJsonObject params;

    params[ "Network-ID" ]  =   _Network_ID;
    params[ "Device-EUI" ]  =   _DeviceEUI_Node_A;

    _RadioHub.GetLoRaMeshRouter().OnSetNetworkAddress( params );
}

void
LoRaMesh_DemoApp::OnSetNetworkAddress_B()
{
    QJsonObject params;

    params[ "Network-ID" ]  =   _Network_ID;
    params[ "Device-EUI" ]  =   _DeviceEUI_Node_B;

    _RadioHub.GetLoRaMeshRouter().OnSetNetworkAddress( params );
}

void
LoRaMesh_DemoApp::OnGetMode()
{
    _RadioHub.GetLoRaMeshRouter().OnGetMode();
}

void
LoRaMesh_DemoApp::OnDisableRouter()
{
    QJsonObject params;

    params[ "Mode" ]  = "Off";

    _RadioHub.GetLoRaMeshRouter().OnSetMode( params );
}

void
LoRaMesh_DemoApp::OnEnableRouter()
{
    QJsonObject params;

    params[ "Mode" ]  = "Router";

    _RadioHub.GetLoRaMeshRouter().OnSetMode( params );
}

void
LoRaMesh_DemoApp::OnEnableCoordinator()
{
    QJsonObject params;

    params[ "Mode" ]  = "Coordinator";

    _RadioHub.GetLoRaMeshRouter().OnSetMode( params );
}

void
LoRaMesh_DemoApp::OnGetLinkStatus()
{
    _RadioHub.GetLoRaMeshRouter().OnGetLinkStatus();
}

void
LoRaMesh_DemoApp::OnGetRoutingInfo()
{
    QJsonObject params;

    // start index
    params[ "Index" ]       = 0;

    // read MaxItems per request
    params[ "MaxItems" ]    = 4;

    _RadioHub.GetLoRaMeshRouter().OnGetRoutingInfo( params );
}

void
LoRaMesh_DemoApp::OnSendPacketToNode_A()
{
    QJsonObject params;

    params[ "Destination-EUI" ] = _DeviceEUI_Node_A;
    params[ "Port" ]            = _User_Port;
    params[ "Payload" ]         = _Payload_for_Node_A;

    _RadioHub.GetLoRaMeshRouter().OnSendPacket( params );
}

void
LoRaMesh_DemoApp::OnSendPacketToNode_B()
{
    QJsonObject params;

    params[ "Destination-EUI" ] = _DeviceEUI_Node_B;
    params[ "Port" ]            = _User_Port;
    params[ "Payload" ]         = _Payload_for_Node_B;

    _RadioHub.GetLoRaMeshRouter().OnSendPacket( params );
}


/**
 * @brief   print results for incoming radio events and response
 *
 * @param   result      decoded data from radio module in Json format
 */

void
LoRaMesh_DemoApp::OnRadioHub_DataEvent( const QJsonObject& result )
{
    QJsonObject data = result;

    // print results in a certain order
    QStringList order; order << "Event" << "Status";

    for ( const QString& key : order )
    {
        if ( data.contains( key ) )
        {
            _Console.Print( key, data.value( key ).toString() );
            data.remove( key );
        }
    }
    _Console.PrintResults( data );
    _Console.PrintLine();
}

/**
 * @brief   handle timer event, used for port discovery
 */

void
LoRaMesh_DemoApp::timerEvent( QTimerEvent* /* ev */ )
{
    QString portName = FindPortByVendorID();

    if ( portName.isEmpty() )
        return;

    if ( _RadioHub.Enable( portName ) )
    {
        // stop timer
        killTimer( _TimerID );
        _TimerID = -1;

        _PortName = portName;

        OnShowMenu();
    }
}

/**
 * @brief   find a serial port name by VendorID
 *
 * @return  first portName of avaialable ports
 *
 */

QString
LoRaMesh_DemoApp::FindPortByVendorID()
{
    enum VendorIDs
    {
        VID_FTDI    = 0x0403,   // Vendor ID for FTDI
        VID_SILABS  = 0x10C4,   // Vendor ID for Silicon Labs
        VID_STM32   = 0x0483,   // Vendor ID for STM32
        VID_CYPRESS = 0x04B4    // Vendor ID for Cypress, used on iU881A-XL
     };

    QList < uint16_t > supportedVendorIDs;

    supportedVendorIDs.append( VID_FTDI );
    supportedVendorIDs.append( VID_SILABS );
    supportedVendorIDs.append( VID_STM32 );
    supportedVendorIDs.append( VID_CYPRESS );

    QList < QSerialPortInfo > portInfos = QSerialPortInfo::availablePorts();

    for ( const QSerialPortInfo& portInfo : portInfos )
    {
        if ( supportedVendorIDs.contains( portInfo.vendorIdentifier() ) )
        {
#ifdef  Q_OS_OSX
            return portInfo.portName().replace( "cu.", "tty." );
#else
            return portInfo.portName();
#endif
        }
    }
    return QString();
}
