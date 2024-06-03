/**
 * @file    iM284A.h
 *
 * @brief   header file for a test of iM284A
 *
 * @note    This example code is free software: you can redistribute it and/or modify it.
 *
 *          This program is provided by EDI on an "AS IS" basis without
 *          any warranties in the hope that it will be useful.
 * 
 * Gatis Gaigals @ EDI, 2024
 */

#ifndef _iM284A_h_
#define _iM284A_h_

#include "CommandTables.h"
#include "LoRa_Mesh_DemoApp.h"

extern const CommandTables_t  CommandTables[];
extern const uint8_t          cntTables;

extern       LoRaMesh_DemoApp DemoApp;

#endif // _iM284A_h_
