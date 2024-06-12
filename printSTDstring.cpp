/**
 * @file    printSTDstring.h
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

#include "printSTDstring.h"
#include <Arduino.h>

/**
 * @brief   
 *
 * @param   
 *
 */
void printSTDstring( const std::string& aString ) {
    for ( uint16_t i = 0; i < aString.size(); ++i ) {
        SerialUSB.print( aString[i] );
    }
}
