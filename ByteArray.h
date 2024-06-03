/**
 * @file    ByteArray.h
 *
 * @brief   Declaration of class ByteArray
 *
 * @note    This example code is free software: you can redistribute it and/or modify it.
 *
 *          This program is provided by EDI on an "AS IS" basis without
 *          any warranties in the hope that it will be useful.
 * 
 * Gatis Gaigals @ EDI, 2024
 */

#ifndef _ByteArray_H_
#define _ByteArray_H_

#include <stdint.h>

/**
 * @brief   The ByteArray class provides methods for ByteArray.
 */

class ByteArray {
    public:

        /**
          * @brief  class constructor for empty instance
          *
          * @param  -
          */
                    ByteArray( void );

        /**
          * @brief  class constructor, not initialised buffer
          *
          * @param  buffersize  buffer size
          */
                    ByteArray( uint16_t buffersize );

        /**
          * @brief  class constructor, size, pointe to buffer
          *
          * @param  size  buffer size, it all contains data
          *         dataptr  pointer to buffer
          */
                    ByteArray( uint16_t size, uint8_t* dataptr );

        /**
          * @brief  class constructor, all parameters
          *
          * @param  size  buffer size, it all contains data
          *         filled  useful data bytes count
          *         dataptr  pointer to buffer
          */
                    ByteArray( uint16_t size, uint16_t filled, uint8_t* dataptr );

        /**
          * @brief  class constructor, initialised repeating a char
          *
          * @param  repeats  character repeat count
          *         c        character
          */
                    ByteArray( uint16_t repeats, char c );

        /**
          * @brief  class destructor
          *
          * @param  -
          */
                   ~ByteArray( void );

        /**
         * @brief   returns data buffer
         *
         * @param   -
         *
         * @return  ByteArray data buffer
         */
        uint8_t*    data( void ) const;

        /**
         * @brief   returns size of data in array
         *
         * @param   -
         *
         * @return  ByteArray data size
         */
        uint16_t    count( void ) const;

        /**
         * @brief   returns size of array
         *
         * @param   -
         *
         * @return  ByteArray array size
         */
        uint16_t    size( void ) const;

        /**
         * @brief   clears the contents of the byte array and makes it null
         *
         * @param   -
         *
         * @return  -
         */
        void        clear( void );

        /**
         * @brief   updates count of the data size in arrray
         *          in the case it is manipulated from outside
         *
         * @param   -
         *
         * @return  correctly updated count of the data size in arrray
         */
        uint16_t    update_count( uint16_t newcount );

        /**
         * @brief   return byte at index
         *
         * @param   index  array index
         *
         * @return  byte at position index
         */
        uint8_t     at( uint16_t index ) const;

        /**
         * @brief   returns byte count in array
         *
         * @param   byte  byte to append
         *
         * @return  byte count in array
         */
        uint16_t     append( uint8_t byte );

        /**
         * @brief   return ByteArray converted form HEX
         *
         * @param   hexEncoded  HEX encoded array
         *
         * @return  ByteArray converted form HEX
         */
        ByteArray   fromHex( const ByteArray &hexEncoded ) const;

    private:
        //<! size
        uint16_t        _size;
        //<! count
        uint16_t        _count;
        //<! data
        uint8_t* const  _data;
};

#endif // _ByteArray_H_
