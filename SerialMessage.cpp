/**
 * @file    SerialMessage.cpp
 *
 * @brief   Implementation of class SerialMessage
 *
 * @note    This example code is free software: you can redistribute it and/or modify it.
 *
 *          This program is provided by EDI:
 *              on basis of IMST GmbH code,
 *              on an "AS IS" basis without any warranties in the hope that it will be useful.
 * 
 * Gatis Gaigals @ EDI, 2024 
 */

#include "SerialMessage.h"
#include "CRC16.h"

//#include <QDateTime>
#include <chrono>
#include <ctime>


SerialMessage::SerialMessage() {
}


SerialMessage::SerialMessage( uint8_t sapID, uint8_t msgID ) {
    InitRequest( sapID, msgID );
}


uint8_t
SerialMessage::GetSapID() const {
    if ( count() >= ( Header_Size ) ) {
        return (uint8_t)at( SapID_Index );
    }
    return InvalidSapID;
}


uint8_t
SerialMessage::GetMsgID() const {
    if ( count() >= ( Header_Size ) ) {
        return (uint8_t)at( MsgID_Index );
    }
    return InvalidMsgID;
}


uint8_t
SerialMessage::GetResponseStatus() const {
    if ( count() > ( Status_Index ) ) {
        return (uint8_t)at( Status_Index );
    }
    return InvalidStatus;
}


int
SerialMessage::GetTotalLength() const {
    return count();
}


int
SerialMessage::GetPayloadLength() const {
    if ( count() >= ( Header_Size ) ) {
        return ( count() - ( Header_Size ) );
    }
    return InvalidLength;
}


int
SerialMessage::GetResponsePayloadLength() const {
    if ( count() >= ( Response_Header_Size ) ) {
        return ( count() - ( Response_Header_Size ) );
    }
    return InvalidLength;
}


uint8_t
SerialMessage::GetU8( int index ) const {
    if ( count() >= ( index + 1 ) ) {
        return (uint8_t)at( index );
    }
    return 0;
}


int8_t
SerialMessage::GetI8( int index ) const {
    if ( count() >= ( index + 1 ) ) {
        return (int8_t)at( index );
    }
    return 0;
}


uint16_t
SerialMessage::GetU16( int index ) const {
    if ( count() >= ( index + 2 ) ) {
        return (uint16_t)( (uint8_t)at( index ) <<  0 ) |
               (uint16_t)( (uint8_t)at( 1 + index ) <<  8 );
    }
    return 0;
}


uint32_t
SerialMessage::GetU32( int index ) const {
    if ( count() >= ( index + 4 ) ) {
        return (uint32_t)( (uint8_t)at( index ) <<  0 ) |
               (uint32_t)( (uint8_t)at( 1 + index ) <<  8 ) |
               (uint32_t)( (uint8_t)at( 2 + index ) << 16 ) |
               (uint32_t)( (uint8_t)at( 3 + index ) << 24 );
    }
    return 0;
}


uint64_t
SerialMessage::GetU64( int index ) const {
    if ( count() >= ( index + 8 ) ) {
        uint32_t lo = GetU32( index );
        uint32_t hi = GetU32( index + 4 );
        return (uint64_t)( ( (uint64_t)hi << 32 ) |  lo );
    }
    return 0;
}


const uint8_t*
SerialMessage::GetData( int index ) const {
    if ( ( 0 <= index ) && ( index <= count() ) ) {
        return (const uint8_t*)( data() + index );
    }
    return nullptr;
}


ByteArray
SerialMessage::GetPayload( int index, int size ) const {
    if ( size != -1 ) {
        if ( count() >= ( index + size ) ) {
            // return remaining payload
            return mid( index, size );
        }
    } else {
        // return remaining part of payload
        if ( count() > ( index ) ) {
            // return remaining payload
            return mid( index, size );
        }
    }

    // return empty array
    return ByteArray();
}


std::string
SerialMessage::GetString( int index, int size ) const {
    //return std::string( GetPayload( index, size ) );
    if ( size < 0 ) {
        return std::string( reinterpret_cast<const char*>( data() + index ), count() );
    }
    return std::string( reinterpret_cast<const char*>( data() + index ), size );
}


static uint8_t _hex_table[16] = {
        '0', '1', '2', '3',
        '4', '5', '6', '7',
        '8', '9', 'A', 'B',
        'C', 'D', 'E', 'F'
    };


std::string
SerialMessage::GetHexString( int index , int size ) const {

    ByteArray rawData = GetPayload( index, size );

    std::string result;
    for( int i = 0; i < rawData.size(); i++ ) {

        result += _hex_table[ rawData.at( i ) >> 4 ];
        result += _hex_table[ rawData.at( i )  & 7 ];
        result += '-';

    }

    uint16_t rsize = result.length();
    if ( rsize )
            return result.substr( 0, rsize - 1 );

    return result;
}


std::string
SerialMessage::GetHexString_LSB( int index , int size ) const {

    ByteArray rawData = GetPayload( index, size );

    std::string result;
    for( int i = rawData.size() - 1; i >= 0; i-- ) {

        result += _hex_table[ rawData.at( i ) >> 4 ];
        result += _hex_table[ rawData.at( i )  & 7 ];
        result += "-";

    }

    uint16_t rsize = result.length();
    if ( rsize )
            return result.substr( 0, rsize - 1 );

    return result;
}


std::string
SerialMessage::GetDateTime( int index , const std::string& formatString) const {

    std::time_t seconds = GetU32( index );
    std::tm *local_time = std::localtime( &seconds );
    return std::asctime( local_time );
}


void
SerialMessage::InitRequest( uint8_t sapID, uint8_t msgID ) {
    // clear buffer for init
    clear();

    // attach HCI header
    append( sapID );
    append( msgID );
}


int
SerialMessage::Append( uint8_t value ) {
    //LSB first
    //append( (uint8_t)( value ) );
    append( value );
    //1 byte appended
    return 1;
}


int
SerialMessage::Append( uint16_t value ) {
    // LSB first
    append( (uint8_t)( value ) );
    append( (uint8_t)( value >> 8 ) );
    // 2 bytes appended
    return 2;
}


int
SerialMessage::Append( uint32_t value ) {
    // LSB first
    append( (uint8_t)( value ) );
    append( (uint8_t)( value >> 8 ) );
    append( (uint8_t)( value >> 16 ) );
    append( (uint8_t)( value >> 24 ) );
    // 4 bytes appended
    return 4;
}


int
SerialMessage::Append( uint64_t value ) {
    // LSB first
    append( (uint8_t)( value ) );
    append( (uint8_t)( value >> 8 ) );
    append( (uint8_t)( value >> 16 ) );
    append( (uint8_t)( value >> 24 ) );
    append( (uint8_t)( value >> 32 ) );
    append( (uint8_t)( value >> 40 ) );
    append( (uint8_t)( value >> 48 ) );
    append( (uint8_t)( value >> 56 ) );
    // 8 bytes appended
    return 8;
}


int
SerialMessage::AppendHexString( const std::string& input ) {
    //ByteArray payload = input.toUtf8().replace( '-', "" );
    ByteArray payload = ByteArray( input );
    ByteArray data = ByteArray::fromHex( payload ); //ignores '-'
    if ( data.count() ) {
        //jaataisa!!!
        //append( data );
        for( int i = 0; i < data.count(); i++ )
            append( data.at( i ) );
        return data.count();
    }
    return 0;
}


int
SerialMessage::AppendHexString_LSB( const std::string& input ) {
    //ByteArray payload = input.toUtf8().replace( '-', "" );
    ByteArray payload = ByteArray( input );
    ByteArray data = ByteArray::fromHex( payload );

    if ( data.count() ) {
        for( int i = data.count() - 1; i >= 0; i-- )
            append( data.at( i ) );
        return data.count();
    }
    return 0;
}


int
SerialMessage::Append_CRC16() {
    CRC16   crc16;

    // get reference to base class
    const ByteArray& data = *this;

    // append one's complement of crc
    return Append( (uint16_t)crc16.Calc_X25( data ) );
}


bool
SerialMessage::CheckCRC16() const {
    CRC16   crc16;

    // get reference to base class
    const ByteArray& data = *this;

    return crc16.Check_X25( data );
}


void
SerialMessage::RemoveCRC16() {
    if ( count() >= Min_Size ) {
        // remove trailing crc bytes
        chop( CRC_Size );
    }
}
