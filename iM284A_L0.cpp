/**
 * @file    iM284A_L0.c
 *
 * @brief   iM284A LVL0 API test commands
 *
 * @note    This example code is free software: you can redistribute it and/or modify it.
 *
 *          This program is provided by EDI on an "AS IS" basis without
 *          any warranties in the hope that it will be useful.
 * 
 * Gatis Gaigals @ EDI, 2024
 */

#include <stdint.h>
//#include <Arduino.h>

#include "iM284A.h"
#include "iM284A_L0.h"

#include "LoRa_Mesh_DemoApp.h"
extern LoRaMesh_DemoApp* pDemoApp;


const char cDescription00[] = "print usage";
const char cDescription0Q[] = "quit";
const char cDescription0A[] = "Device Management Services";
const char cDescription01[] = "ping device";
const char cDescription02[] = "get Device Information";
const char cDescription03[] = "get Firmware Version";
const char cDescription04[] = "get Date and Time";
const char cDescription05[] = "set Date and Time";
const char cDescription06[] = "restart Target Device";
const char cDescription07[] = "get System Options";
const char cDescription08[] = "set System Options";
const char cDescription0B[] = "LoRa Mesh Router";
const char cDescription0a[] = "get Network Address";
const char cDescription0b[] = "set Network Address(A)";
const char cDescription0c[] = "set Network Address(B)";
const char cDescription0d[] = "get Mode";
const char cDescription0e[] = "disable Router";
const char cDescription0f[] = "enable Router";
const char cDescription0g[] = "enable Coordinator";
const char cDescription0h[] = "get Link Status";
const char cDescription0i[] = "get Routing Info";
const char cDescription0j[] = "send Packet to Node(A)";
const char cDescription0k[] = "send Packet to Node(B)";
const char cDescription0C[] = "Misc";
const char cDescription0p[] = "print demo setup";
const char cDescription0t[] = "test radio serial monitor";

const Command_t Commands_L0[] = {
  { ' ', cDescription00, &printUsage },
  {  27, cDescription0Q, &quitDemo },
  { '-', cDescription0A, nullptr },
  { '1', cDescription01, &PingDevice },
  { '2', cDescription02, &GetDeviceInformation },
  { '3', cDescription03, &GetFirmwareVersion },
  { '4', cDescription04, &GetDateTime },
  { '5', cDescription05, &SetDateTime },
  { '6', cDescription06, &RestartDevice },
  { '7', cDescription07, &GetSystemOptions },
  { '8', cDescription08, &SetSystemOptions },
  { '-', cDescription0B, nullptr },
  { 'a', cDescription0a, &GetNetworkAddress },
  { 'b', cDescription0b, &SetNetworkAddress_A },
  { 'c', cDescription0c, &SetNetworkAddress_B },
  { 'd', cDescription0d, &GetMode },
  { 'e', cDescription0e, &DisableRouter },
  { 'f', cDescription0f, &EnableRouter },
  { 'g', cDescription0g, &EnableCoordinator },
  { 'h', cDescription0h, &GetLinkStatus },
  { 'i', cDescription0i, &GetRoutingInfo },
  { 'j', cDescription0j, &SendPacketToNode_A },
  { 'k', cDescription0k, &SendPacketToNode_B },
  { '-', cDescription0C, nullptr },
  { 'p', cDescription0p, &printDemo },
  { 't', cDescription0t, &testRadioSerialMonitor }
};

const uint8_t cntCommands_L0 = sizeof( Commands_L0 ) / sizeof( Commands_L0[0] );


/*************************************************/

void printUsage( void ) {
  for ( 
    uint8_t i = 0;
    i < CommandTables[ActiveCommandTable].CommandCount;
    showItem( CommandTables[ActiveCommandTable].pCommandTable, i++ )
  );
}

void quitDemo( void ) {
    //SerialUSB.println( F("Quit?") );
    printf("Quit?");
    pDemoApp->OnExit();
}


/*** Device Management Services ***/

void PingDevice( void ) {
    //SerialUSB.println( F("PingDevice") );
    printf("PingDevice");
    pDemoApp->OnPingDevice();
}

void GetDeviceInformation( void ) {
    //SerialUSB.println( F("GetDeviceInformation") );
    printf("GetDeviceInformation");
    pDemoApp->OnGetDeviceInformation();
}

void GetFirmwareVersion( void ) {
    //SerialUSB.println( F("GetFirmwareVersion") );
    printf("GetFirmwareVersion");
    pDemoApp->OnGetFirmwareVersion();
}

void GetDateTime( void ) {
    //SerialUSB.println( F("GetDateTime") );
    printf("GetDateTime");
    pDemoApp->OnGetDateTime();
}

void SetDateTime( void ) {
    //SerialUSB.println( F("SetDateTime") );
    printf("SetDateTime");
    pDemoApp->OnSetDateTime();
}

void RestartDevice( void ) {
    //SerialUSB.println( F("RestartDevice") );
    printf("RestartDevice");
    pDemoApp->OnRestartDevice();
}

void GetSystemOptions( void ) {
    //SerialUSB.println( F("GetSystemOptions") );
    printf("GetSystemOptions");
    pDemoApp->OnGetSystemOptions();
}

void SetSystemOptions( void ) {
    //SerialUSB.println( F("SetSystemOptions") );
    printf("SetSystemOptions");
    pDemoApp->OnGetSystemOptions();
}


/*** LoRa Mesh Router ***/

void GetNetworkAddress( void ) {
    //SerialUSB.println( F("GetNetworkAddress") );
    printf("GetNetworkAddress");
    pDemoApp->OnGetNetworkAddress();
}

void SetNetworkAddress_A( void ) {
    //SerialUSB.println( F("SetNetworkAddress_A") );
    printf("SetNetworkAddress_A");
    pDemoApp->OnSetNetworkAddress_A();
}

void SetNetworkAddress_B( void ) {
    //SerialUSB.println( F("SetNetworkAddress_B") );
    printf("SetNetworkAddress_B");
    pDemoApp->OnSetNetworkAddress_B();
}

void GetMode( void ) {
    //SerialUSB.println( F("GetMode") );
    printf("GetMode");
    pDemoApp->OnGetMode();
}

void DisableRouter( void ) {
    //SerialUSB.println( F("DisableRouter") );
    printf("DisableRouter");
    pDemoApp->OnDisableRouter();
}

void EnableRouter( void ) {
    //SerialUSB.println( F("EnableRouter") );
    printf("EnableRouter");
    pDemoApp->OnEnableRouter();
}

void EnableCoordinator( void ) {
    //SerialUSB.println( F("EnableCoordinator") );
    printf("EnableCoordinator");
    pDemoApp->OnEnableCoordinator();
}

void GetLinkStatus( void ) {
    //SerialUSB.println( F("GetLinkStatus") );
    printf("GetLinkStatus");
    pDemoApp->OnGetLinkStatus();
}

void GetRoutingInfo( void ) {
    //SerialUSB.println( F("GetRoutingInfo") );
    printf("GetRoutingInfo");
    pDemoApp->OnGetRoutingInfo();
}

void SendPacketToNode_A( void ) {
    //SerialUSB.println( F("SendPacketToNode_A") );
    printf("SendPacketToNode_A");
    pDemoApp->OnSendPacketToNode_A();
}

void SendPacketToNode_B( void ) {
    //SerialUSB.println( F("SendPacketToNode_B") );
    printf("SendPacketToNode_B");
    pDemoApp->OnSendPacketToNode_B();
}


/*** Rest ***/

void printDemo( void ) {
    pDemoApp->print();
}

void testRadioSerialMonitor( void ) {
    //SerialUSB.println( F("testRadioSerialMonitor") );
    printf("testRadioSerialMonitor");
    pDemoApp->TestRadioSerialMonitor();
}
