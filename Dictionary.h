/**
 * @file    Dictionary.h
 *
 * @brief   Declaration of class Dictionary
 *
 * @note    This example code is free software: you can redistribute it and/or modify it.
 *
 *          This program is provided by EDI on an "AS IS" basis without
 *          any warranties in the hope that it will be useful.
 * 
 * Gatis Gaigals @ EDI, 2024
 */

#ifndef _Dictionary_H_
#define _Dictionary_H_

#include <stdint.h>
#include "ByteArray.h"

/**
 * @brief   The Dictionary class provides methods for Dictionary based on ByteArray.
 *          Dictionary can not contain 0.
 */

class Dictionary {
    public:

        /**
          * @brief  class constructor, not initialised buffer
          *
          * @param  buffersize  buffer size
          */
                    Dictionary( uint16_t buffersize = 256 );

        /**
         * @brief   returns size of data in Dictionary
         *
         * @param   -
         *
         * @return  Dictionary data size
         */
        uint16_t    count( void ) const;

        /**
         * @brief   returns size of Dictionary
         *
         * @param   -
         *
         * @return  Dictionary size
         */
        uint16_t    size( void ) const;

        /**
         * @brief   returns key count of Dictionary
         *
         * @param   -
         *
         * @return  Dictionary key count
         */
        uint16_t    keys( void ) const;

        /**
         * @brief   returns pointer to key[n]
         *
         * @param   -
         *
         * @return  pointer to dictionary key[n]
         */
        uint8_t*    key( uint16_t n ) const;

        /**
         * @brief   returns pointer to data[n]
         *
         * @param   -
         *
         * @return  pointer to dictionary data[n]
         */
        uint8_t*    data( uint16_t n ) const;

        /**
         * @brief   returns key[n] size not including end separator
         *
         * @param   -
         *
         * @return  key[n] size not including end separator
         */
        uint16_t    sizeof_key( uint16_t n ) const;

        /**
         * @brief   returns data[n] size not including end separator
         *
         * @param   -
         *
         * @return  data[n] size not including end separator
         */
        uint16_t    sizeof_data( uint16_t n ) const;

        /**
         * @brief   returns byte count in array
         *
         * @param   key and data to append
         *
         * @return  byte count in array
         */
        uint16_t    append( const char* data, bool Continue = true );

        /**
         * @brief   returns byte count in array
         *
         * @param   key and data to append
         *
         * @return  byte count in array
         */
        uint16_t    appendU8( uint8_t n, bool Continue = true );

        /**
         * @brief   returns byte count in array
         *
         * @param   key and data to append
         *
         * @return  byte count in array
         */
        uint16_t    appendU32( uint32_t n, bool Continue = true );

        /**
         * @brief   returns byte count in array
         *
         * @param   key and data to append
         *
         * @return  byte count in array
         */
        uint16_t    append( const char* akey, char* data );

        /**
         * @brief   returns byte count in array
         *
         * @param   key and data to append
         *
         * @return  byte count in array
         */
        uint16_t    append( const char* akey, const char* data );

        /**
         * @brief   returns byte count in array
         *
         * @param   key and data to append
         *
         * @return  byte count in array
         */
        uint16_t    append( const char* akey, const uint8_t* data );

        /**
         * @brief   returns byte count in array
         *
         * @param   key and data to append
         *
         * @return  byte count in array
         */
        uint16_t    append( const char* akey, const uint8_t* data, int size );

        /**
         * @brief   returns byte count in array
         *
         * @param   key and data to append
         *
         * @return  byte count in array
         */
        uint16_t    append( const char* akey, uint8_t n );

        /**
         * @brief   returns byte count in array
         *
         * @param   key and data to append
         *
         * @return  byte count in array
         */
        uint16_t    append( const char* akey, uint32_t n );

        /**
         * @brief   returns byte count in array
         *
         * @param   key and data to append
         *
         * @return  byte count in array
         */
        uint16_t    append( const char* akey, std::string& aString );

        /**
         * @brief   returns byte count in array
         *
         * @param   key and data to append
         *
         * @return  byte count in array
         */
        uint16_t    append( const char* akey, std::string&& aString );

        /**
         * @brief   returns byte count in array
         *
         * @param   key and data to append
         *
         * @return  byte count in array
         */
//        uint16_t    append( const uint8_t* akey, std::string& aString );

        /**
         * @brief   
         *
         * @param   
         *
         * @return  
         */
        uint16_t    append( const char* akey, const Dictionary aDictionary, char delimiter = '.');

        /**
         * @brief   finds if the dictionary contains a record with a given key
         *
         * @param   -
         *
         * @return  the pos of the data
         */
        const uint8_t*  contains( const char* akey ) const;

        /**
         * @brief   finds if the dictionary contains a record with a given key
         *
         * @param   -
         *
         * @return  the pos of the data
         */
        const uint8_t*  contains( const uint8_t* akey ) const;

        /**
         * @brief   deletes from the dictionary the record with a given key
         *
         * @param   -
         *
         * @return  the size of the deleted record
         */
        uint16_t    remove( const uint8_t* akey );

        /**
         * @brief   clears the contents of the Dictionary
         *
         * @param   -
         *
         * @return  -
         */
        void        clear( void );

        /**
         * @brief   prints the dictionary in an elegant way
         *
         * @param   -
         *
         * @return  prints the dictionary
         */
        void        print( void ) const;
//        void        print( HardwareSerial Serial ) const;

        /**
         * @brief   prints the dictionary in an elegant way
         *
         * @param   -
         *
         * @return  prints the dictionary
         */
//        void        print( USBSerial Serial ) const;

        /**
         * @brief   prints the keys containing records of the dictionary in an elegant way
         *
         * @param   -
         *
         * @return  prints the dictionary
         */
//        void        print( HardwareSerial Serial, const char* keys[],
//                        const uint16_t keycount, bool inverse = false ) const;
        void        print( const char* keys[],
                        const uint16_t keycount, bool inverse = false ) const;

        /**
         * @brief   prints the keys containing records of the dictionary in an elegant way
         *
         * @param   -
         *
         * @return  prints the dictionary
         */
//        void        print( USBSerial Serial, const char* keys[],
//                        const uint16_t keycount, bool inverse = false ) const;

    private:
        //<! ByteArray
        ByteArray       _ByteArray;
        //<! keys
        uint16_t        _keys;
};

#endif // _Dictionary_H_
