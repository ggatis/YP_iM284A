/**
 * @file    Dictionary.cpp
 *
 * @brief   Implementation of class Dictionary based on ByteArray
 *
 * @note    This example code is free software: you can redistribute it and/or modify it.
 *
 *          This program is provided by EDI on an "AS IS" basis without
 *          any warranties in the hope that it will be useful.
 * 
 * Gatis Gaigals @ EDI, 2024
 */

//#include <cstddef>          //size_t - kick?
//#include <string>           //string
//#include <cstring>          //nullptr
#include "Dictionary.h"


uint8_t* skipNzeros( uint8_t* ptr, uint16_t n, uint16_t dicsize ) {
//    uint16_t dicsize = _ByteArray.count();
//    while( dicsize && n ) {
//        while( dicsize && *ptr++ ) {
//            dicsize--;
//        }
//        n--;
//    }
//    return ptr;
    uint8_t* i = ptr;
    for ( ; n && ( i < ( ptr + dicsize ) ); i++ ) {
        if ( 0 == *i ) n--;
    }
    return ++i;
}

uint16_t scan0FromHere( uint8_t* ptr, uint16_t maxsize ) {
    uint8_t* i = ptr;
    for ( ; i < ( ptr + maxsize ); i++ ) {
        if ( 0 == *i ) break;
    }
    return (uint16_t)( i - ptr );
}


/**
  * @brief  class constructor for empty instance
  *
  * @param  -
 */
Dictionary::Dictionary( void ) :
    _ByteArray( 256 ), _keys( 0 ) {
}

/**
  * @brief  class constructor, not initialised buffer
  *
  * @param  buffersize  buffer size
 */
Dictionary::Dictionary( uint16_t buffersize ) :
    _ByteArray( buffersize ), _keys( 0 ) {
}

/**
 * @brief   returns size of data in Dictionary
 *
 * @param   -
 *
 * @return  Dictionary data size
 */
uint16_t    Dictionary::count( void ) const {
    return _ByteArray.count();
}

/**
 * @brief   returns size of Dictionary
 *
 * @param   -
 *
 * @return  Dictionary size
 */
uint16_t    Dictionary::size( void ) const {
    return _ByteArray.size();
}

/**
 * @brief   returns key count of Dictionary
 *
 * @param   -
 *
 * @return  Dictionary key count
 */
uint16_t    Dictionary::keys( void ) const {
    return _keys;
}

/**
 * @brief   returns pointer to key[n]
 *
 * @param   -
 *
 * @return  pointer to dictionary key[n]
 */
uint8_t*    Dictionary::key( uint16_t n ) const {
    return skipNzeros( _ByteArray.data(), n << 1, _ByteArray.count() );
}

/**
 * @brief   returns pointer to data[n]
 *
 * @param   -
 *
 * @return  pointer to dictionary data[n]
 */
uint8_t*    Dictionary::data( uint16_t n ) const {
    return skipNzeros( _ByteArray.data(), 1 + n << 1, _ByteArray.count() );
}

/**
 * @brief   returns key[n] size not including end separator
 *
 * @param   -
 *
 * @return  key[n] size not including end separator
 */
uint16_t    Dictionary::sizeof_key( uint16_t n ) const {
    uint16_t    dicsize = _ByteArray.count();
    uint8_t*    pbegin  = _ByteArray.data();
    uint8_t*    pkeyn   = key( n );
    return scan0FromHere( pkeyn, dicsize - (uint16_t)( pkeyn - pbegin ) );
}

/**
 * @brief   returns data[n] size not including end separator
 *
 * @param   -
 *
 * @return  data[n] size not including end separator
 */
uint16_t    Dictionary::sizeof_data( uint16_t n ) const {
    uint16_t    dicsize = _ByteArray.count();
    uint8_t*    pbegin  = _ByteArray.data();
    uint8_t*    pkeyn   = data( n );
    return scan0FromHere( pkeyn, dicsize - (uint16_t)( pkeyn - pbegin ) );
}

/**
  * @brief   returns byte count in array
  *
  * @param   byte  byte to append
  *
  * @return  byte count in array
  */
uint16_t     Dictionary::append( const char* akey, const uint8_t* data ) {
    _keys++;
    uint8_t*    plimit  = _ByteArray.data() + _ByteArray.size();
    uint8_t*    pactual = _ByteArray.data() + _ByteArray.count();
    for ( ; pactual < plimit; ) {
        if ( 0 == ( *pactual++ = (uint8_t)*akey++ ) ) break;
    }
    for ( ; pactual < plimit; ) {
        if ( 0 == ( *pactual++ = *data++ ) ) break;
    }
    *--plimit = 0;
    _ByteArray.update_count( (uint16_t)( pactual - _ByteArray.data() ) );
    return (uint16_t)( pactual - _ByteArray.data() );
}

/**
  * @brief   returns byte count in array
  *
  * @param   byte  byte to append
  *
  * @return  byte count in array
  */
uint16_t     Dictionary::append( const char* akey, char* data ) {
    _keys++;
    uint8_t*    plimit  = _ByteArray.data() + _ByteArray.size();
    uint8_t*    pactual = _ByteArray.data() + _ByteArray.count();
    for ( ; pactual < plimit; ) {
        if ( 0 == ( *pactual++ = (uint8_t)*akey++ ) ) break;
    }
    for ( ; pactual < plimit; ) {
        if ( 0 == ( *pactual++ = (uint8_t)*data++ ) ) break;
    }
    *--plimit = 0;
    _ByteArray.update_count( (uint16_t)( pactual - _ByteArray.data() ) );
    return (uint16_t)( pactual - _ByteArray.data() );
}

/**
  * @brief   returns byte count in array
  *
  * @param   byte  byte to append
  *
  * @return  byte count in array
  */
uint16_t     Dictionary::append( const char* akey, const char* data ) {
    _keys++;
    uint8_t*    plimit  = _ByteArray.data() + _ByteArray.size();
    uint8_t*    pactual = _ByteArray.data() + _ByteArray.count();
    for ( ; pactual < plimit; ) {
        if ( 0 == ( *pactual++ = (uint8_t)*akey++ ) ) break;
    }
    for ( ; pactual < plimit; ) {
        if ( 0 == ( *pactual++ = (uint8_t)*data++ ) ) break;
    }
    *--plimit = 0;
    _ByteArray.update_count( (uint16_t)( pactual - _ByteArray.data() ) );
    return (uint16_t)( pactual - _ByteArray.data() );
}

/**
  * @brief   returns byte count in array
  *
  * @param   byte  byte to append
  *
  * @return  byte count in array
  */
uint16_t     Dictionary::append( const uint8_t* akey, const uint8_t* data ) {
    _keys++;
    uint8_t*    plimit  = _ByteArray.data() + _ByteArray.size();
    uint8_t*    pactual = _ByteArray.data() + _ByteArray.count();
    for ( ; pactual < plimit; ) {
        if ( 0 == ( *pactual++ = *akey++ ) ) break;
    }
    for ( ; pactual < plimit; ) {
        if ( 0 == ( *pactual++ = *data++ ) ) break;
    }
    *--plimit = 0;
    _ByteArray.update_count( (uint16_t)( pactual - _ByteArray.data() ) );
    return (uint16_t)( pactual - _ByteArray.data() );
}

/**
 * @brief   finds if the dictionary contains a record with a given key
 *
 * @param   -
 *
 * @return  the pos of the data
 */
uint8_t*    Dictionary::contains( const uint8_t* akey ) const {
    uint16_t k;
    uint8_t* testkey;
    uint8_t* keycopy;

    uint16_t sizeofkey = strlen( (const char*)akey );
    if ( 0 == sizeofkey++ ) return nullptr; //sizeofkey++ include zero in check

    for ( k = 0; k < _keys; k++ ) {
        testkey = key( k );
        for ( keycopy = (uint8_t*)akey; keycopy < (uint8_t*)( akey + sizeofkey ); ) {
            if ( *testkey++ != *keycopy++ ) goto try_next;
        }
        if ( (uint16_t)( testkey - _ByteArray.data() ) > _ByteArray.size() ) {
            return _ByteArray.data() + _ByteArray.size() - 1;
        } else {
            return testkey;
        }
try_next:
        ;   //at least an empty statement for the God of labels
    }
    return nullptr;
}

/**
 * @brief   deletes from the dictionary the record with a given key
 *
 * @param   -
 *
 * @return  the size of the deleted record
 */
uint16_t    Dictionary::remove( const uint8_t* akey ) {
    uint16_t deleted = 0;

    const uint16_t sizeofkey = strlen( (const char*)akey );
    if ( 0 == sizeofkey ) return deleted;

    uint8_t* ptrdata = contains( akey );
    if ( ptrdata ) {
        //Houston we have data!
        //outer frame
        uint8_t* MrFirst = _ByteArray.data();
        uint8_t* MrLast  = _ByteArray.data() + _ByteArray.count();    //actually next to last
        //inner frame
        uint8_t* first = ptrdata - sizeofkey - 1;   //minus zero sugar
        const uint16_t sizeofdata = strlen( (const char*)ptrdata );
        uint8_t* last  = ptrdata + sizeofdata + 1;  //next to zero sugar
        if ( last > MrLast ) {
            last = MrLast;
        }
        deleted = last - first;
        if ( deleted < _ByteArray.count() ) {
            for ( uint8_t* s = last; s < MrLast; )
                *first++ = *s++;
            *--first = 0;
        } else {
            deleted = _ByteArray.count();
            _ByteArray.clear();
        }
    }
    return deleted;
}

/**
 * @brief   clears the contents of the Dictionary
 *
 * @param   -
 *
 * @return  -
 */
void        Dictionary::clear( void ) {
    _ByteArray.clear();
    _keys = 0;
}

/**
 * @brief   prints the dictionary in an elegant way
 *
 * @param   -
 *
 * @return  prints the dictionary
 */
void        Dictionary::print( HardwareSerial Serial ) const {
    //key : data
    uint16_t maxsizeofkey = 0;
    uint16_t sizeofkeyi;
    uint16_t i;
    for ( i = 0; i < _keys; i++ ) {
        sizeofkeyi = sizeof_key( i );
        if ( maxsizeofkey < sizeofkeyi ) maxsizeofkey = sizeofkeyi;
    }
    for ( i = 0; i < _keys; i++ ) {
        sizeofkeyi = sizeof_key( i );
        Serial.print( (const char*)key( i ) );
        sizeofkeyi = maxsizeofkey - sizeofkeyi;
        sizeofkeyi++;
        while ( sizeofkeyi-- ) {
            Serial.print(' ');
        }
        Serial.print( F(": ") );
        Serial.println( (const char*)data( i ) );
    }
}

/**
 * @brief   prints the dictionary in an elegant way
 *
 * @param   -
 *
 * @return  prints the dictionary
 */
void        Dictionary::print( USBSerial Serial ) const {
    //key : data
    uint16_t maxsizeofkey = 0;
    uint16_t sizeofkeyi;
    uint16_t i;
    for ( i = 0; i < _keys; i++ ) {
        sizeofkeyi = sizeof_key( i );
        if ( maxsizeofkey < sizeofkeyi ) maxsizeofkey = sizeofkeyi;
    }
    for ( i = 0; i < _keys; i++ ) {
        sizeofkeyi = sizeof_key( i );
        Serial.print( (const char*)key( i ) );
        sizeofkeyi = maxsizeofkey - sizeofkeyi;
        sizeofkeyi++;
        while ( sizeofkeyi-- ) {
            Serial.print(' ');
        }
        Serial.print( F(": ") );
        Serial.println( (const char*)data( i ) );
    }
}

/**
 * @brief   prints the keys containing records of the dictionary in an elegant way
 *
 * @param   -
 *
 * @return  prints the dictionary
 */
void        Dictionary::print( HardwareSerial Serial, const char* keys[],
                const uint16_t keycount, bool inverse ) const {

    uint16_t maxsizeofkey = 0;
    uint16_t sizeofkey;
    uint16_t k, i;
    const char*     akey;
    const uint8_t*  testkey;
    const uint8_t*  testdata;

    if ( inverse ) {
        //count only these that do not match!
        
        for ( i = 0; i < _keys; i++ ) {

            testkey = key( i );
            for ( k = 0; k < keycount; k++ ) {
                akey = keys[k];
                if ( strcmp( (const char*)( testkey ), akey ) == 0 )
                    goto SkipThis;
            }

            sizeofkey = strlen( (const char*)testkey );
            if ( maxsizeofkey < sizeofkey ) maxsizeofkey = sizeofkey;

SkipThis:
            ;
        }
        for ( i = 0; i < _keys; i++ ) {

            testkey = key( i );
            for ( k = 0; k < keycount; k++ ) {
                akey = keys[k];
                if ( strcmp( (const char*)( testkey ), akey ) == 0 )
                    goto SkipThisAgain;
            }

            Serial.print( (const char*)testkey );

            sizeofkey = strlen( (const char*)testkey );
            for ( i = sizeofkey; i < maxsizeofkey; i++ )
                Serial.print(' ');
            Serial.print( F(" : ") );

            Serial.println( (const char*)data( i ) );

SkipThisAgain:
            ;
        }

    } else {
        //count only these that match!
        for ( k = 0; k < keycount; k++ ) {

            akey         = keys[k];
            sizeofkey   = strlen( akey );
            if ( maxsizeofkey < sizeofkey ) maxsizeofkey = sizeofkey;

        }
        for ( k = 0; k < keycount; k++ ) {

            akey = keys[k];
            sizeofkey = strlen( akey );
            Serial.print( akey );

            for ( i = sizeofkey; i < maxsizeofkey; i++ )
                Serial.print(' ');
            Serial.print( F(" : ") );

            testdata = contains( (const uint8_t*)( akey ) );
            if ( testdata ) {
                Serial.println( (const char*)( testdata ) );
            } else {
                Serial.println();
            }
        }
    }
}

/**
 * @brief   prints the keys containing records of the dictionary in an elegant way
 *
 * @param   -
 *
 * @return  prints the dictionary
 */
void        Dictionary::print( USBSerial Serial, const char* keys[],
                const uint16_t keycount, bool inverse ) const {

    uint16_t maxsizeofkey = 0;
    uint16_t sizeofkey;
    uint16_t k, i;
    const char*     akey;
    const uint8_t*  testkey;
    const uint8_t*  testdata;

    if ( inverse ) {
        //count only these that do not match!
        
        for ( i = 0; i < _keys; i++ ) {

            testkey = key( i );
            for ( k = 0; k < keycount; k++ ) {
                akey = keys[k];
                if ( strcmp( (const char*)( testkey ), akey ) == 0 )
                    goto SkipThis;
            }

            sizeofkey = strlen( (const char*)testkey );
            if ( maxsizeofkey < sizeofkey ) maxsizeofkey = sizeofkey;

SkipThis:
            ;
        }
        for ( i = 0; i < _keys; i++ ) {

            testkey = key( i );
            for ( k = 0; k < keycount; k++ ) {
                akey = keys[k];
                if ( strcmp( (const char*)( testkey ), akey ) == 0 )
                    goto SkipThisAgain;
            }

            Serial.print( (const char*)testkey );

            sizeofkey = strlen( (const char*)testkey );
            for ( i = sizeofkey; i < maxsizeofkey; i++ )
                Serial.print(' ');
            Serial.print( F(" : ") );

            Serial.println( (const char*)data( i ) );

SkipThisAgain:
            ;
        }

    } else {
        //count only these that match!
        for ( k = 0; k < keycount; k++ ) {

            akey         = keys[k];
            sizeofkey   = strlen( akey );
            if ( maxsizeofkey < sizeofkey ) maxsizeofkey = sizeofkey;

        }
        for ( k = 0; k < keycount; k++ ) {

            akey = keys[k];
            sizeofkey = strlen( akey );
            Serial.print( akey );

            for ( i = sizeofkey; i < maxsizeofkey; i++ )
                Serial.print(' ');
            Serial.print( F(" : ") );

            testdata = contains( (const uint8_t*)( akey ) );
            if ( testdata ) {
                Serial.println( (const char*)( testdata ) );
            } else {
                Serial.println();
            }
        }
    }
}
