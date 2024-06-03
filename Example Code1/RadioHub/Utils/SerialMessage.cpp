/**
 * @file    SerialMessage.cpp
 *
 * @brief   Implementation of class SerialMessage
 *
 * @note    This example code is free software: you can redistribute it and/or modify it.
 *
 *          This program is provided by IMST GmbH on an "AS IS" basis without
 *          any warranties in the hope that it will be useful.
 */

#include "SerialMessage.h"
#include "CRC16.h"

SerialMessage::SerialMessage()
{

}

SerialMessage::SerialMessage( uint8_t sapID, uint8_t msgID )
{
    InitRequest( sapID, msgID );
}

uint8_t
SerialMessage::GetSapID() const
{
    if ( count() >= ( Header_Size ) )
    {
        return (uint8_t)at( SapID_Index );
    }
    return InvalidSapID;
}

uint8_t
SerialMessage::GetMsgID() const
{
    if ( count() >= ( Header_Size ) )
    {
        return (uint8_t)at( MsgID_Index );
    }
    return InvalidMsgID;
}

uint8_t
SerialMessage::GetResponseStatus() const
{
    if ( count() > ( Status_Index ) )
    {
        return (uint8_t)at( Status_Index );
    }
    return InvalidStatus;
}

int
SerialMessage::GetPayloadLength() const
{
    if ( count() >= ( Header_Size ) )
    {
        return ( count() - ( Header_Size ) );
    }
    return InvalidLength;
}

uint8_t
SerialMessage::GetU8( int index ) const
{
    if ( count() >= ( index + 1 ) )
    {
        return (uint8_t)at( index );
    }
    return 0;
}

uint16_t
SerialMessage::GetU16( int index ) const
{
    if ( count() >= ( index + 2 ) )
    {
        return (uint16_t)( (uint8_t)at( index ) <<  0 ) |
               (uint16_t)( (uint8_t)at( 1 + index ) <<  8 );
    }
    return 0;
}

uint32_t
SerialMessage::GetU32( int index ) const
{
    if ( count() >= ( index + 4 ) )
    {
        return (uint32_t)( (uint8_t)at( index ) <<  0 ) |
               (uint32_t)( (uint8_t)at( 1 + index ) <<  8 ) |
               (uint32_t)( (uint8_t)at( 2 + index ) << 16 ) |
               (uint32_t)( (uint8_t)at( 3 + index ) << 24 );
    }
    return 0;
}

uint64_t
SerialMessage::GetU64( int index ) const
{
    if ( count() >= ( index + 8 ) )
    {
        uint32_t lo = GetU32( index );
        uint32_t hi = GetU32( index + 4 );

        return (uint64_t)( ( (uint64_t)hi << 32 ) |  lo );
    }
    return 0;
}


QByteArray
SerialMessage::GetPayload( int index, int size ) const
{
    if ( size != -1 )
    {
        if ( count() >= ( index + size ) )
        {
            // return remaining payload
            return mid( index, size );
        }
    }
    else
    {
        // return remaining part of payload
        if ( count() > ( index ) )
        {
            // return remaining payload
            return mid( index, size );
        }
    }

    // return empty array
    return QByteArray();
}

void
SerialMessage::InitRequest( uint8_t sapID, uint8_t msgID )
{
    // clear buffer for init
    clear();

    // attach HCI header
    append( sapID );
    append( msgID );
}

int
SerialMessage::Append( uint8_t value )
{
    // LSB first
    append( (uint8_t)( value ) );

    // 1 byte appended
    return 1;
}

int
SerialMessage::Append( uint16_t value )
{
    // LSB first
    append( (uint8_t)( value ) );
    append( (uint8_t)( value >> 8 ) );

    // 2 bytes appended
    return 2;
}

int
SerialMessage::Append( uint32_t value )
{
    // LSB first
    append( (uint8_t)( value ) );
    append( (uint8_t)( value >> 8 ) );
    append( (uint8_t)( value >> 16 ) );
    append( (uint8_t)( value >> 24 ) );

    // 4 bytes appended
    return 4;
}

int
SerialMessage::Append( uint64_t value )
{
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
SerialMessage::Append_CRC16()
{
    CRC16   crc16;

    // get reference to base class
    const QByteArray& data = *this;

    // append one's complement of crc
    return Append( (uint16_t)crc16.Calc_X25( data ) );
}

bool
SerialMessage::CheckCRC16() const
{
    CRC16   crc16;

    // get reference to base class
    const QByteArray& data = *this;

    return crc16.Check_X25( data );
}

void
SerialMessage::RemoveCRC16()
{
    if ( count() >= Min_Size )
    {
        // remove trailing crc bytes
        chop( CRC_Size );
    }
}
