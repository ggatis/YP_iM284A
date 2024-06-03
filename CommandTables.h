/**
 * @file    CommandTables.h
 *
 * @brief   header file for CommandTables
 *
 * @note    This example code is free software: you can redistribute it and/or modify it.
 *
 *          This program is provided by EDI on an "AS IS" basis without
 *          any warranties in the hope that it will be useful.
 * 
 * Gatis Gaigals @ EDI, 2024
 */

#ifndef _CommandTables_h_
#define _CommandTables_h_

#include <stdint.h>

extern volatile uint8_t  ActiveCommandTable;

typedef struct {
  uint8_t           aKey;
  const char* const cDescription;
  void (*aFunction)( void );
} Command_t;

typedef struct {
  const Command_t*  pCommandTable;
  uint8_t           CommandCount;
} CommandTables_t;


//C function declarations
void    showItem( const Command_t* Commands, uint8_t n );
int16_t findItem( const Command_t* Commands, uint8_t CommandCount, uint8_t aKey );


#endif // _CommandTables_h_
