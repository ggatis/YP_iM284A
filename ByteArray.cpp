/**
 * @file    ByteArray.cpp
 *
 * @brief   Implementation of class ByteArray
 *
 * @note    This example code is free software: you can redistribute it and/or modify it.
 *
 *          This program is provided by EDI on an "AS IS" basis without
 *          any warranties in the hope that it will be useful.
 * 
 * Gatis Gaigals @ EDI, 2024
 */

#include <cstddef>
#include <cstring>
#include "ByteArray.h"

/**
  * @brief  class constructor for empty instance
  *
  * @param  -
 */
ByteArray::ByteArray( void ) :
    _size( 0 ), _count( 0 ), _data( (uint8_t*)nullptr ) {
}

/**
  * @brief  class constructor, not initialised buffer
  *
  * @param  buffersize  buffer size
 */
ByteArray::ByteArray( uint16_t buffersize ) :
    _size( buffersize ), _count( 0 ), _data( new uint8_t[(size_t)buffersize] ) {
}

/**
  * @brief  class constructor, size, pointe to buffer
  *
  * @param  size  buffer size, it all contains data
  *         dataptr  pointer to buffer
 */
ByteArray::ByteArray( uint16_t size, uint8_t* dataptr ) :
    _size( size ), _count( size ), _data( dataptr ) {
}

/**
  * @brief  class constructor, all parameters
  *
  * @param  size  buffer size, it all contains data
  *         filled  useful data bytes count
  *         dataptr  pointer to buffer
 */
ByteArray::ByteArray( uint16_t size,  uint16_t filled, uint8_t* dataptr ) :
    _size( size ), _count( filled ), _data( dataptr ) {
}

/**
  * @brief  class constructor, initialised repeating a char
  *
  * @param  repeats   character repeat count
  *         c         character
 */
ByteArray::ByteArray( uint16_t repeats, char c ) :
    _size( repeats ), _count( repeats ), _data( new uint8_t[(size_t)repeats] ) {
    std::memset( const_cast<uint8_t*>( _data ), c, (size_t)repeats);
}

/**
  * @brief  class destructor
  *
  * @param  -
 */
ByteArray::~ByteArray( void ) {
    delete[] _data;
}

/**
 * @brief   returns data buffer
 *
 * @param   -
 *
 * @return  ByteArray data buffer
 */
uint8_t*    ByteArray::data( void ) const {
    return _data;
}

/**
 * @brief   returns size of data in array
 *
 * @param   -
 *
 * @return  ByteArray data size
 */
uint16_t    ByteArray::count( void ) const {
    return _count;
}

/**
 * @brief   returns size of array
 *
 * @param   -
 *
 * @return  ByteArray array size
 */
uint16_t    ByteArray::size( void ) const {
    return _size;
}

/**
 * @brief   clears the contents of the byte array and makes it null
 *
 * @param   -
 *
 * @return  -
 */
void        ByteArray::clear( void ) {
    _count = 0;
}

/**
 * @brief   updates count of the data size in arrray in the case it is manipulated from outside
 *
 * @param   -
 *
 * @return  correctly updated count of the data size in arrray
 */
uint16_t    ByteArray::update_count( uint16_t newcount ) {
    _count = ( newcount > _size ) ? _size : newcount;
    return _count;
}

/**
 * @brief   return byte at index
 *
 * @param   index  array index
 *
 * @return  byte at position index
 */
uint8_t     ByteArray::at( uint16_t index ) const {
    //if ( index < _count )
    if ( index < _size )
        return _data[index];
    return 0;
}

/**
 * @brief   returns byte count in array
 *
 * @param   byte  byte to append
 *
 * @return  byte count in array or 0 if array is full
 */
uint16_t    ByteArray::append( uint8_t abyte ) {
    if ( _count < _size ) {
        _data[_count++] = abyte;
        return _count;
    }
    return 0;
}

/**
 * @brief   return ByteArray converted form HEX
 *
 * @param   hexEncoded  HEX encoded array
 *
 * @return  ByteArray converted form HEX
 */
ByteArray
ByteArray::fromHex( const ByteArray &hexEncoded ) const {
    ByteArray tfromHex( ( hexEncoded._count >> 1 ) + ( hexEncoded._count & 1 ) );

    for ( uint16_t i = 0; i < tfromHex._count; i++ ) {
        uint16_t j = 0;
        uint8_t abyte = 0;

        uint8_t hexb = hexEncoded.at( j++ );
        if ( ( '0' <= hexb ) && ( hexb <= '9' ) ) {
            abyte += hexb - '0';
        } else if ( ( 'a' <= hexb ) && ( hexb <= 'f' ) ) {
            abyte += hexb - 'a' + 10;
        } else if ( ( 'A' <= hexb ) && ( hexb <= 'F' ) ) {
            abyte += hexb - 'a' + 10;
        }
        abyte = abyte << 4;

        hexb = hexEncoded.at( j++ );
        if ( ( '0' <= hexb ) && ( hexb <= '9' ) ) {
            abyte += hexb - '0';
        } else if ( ( 'a' <= hexb ) && ( hexb <= 'f' ) ) {
            abyte += hexb - 'a' + 10;
        } else if ( ( 'A' <= hexb ) && ( hexb <= 'F' ) ) {
            abyte += hexb - 'a' + 10;
        }

        tfromHex.append( abyte );
    }
    return tfromHex;
}
