/**
 * @file    iM284A.c
 *
 * @brief   c file for a test of iM284A
 *
 * @note    This example code is free software: you can redistribute it and/or modify it.
 *
 *          This program is provided by EDI on an "AS IS" basis without
 *          any warranties in the hope that it will be useful.
 * 
 * Gatis Gaigals @ EDI, 2024
 */

#include "iM284A.h"
#include "iM284A_L0.h"

const CommandTables_t CommandTables[] = {
  { (const Command_t*)&Commands_L0[0], cntCommands_L0 }
//  , { (const Command_t*)&Commands_L0[0], cntCommands_L0 }
};

const uint8_t cntTables = sizeof( CommandTables ) / sizeof( CommandTables[0] );
