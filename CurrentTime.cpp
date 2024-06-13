/**
 * @file    CurrentTime.cpp
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

#include "CurrentTime.h"

#if 1 == _SYSTIME_

#include <sys/time.h>
#include <chrono>

uint32_t getCurrentTimeInSeconds() {
    //get the current time point
    auto now = std::chrono::system_clock::now();
    //convert the time point to a duration since the epoch
    auto duration = now.time_since_epoch();
    //convert the duration to seconds
    auto seconds = std::chrono::duration_cast<std::chrono::seconds>( duration ).count();
    //return the current time in seconds since epoch
    return static_cast<uint32_t>(seconds);
}

#else

#include <Arduino.h>

static struct timeval CurrentTime;

//get the current time in seconds since the epoch
void        initCurrentTime( void ) {
    noInterrupts();

    CurrentTime.tv_sec  = 0;
    CurrentTime.tv_usec = 0;

    interrupts();
}


uint32_t    getCurrentTimeInSeconds( void ) {
    return (uint32_t)CurrentTime.tv_sec;
}


void        setCurrentTimeInSeconds( uint32_t CurrentTimeInSeconds ) {
    noInterrupts();

    CurrentTime.tv_sec  = CurrentTimeInSeconds;
    CurrentTime.tv_usec = 0;

    interrupts();
}


void        addCurrentTimeInUSeconds( uint32_t USeconds ) {
    noInterrupts();

    CurrentTime.tv_usec += USeconds;
    while ( CurrentTime.tv_usec > 1000000 ) {
        CurrentTime.tv_sec++;
        CurrentTime.tv_usec -= 1000000;
    }

    interrupts();
}

#endif
