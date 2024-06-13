/**
 * @file    iM284A_L0.h
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


#ifndef _iM284A_L0_h_
#define _iM284A_L0_h_

#include "iM284A.h"

extern const Command_t Commands_L0[];
extern const uint8_t   cntCommands_L0;

//C function declarations
void printDemo( void );
void printUsage( void );
void quitDemo( void );
void PingDevice( void );
void GetDeviceInformation( void );
void GetFirmwareVersion( void );
void GetDateTime( void );
void SetDateTime( void );
void RestartDevice( void );
void GetSystemOptions( void );
void SetSystemOptions( void );
void GetNetworkAddress( void );
void SetNetworkAddress_A( void );
void SetNetworkAddress_B( void );
void GetMode( void );
void DisableRouter( void );
void EnableRouter( void );
void EnableCoordinator( void );
void GetLinkStatus( void );
void GetRoutingInfo( void );
void SendPacketToNode_A( void );
void SendPacketToNode_B( void );
void testRadioSerialMonitor( void );

#endif // _iM284A_L0_h_
