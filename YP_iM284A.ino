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

//#include "HardwareSerial.h"
//HardwareSerial Serial1( USART1 );  //Radio
//HardwareSerial Serial2( USART2 );  //monitor
//HardwareSerial Serial4( USART4 );
//HardwareSerial Serial5( USART5 );

#define REDEFINE_WRITE 1
#if 0 == REDEFINE_WRITE
/* Private function prototypes -----------------------------------------------*/
extern "C" PUTCHAR_PROTOTYPE {
  HAL_UART_Transmit( SerialUSB.getHandle(), (uint8_t*)&ch, 1, HAL_MAX_DELAY );
  return ch;
}
#else   //0 == REDEFINE_WRITE
//Override the _write function to use SerialUSB
extern "C" int _write( int file, char* ptr, int len ) {
    //Write the character array to SerialUSB
    return (int)SerialUSB.write( (uint8_t*)ptr, len );
}
#endif  //0 == REDEFINE_WRITE

#include "ByteArray.h"
ByteArray* pRaMonBuff = nullptr;  //serial activity

#include "CRC16.h"
#include "SlipDecoder.h"
#include "SlipEncoder.h"

#include "iM284A.h"
#include "iM284A_L0.h"  //for "print usage"

#include "CurrentTime.h"

#include "LoRa_Mesh_DemoApp.h"
LoRaMesh_DemoApp* pDemoApp = nullptr;

/*********************************************************************/
/*                               Pins                                */
/*********************************************************************/
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


/*********************************************************************/
/*                               Timer                               */
/*********************************************************************/
HardwareTimer* pMyTim;

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


/*********************************************************************/
/*                              Serials                              */
/*********************************************************************/
void serialEvent1() {
    LEDtoggle();
    //SerialUSB.println( F("sEv1") );
    printf("sEv1\r\n");
}


void serialEvent2() {
    LEDtoggle();
    //SerialUSB.println( F("sEv2") );
    printf("sEv2\r\n");
    if ( pRaMonBuff ) {
        lastS2IOtick = mySysTick;
        while ( 0 < Serial2.available() ) {
            LEDtoggle();
            pRaMonBuff->append( (uint8_t)Serial2.read() );
        }
    }
}

void serialEvent4() {
    LEDtoggle();
    //SerialUSB.println( F("sEv4") );
    printf("sEv4\r\n");
}

void serialEvent5() {
    LEDtoggle();
    //SerialUSB.println( F("sEv5") );
    printf("sEv5\r\n");
}

/*********************************************************************/
/*                               Setup                               */
/*********************************************************************/
void setupPins( void ) {
  LEDinit();
}

void setupSerials( void ) {
  //Serial.end();
  SerialUSB.begin( 115200 );
  //Initialize serial and wait for port to open:
  while ( !SerialUSB ) {
    ;   //wait for serial port to connect. Needed for native USB port only
  }
  LEDoff();

  //
  //Serial1.end();
  //Serial1.setRx( PA10 );
  //Serial1.setTx( PA9 );
  Serial1.begin( 115200 );
  //Serial1.print( "S1" );
  printf("S1");

  Serial2.end();
  //Serial2.setRx( PA3 );
  //Serial2.setTx( PA2 );
  Serial2.setRx( PA_3_ALT1 );
  Serial2.setTx( PA_2_ALT1 );
  Serial2.begin( 115200 );

  Serial4.end();
  Serial4.setRx( PA1 );
  Serial4.setTx( PA0 );
  Serial4.begin( 115200 );

  Serial5.end();
  Serial5.setRx( PB4 );
  Serial5.setTx( PB3 );
  Serial5.begin( 115200 );

}

void setupTimers( void ) {

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
  pMyTim = new HardwareTimer( Instance );
  pMyTim->setMode( 2, TIMER_OUTPUT_COMPARE );
  pMyTim->setOverflow( 10, HERTZ_FORMAT );  //10 Hz
  pMyTim->attachInterrupt( _100msCallback );
  pMyTim->resume();
}

void setup( void ) {

  pRaMonBuff = new ByteArray( 300 );
  
  setupPins();
  setupSerials();
  setupTimers();
  
  //SerialUSB.println( F("") );
  printf("\r\n");
  //SerialUSB.println( F("") );
  printf("\r\n");
  //SerialUSB.println( F("") );
  printf("\r\n");
  //SerialUSB.println( F("YP_iM284A: test of iM284A HCI v0.2:") );
  printf("YP_iM284A: test of iM284A HCI v0.2:\r\n");
  
  printUsage();
  Serial1.write("Serial1\r\n");
  Serial2.write("Serial2\r\n");
  Serial4.write("Serial4\r\n");
  Serial5.write("Serial5\r\n");
  
  pDemoApp = new LoRaMesh_DemoApp( Serial1 );
  pDemoApp->print();

}


/*********************************************************************/
/*                               Main                                */
/*********************************************************************/
#define CommandTimeout  30000

void SerialUSBHandler( void ) {
  static uint32_t LastInputAtTick = 0;
  
  if ( ActiveCommand ) {
    if ( HAL_GetTick() > ( LastInputAtTick + CommandTimeout ) ) {
      ActiveCommand = 0;    //rollback visam!!!
      printf(" <- timeout");
    } else
      return;
  }
  if ( SerialUSB.available() ) {
    uint8_t inChar = (uint8_t)SerialUSB.read();
    printf("%c", inChar );
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
      printf(" <- command?\r\n");
    } else {
      if ( CommandTables[ActiveCommandTable].pCommandTable[item].aFunction ) {
        //SerialUSB.print( F(": ") );
        printf(": ");
        //SerialUSB.println( CommandTables[ActiveCommandTable].pCommandTable[item].cDescription );
        printf( CommandTables[ActiveCommandTable].pCommandTable[item].cDescription );
        printf("\r\n");
        CommandTables[ActiveCommandTable].pCommandTable[item].aFunction();
      } else {
        //SerialUSB.println( F(" <- a separator") );
        printf(" <- a separator\r\n");
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
    if ( ( pRaMonBuff ) && ( pRaMonBuff->count() ) ) {
        if ( ( mySysTick >= ( lastS2IOtick + MonitorDelayTicks ) ) ||
             ( pRaMonBuff->count() > ( pRaMonBuff->size() - 10 ) ) ) {
            //SerialUSB.print( F("In2:") );
            printf("In2:");
            pRaMonBuff->print();
            pRaMonBuff->clear();
        }
    }
}

void GetSleep( void ) {
  //no sleep so far
  //deepsleep lib
  //check state
  //return if action
  //sleep
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
  GetSleep();
  if ( 0 == ( mySysTick % 1000 ) ) {
    printf("%d\r\n", pRaMonBuff->count() );
    Serial1.print('1');
    Serial2.print('2');
    Serial4.print('4');
    Serial5.print('5');
    delay( 1 );
  }
}
