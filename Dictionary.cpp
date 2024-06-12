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

static char snBuffer[20];

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
uint16_t
Dictionary::sizeof_key( uint16_t n ) const {
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
uint16_t
Dictionary::sizeof_data( uint16_t n ) const {
    uint16_t    dicsize = _ByteArray.count();
    uint8_t*    pbegin  = _ByteArray.data();
    uint8_t*    pkeyn   = data( n );
    return scan0FromHere( pkeyn, dicsize - (uint16_t)( pkeyn - pbegin ) );
}


/**
 * @brief   returns byte count in array
 *
 * @param   key and data to append
 *
 * @return  byte count in array
 */
uint16_t
Dictionary::append( const char* data, bool Continue ) {
    uint8_t*    plimit  = _ByteArray.data() + _ByteArray.size();
    //overwrite last 0
    uint8_t*    pactual = Continue ?
         ( _ByteArray.count() ? 
            _ByteArray.data() + _ByteArray.count() - 1:
            _ByteArray.data() ):
         _ByteArray.data() + _ByteArray.count();
    for ( ; pactual < plimit; ) {
        if ( 0 == ( *pactual++ = (uint8_t)*data++ ) ) goto skip_endmarking;
    }
    *( pactual - 1 ) = 0;
skip_endmarking:
    _ByteArray.update_count( (uint16_t)( pactual - _ByteArray.data() ) );
    return _ByteArray.count();
}


/**
 * @brief   returns byte count in array
 *
 * @param   key and data to append
 *
 * @return  byte count in array
 */
uint16_t
Dictionary::appendU8( uint8_t n, bool Continue ) {

    const char* format = "%d";
    snprintf( snBuffer, sizeof( snBuffer ), format, n );

    return append( (const char*)snBuffer, Continue );
}


/**
 * @brief   returns byte count in array
 *
 * @param   key and data to append
 *
 * @return  byte count in array
 */
uint16_t
Dictionary::appendU32( uint32_t n, bool Continue ) {

    const char* format = "%d";
    snprintf( snBuffer, sizeof( snBuffer ), format, n );

    return append( (const char*)snBuffer, Continue );
}


/**
  * @brief   returns byte count in array
  *
  * @param   byte  byte to append
  *
  * @return  byte count in array
  */
uint16_t
Dictionary::append( const char* akey, char* data ) {
    _keys++;
/*    uint8_t*    plimit  = _ByteArray.data() + _ByteArray.size();
    uint8_t*    pactual = _ByteArray.data() + _ByteArray.count();
    for ( ; pactual < plimit; ) {
        if ( 0 == ( *pactual++ = (uint8_t)*akey++ ) ) goto skip_endmarking1;
    }
    *( pactual - 1 ) = 0;
skip_endmarking1:
    for ( ; pactual < plimit; ) {
        if ( 0 == ( *pactual++ = (uint8_t)*data++ ) ) goto skip_endmarking2;
    }
    *( pactual - 1 ) = 0;
skip_endmarking2:
    _ByteArray.update_count( (uint16_t)( pactual - _ByteArray.data() ) );
    return _ByteArray.count();
*/
    return append( akey, false ) + append( data, false );
}


/**
  * @brief   returns byte count in array
  *
  * @param   byte  byte to append
  *
  * @return  byte count in array
  */
uint16_t
Dictionary::append( const char* akey, const char* data ) {
    _keys++;
    uint8_t*    plimit  = _ByteArray.data() + _ByteArray.size();
    uint8_t*    pactual = _ByteArray.data() + _ByteArray.count();
    for ( ; pactual < plimit; ) {
        if ( 0 == ( *pactual++ = (uint8_t)*akey++ ) ) goto skip_endmarking3;
    }
    *( pactual - 1 ) = 0;
skip_endmarking3:
    for ( ; pactual < plimit; ) {
        if ( 0 == ( *pactual++ = (uint8_t)*data++ ) ) goto skip_endmarking4;
    }
    *( pactual - 1 ) = 0;
skip_endmarking4:
    _ByteArray.update_count( (uint16_t)( pactual - _ByteArray.data() ) );
    return _ByteArray.count();
}


/**
  * @brief   returns byte count in array
  *
  * @param   byte  byte to append
  *
  * @return  byte count in array
  */
uint16_t
Dictionary::append( const char* akey, const uint8_t* data ) {
    _keys++;
    uint8_t*    plimit  = _ByteArray.data() + _ByteArray.size();
    uint8_t*    pactual = _ByteArray.data() + _ByteArray.count();
    for ( ; pactual < plimit; ) {
        if ( 0 == ( *pactual++ = (uint8_t)*akey++ ) ) goto skip_endmarking5;
    }
    *( pactual - 1 ) = 0;
skip_endmarking5:
    for ( ; pactual < plimit; ) {
        if ( 0 == ( *pactual++ = (uint8_t)*data++ ) ) goto skip_endmarking6;
    }
    *( pactual - 1 ) = 0;
skip_endmarking6:
    _ByteArray.update_count( (uint16_t)( pactual - _ByteArray.data() ) );
    return _ByteArray.count();
}


/**
  * @brief   returns byte count in array
  *
  * @param   byte  byte to append
  *
  * @return  byte count in array
  */
uint16_t
Dictionary::append( const char* akey, const uint8_t* data, int size ) {
    _keys++;
    uint8_t*    plimit  = _ByteArray.data() + _ByteArray.size();
    uint8_t*    pactual = _ByteArray.data() + _ByteArray.count();

    if ( pactual < plimit ) {
        for ( ; pactual < plimit; ) {
            if ( 0 == ( *pactual++ = (uint8_t)*akey++ ) ) goto skip_endmarking7;
        }
        *( pactual - 1 ) = 0;   //ja izmet peec for nosaciijuma
    }
skip_endmarking7:
    if ( size < 0 ) {
        if ( pactual < plimit ) {
            for ( ; pactual < plimit; ) {
                if ( 0 == ( *pactual++ = *data++ ) ) goto skip_endmarking8;
            }
            *( pactual - 1 ) = 0;   //ja izmet peec for nosaciijuma
        }
skip_endmarking8:
        ;
    } else {
        //constant data size if allowed
        while ( size-- ) {
            if ( pactual < plimit ) {
                *pactual++ = *data++;
            }
        }
        if ( *( pactual - 1 ) ) {
            if ( pactual < plimit ) {
                *pactual++ = 0;
            } else {
                *( pactual - 1 ) = 0;
            }
        }
    }
    _ByteArray.update_count( (uint16_t)( pactual - _ByteArray.data() ) );
    return _ByteArray.count();
}


/**
 * @brief   returns byte count in array
 *
 * @param   key and data to append
 *
 * @return  byte count in array
 */
uint16_t
Dictionary::append( const char* akey, uint8_t n ) {
    _keys++;
    return append( akey, false ) + appendU8( n, false );
}


/**
 * @brief   returns byte count in array
 *
 * @param   key and data to append
 *
 * @return  byte count in array
 */
uint16_t
Dictionary::append( const char* akey, uint32_t n ) {
    _keys++;
    return append( akey, false ) + appendU32( n, false );
}


/**
  * @brief   returns byte count in array
  *
  * @param   byte  byte to append
  *
  * @return  byte count in array
  */
uint16_t
Dictionary::append( const char* akey, std::string& aString ) {
    //void fromCString(const char* aCString) {
    //    _size = std::strlen(aCString);
    //    _count = _size;
    //    _data = new uint8_t[_size];
    //    std::memcpy(_data, reinterpret_cast<const uint8_t*>(aCString), _size);
    //}
    _keys++;
    uint8_t*    plimit  = _ByteArray.data() + _ByteArray.size();
    uint8_t*    pactual = _ByteArray.data() + _ByteArray.count();
    for ( ; pactual < plimit; ) {
        if ( 0 == ( *pactual++ = (uint8_t)*akey++ ) ) goto skip_endmarking9;
    }
    *( pactual - 1 ) = 0;   //ja izmet peec for nosaciijuma
skip_endmarking9:
    char c;
    for ( uint16_t i = 0; i < aString.size(); ++i ) {
        c = aString[i];
        if ( pactual < plimit ) {
            *pactual++ = c;
        }
        if ( 0 == c ) break;
    }
    if ( *( pactual - 1 ) ) {
        if ( pactual < plimit ) {
            *pactual++ = 0;
        } else {
            *( pactual - 1 ) = 0;
        }
    }
    _ByteArray.update_count( (uint16_t)( pactual - _ByteArray.data() ) );
    return _ByteArray.count();
}


/**
  * @brief   returns byte count in array
  *
  * @param   byte  byte to append
  *
  * @return  byte count in array
  */
uint16_t
Dictionary::append( const char* akey, std::string&& aString ) {
    //void fromCString(const char* aCString) {
    //    _size = std::strlen(aCString);
    //    _count = _size;
    //    _data = new uint8_t[_size];
    //    std::memcpy(_data, reinterpret_cast<const uint8_t*>(aCString), _size);
    //}
    _keys++;
    uint8_t*    plimit  = _ByteArray.data() + _ByteArray.size();
    uint8_t*    pactual = _ByteArray.data() + _ByteArray.count();
    for ( ; pactual < plimit; ) {
        if ( 0 == ( *pactual++ = (uint8_t)*akey++ ) ) goto skip_endmarkingA;
    }
    *( pactual - 1 ) = 0;   //ja izmet peec for nosaciijuma
skip_endmarkingA:
    char c;
    for ( uint16_t i = 0; i < aString.size(); ++i ) {
        c = aString[i];
        if ( pactual < plimit ) {
            *pactual++ = c;
        }
        if ( 0 == c ) break;
    }
    if ( *( pactual - 1 ) ) {
        if ( pactual < plimit ) {
            *pactual++ = 0;
        } else {
            *( pactual - 1 ) = 0;
        }
    }
    _ByteArray.update_count( (uint16_t)( pactual - _ByteArray.data() ) );
    return _ByteArray.count();
}


/**
  * @brief   returns byte count in array
  *
  * @param   byte  byte to append
  *
  * @return  byte count in array
  */
/*
uint16_t
Dictionary::append( const uint8_t* akey, std::string& aString ) {
    _keys++;
    uint8_t*    plimit  = _ByteArray.data() + _ByteArray.size();
    uint8_t*    pactual = _ByteArray.data() + _ByteArray.count();
    for ( ; pactual < plimit; ) {
        if ( 0 == ( *pactual++ = *akey++ ) ) break;
    }
    char c;
    for ( uint16_t i = 0; i < aString.size(); ++i ) {
        c = aString[i];
        *pactual++ = c;
        if ( 0 == c ) break;
    }
    if ( c ) *pactual++ = 0;
    _ByteArray.update_count( (uint16_t)( pactual - _ByteArray.data() ) );
    return (uint16_t)( pactual - _ByteArray.data() );
}
*/

/**
 * @brief   
 *
 * @param   
 *
 * @return  
 */
uint16_t
Dictionary::append( const char* akey, const Dictionary aDictionary, char delimiter ) {
    uint16_t oldCount = _ByteArray.count();
    uint8_t b;
    uint8_t* puint8;
    //aDictionary keys
    for ( uint16_t k = 0; k < aDictionary.keys(); k++ ) {
        //akey, no 0 after
        if ( _ByteArray.size() == _ByteArray.count() )
            goto finita;
        for ( uint16_t i = 0; ; i++ ) {
            b = (uint8_t)akey[i];
            if ( 0 == b ) break;
            _ByteArray.append( b );
            if ( _ByteArray.size() == _ByteArray.count() )
                goto finita;
        }
        _ByteArray.append( (uint8_t)delimiter );
        if ( _ByteArray.size() == _ByteArray.count() )
            goto finita;
        //key
        puint8 = aDictionary.key( k );
        for ( uint16_t i = 0; ; i++ ) {
            b = puint8[i];
            _ByteArray.append( b );
            if ( _ByteArray.size() == _ByteArray.count() )
                goto finita;
            if ( 0 == b ) break;
        }
        //safety measure ensuring 0 after key
        if ( _ByteArray.at( _ByteArray.count() ) ) {
            if ( _ByteArray.size() == _ByteArray.count() ) {
                goto finita;
            } else {
                _ByteArray.append( 0 );
            }
        }
        //key
        puint8 = aDictionary.data( k );
        for ( uint16_t i = 0; i < ( _ByteArray.size() - _ByteArray.count() ); i++ ) {
            uint8_t b = puint8[i];
            _ByteArray.append( b );
            if ( _ByteArray.size() == _ByteArray.count() )
                goto finita;
            if ( 0 == b ) break;
        }
        //safety measure ensuring 0 after data
        if ( _ByteArray.at( _ByteArray.count() ) ) {
            if ( _ByteArray.size() == _ByteArray.count() ) {
                goto finita;
            } else {
                _ByteArray.append( 0 );
            }
        }
    }
    if ( _ByteArray.count() == _ByteArray.size() ) {
finita:
        //overdose
        if ( _ByteArray.size() ) {
            _ByteArray.update_count( _ByteArray.size() - 1 );
            _ByteArray.append( 0 );
        }
    }
    return _ByteArray.count() - oldCount;
}


/**
 * @brief   finds if the dictionary contains a record with a given key
 *
 * @param   akey    search key
 *
 * @return  the pos of the data
 */
const uint8_t*
Dictionary::contains( const char* akey ) const {
    uint16_t k;
    const uint8_t* testkey;
    const uint8_t* keycopy;

    uint16_t sizeofkey = strlen( akey );    //optimizeet!
    if ( 0 == sizeofkey++ ) return nullptr; //sizeofkey++ include zero in check

    for ( k = 0; k < _keys; k++ ) {
        testkey = key( k );
        for ( keycopy = (const uint8_t*)akey; keycopy < (const uint8_t*)( akey + sizeofkey ); ) {
            if ( *testkey++ != *keycopy++ ) goto try_next;
        }
        if ( (uint16_t)( testkey - _ByteArray.data() ) > _ByteArray.size() ) {
            //key fits, no data due the size shortage
            return _ByteArray.data() + _ByteArray.size() - 1;
        } else {
            return testkey;
        }
try_next:
        ;   //at least an empty statement as a tribute to the God of labels
    }
    return nullptr;
}

/**
 * @brief   finds if the dictionary contains a record with a given key
 *
 * @param   -
 *
 * @return  the pos of the data
 */
const uint8_t*
Dictionary::contains( const uint8_t* akey ) const {
    uint16_t k;
    const uint8_t* testkey;
    const uint8_t* keycopy;

    uint16_t sizeofkey = strlen( (const char*)akey );
    if ( 0 == sizeofkey++ ) return nullptr; //sizeofkey++ include zero in check

    for ( k = 0; k < _keys; k++ ) {
        testkey = key( k );
        for ( keycopy = akey; keycopy < ( akey + sizeofkey ); ) {
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
uint16_t
Dictionary::remove( const uint8_t* akey ) {
    uint16_t deleted = 0;

    const uint16_t sizeofkey = strlen( (const char*)akey );
    if ( 0 == sizeofkey ) return deleted;

    const uint8_t* ptrdata = contains( akey );
    if ( ptrdata ) {
        //Houston we have data!
        //outer frame
        uint8_t* MrFirst = _ByteArray.data();
        uint8_t* MrLast  = _ByteArray.data() + _ByteArray.count();  //actually next to last
        //inner frame
        uint8_t* first = const_cast<uint8_t*>( ptrdata - 1 - sizeofkey );   //minus zero sugar
        const uint16_t sizeofdata = strlen( (const char*)ptrdata ); //optimizeet!
        uint8_t* last  = const_cast<uint8_t*>( ptrdata + sizeofdata + 1 );  //next to zero sugar
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
