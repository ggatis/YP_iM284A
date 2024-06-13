/**
 * @file    CurrentTime.h
 *
 * @brief   
 *
 * @note    
 *
 *          This program is provided by EDI on an "AS IS" basis without
 *          any warranties in the hope that it will be useful.
 * 
 * Gatis Gaigals @ EDI, 2024
 */

#ifndef _CURRENTTIME_H_
#define _CURRENTTIME_H_

#include <cstdint>


#define _SYSTIME_ 0  //1: sys/time.h, 0: internal


#if 1 == _SYSTIME_

//get the current time in seconds since the epoch
uint32_t    getCurrentTimeInSeconds( void );

#else

void        initCurrentTime( void );
uint32_t    getCurrentTimeInSeconds( void );
void        setCurrentTimeInSeconds( uint32_t CurrentTimeInSeconds );
void        addCurrentTimeInUSeconds( uint32_t USeconds );

#endif

#endif // _CURRENTTIME_H_
