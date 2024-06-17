/**
 * @file    CommandTables.c
 *
 * @brief   c file for CommandTables
 *
 * @note    This example code is free software: you can redistribute it and/or modify it.
 *
 *          This program is provided by EDI on an "AS IS" basis without
 *          any warranties in the hope that it will be useful.
 * 
 * Gatis Gaigals @ EDI, 2024
 */

#include <stdint.h>
#include <Arduino.h>
#include "CommandTables.h"


volatile uint8_t  ActiveCommandTable = 0;


void showItem( const Command_t* Commands, uint8_t n ) {
  //show "  ESC: quit" or "  'C': description of the command" 
  uint8_t aKey = Commands[n].aKey;
  if ( ' ' == aKey ) {
    SerialUSB.write( "SPACE" );
  } else if ( 27 == aKey ) {
    SerialUSB.write( "  ESC" );
  } else {
    SerialUSB.write( "  \'" );
    //Serial.print( n );
    SerialUSB.write( char( aKey ) );
    SerialUSB.write('\'');
  }
  SerialUSB.write( ": " );
  SerialUSB.write( Commands[n].cDescription );
  SerialUSB.write( "\r\n" );
}

int16_t findItem( const Command_t* Commands, uint8_t CommandCount, uint8_t aKey ) {
  for ( uint8_t i = 0; i < CommandCount; i++ ) {
    if ( Commands[i].aKey == aKey ) {
      return i;
    }
  }
  return -1;
}
