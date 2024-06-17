/**
 * @file    YP_iM284A.ino
 *
 * @brief   Test of iM284A
 *
 * @note    This example code is free software: you can redistribute it and/or modify it.
 *
 *          This program is provided by EDI on an "AS IS" basis without
 *          any warranties in the hope that it will be useful.
 * 
 * Gatis Gaigals @ EDI, 2024
 * 
 */


volatile uint32_t mySysTick = 0;
volatile uint32_t lastS2IOtick;
volatile uint8_t  ActiveCommand = 0;


#include "YP_iM284A.h"
//#include "Utils/Console.h"

#include "ByteArray.h"

ByteArray* pRaMonBuff;

#include "CRC16.h"
#include "SlipDecoder.h"
#include "SlipEncoder.h"

#include "iM284A.h"
#include "iM284A_L0.h"  //for "print usage"

#include "HardwareSerial.h"
HardwareSerial Serial1( USART1 );  //Radio
HardwareSerial Serial2( USART2 );  //monitor
//HardwareSerial Serial4( USART4 );
HardwareSerial Serial5( USART5 );

#include "CurrentTime.h"

#include "LoRa_Mesh_DemoApp.h"
LoRaMesh_DemoApp* pDemoApp;

//LED
#define LED_BUILTIN   13

void LEDon( void ) {
  digitalWrite( LED_BUILTIN, LOW );
}

void LEDoff( void ) {
  digitalWrite( LED_BUILTIN, HIGH );
}

void LEDset( uint8_t state ) {
  if ( state ) {
    LEDon();
  } else {
    LEDoff();
  }
}

void LEDtoggle( void ) {
  if ( HIGH == digitalRead( LED_BUILTIN ) ) {
    digitalWrite( LED_BUILTIN, LOW );
  } else {
    digitalWrite( LED_BUILTIN, HIGH );
  }
}

void LEDinit( void ) {
  pinMode( LED_BUILTIN, OUTPUT );
  LEDon();
}

#define ON_time      100
#define ONOFF_time  1000

static uint8_t  LEDstate = 1;

void LEDhandler( void ) {
  static uint32_t LastSysTick = 0;
  uint32_t NewSysTick = mySysTick;
  if ( LEDstate ) {
    if ( NewSysTick > ( LastSysTick + ON_time ) ) {
      LEDstate = 0;
      LEDoff();
      LastSysTick = LastSysTick ? LastSysTick + ON_time : NewSysTick;
    }
  } else {
    if ( NewSysTick > ( LastSysTick + ONOFF_time - ON_time  ) ) {
      LEDstate = 1;
      LEDon();
      LastSysTick = LastSysTick + ONOFF_time - ON_time;
    }
  }
}

/*********************************************************************/
/*                               Timer                               */
/*********************************************************************/

void _100msCallback( void ) {
  //10hz
  static uint8_t mSeconds100 = 9;
  if ( 9 == mSeconds100 ) {
    mSeconds100 = 0;
  } else {
    mSeconds100++;
  }
  if ( 0 == mSeconds100 ) {
    LEDoff();
    //Serial1.print( (char)( mySysTick % 256 ) );
  } else if ( 9 == mSeconds100 ) {
    LEDon();
  }
#if 0 == _SYSTIME_
  addCurrentTimeInUSeconds( 100000 );
#endif
}


void serialEvent1() {
    while ( 0 < Serial1.available() ) {
        LEDtoggle();
        char c = Serial1.read();
    }
}


void serialEvent2() {
    lastS2IOtick = mySysTick;
    while ( 0 < Serial2.available() ) {
        LEDtoggle();
        char c = Serial2.read();
        pRaMonBuff->append( (uint8_t)c );
        //Serial2.read();
    }
    //Serial2.read();
}


////

void setup( void ) {

  LEDinit();
  
  //Initialize serial and wait for port to open:
  SerialUSB.begin( 115200 );
  while ( !SerialUSB ) {
    ;   //wait for serial port to connect. Needed for native USB port only
  }
  LEDoff();
  SerialUSB.println( F("") );
  SerialUSB.println( F("") );
  SerialUSB.println( F("") );
  SerialUSB.println( F("YP_iM284A.ino: test of iM284A HCI v0.2:") );
  
  printUsage();

#if 0 == _SYSTIME_
  initCurrentTime();
#endif

#if defined( TIM1 )
  TIM_TypeDef *Instance = TIM1;
#else
  TIM_TypeDef *Instance = TIM2;
#endif

  //Instantiate HardwareTimer object.
  //Thanks to 'new' instanciation, HardwareTimer is not destructed when setup() function is finished.
  HardwareTimer *MyTim = new HardwareTimer( Instance );
  MyTim->setMode( 2, TIMER_OUTPUT_COMPARE );
  MyTim->setOverflow( 10, HERTZ_FORMAT );  //10 Hz
  MyTim->attachInterrupt( _100msCallback );
  MyTim->resume();

  //
  Serial1.setRx( PA10 );
  Serial1.setTx( PA9 );
  Serial1.begin( 115200 );

  //Serial2.setRx( PA3 );
  //Serial2.setTx( PA2 );
  Serial2.setRx( PA_3_ALT1 );
  Serial2.setTx( PA_2_ALT1 );
  Serial2.begin( 115200 );

  Serial4.setRx( PA1 );
  Serial4.setTx( PA0 );
  Serial4.begin( 115200 );

  Serial5.setRx( PB4 );
  Serial5.setTx( PB3 );
  Serial5.begin( 115200 );

  Serial1.write( "Serial1\r\n" );
  Serial2.write( "Serial2\r\n" );
  Serial4.write( "Serial4\r\n" );
  Serial5.write( "Serial5\r\n" );
  
  pRaMonBuff = new ByteArray( 300 );
  
  pDemoApp = new LoRaMesh_DemoApp( Serial1, SerialUSB );
  pDemoApp->print();

}

#define CommandTimeout  30000

void SerialUSBHandler( void ) {
  static uint32_t LastInputAtTick = 0;
  
  if ( ActiveCommand ) {
    if ( HAL_GetTick() > ( LastInputAtTick + CommandTimeout ) ) {
      ActiveCommand = 0;    //rollback visam!!!
      SerialUSB.println(" <- timeout");
    } else
      return;
  }
  if ( SerialUSB.available() ) {
    uint8_t inChar = (uint8_t)SerialUSB.read();
    SerialUSB.print( char( inChar ) );
    if ( '<' == inChar ) inChar = 27;
    ActiveCommand = inChar;
    LastInputAtTick = mySysTick;
  }
}


void CommandHandler( void ) {
  if ( ActiveCommand ) {
    int16_t item = findItem(
      CommandTables[ActiveCommandTable].pCommandTable,
      CommandTables[ActiveCommandTable].CommandCount,
      ActiveCommand
    );
    if ( -1 == item ) {
      SerialUSB.println( F(" <- command?") );
    } else {
      if ( CommandTables[ActiveCommandTable].pCommandTable[item].aFunction ) {
        SerialUSB.print( F(": ") );
        SerialUSB.println( CommandTables[ActiveCommandTable].pCommandTable[item].cDescription );
        CommandTables[ActiveCommandTable].pCommandTable[item].aFunction();
      } else {
        SerialUSB.println( F(" <- a separator") );
      }
    }
    ActiveCommand = 0;
  }
}

void RadioHandler( void ) {
  if ( pDemoApp->GetSerial().available() )
    pDemoApp->OnSerialPort_ReadyRead();
}

#define MonitorDelayTicks 1

void MonitorHandler( void ) {
    if ( pRaMonBuff->count() ) {
        if (
            ( mySysTick >= ( lastS2IOtick + MonitorDelayTicks ) ) ||
            ( pRaMonBuff->count() > ( pRaMonBuff->size() - 10 ) ) ) {
            pRaMonBuff->print( SerialUSB );
        }
    }
}

//SerialUSB: commands from PC
//Serial1  : iM284A interface
//Serial2  : outoing communication with iM284A monitor
void loop( void ) {
  mySysTick = HAL_GetTick();
  //LEDhandler(); //HW interrupt
  SerialUSBHandler();
  CommandHandler();
  RadioHandler();
  MonitorHandler();
}
