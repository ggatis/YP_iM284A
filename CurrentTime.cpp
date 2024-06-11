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
