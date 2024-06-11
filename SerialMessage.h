/**
 * @file    SerialMessage.h
 *
 * @brief   Declaration of class SerialMessage
 *
 * @note    This example code is free software: you can redistribute it and/or modify it.
 *
 *          This program is provided by EDI:
 *              on basis of IMST GmbH code,
 *              on an "AS IS" basis without any warranties in the hope that it will be useful.
 * 
 * Gatis Gaigals @ EDI, 2024 
 */

#ifndef _SerialMessage_H_
#define _SerialMessage_H_

#include <stdint.h>
#include <string>
#include "ByteArray.h"
//#include <QString>


/**
 * @brief   The class SerialMessage extends QByteArray with following functionalities
 *          - access to dedicated message fields: header fields, payload
 *          - CRC16 calculation and checking
 *
 * @note    QByteArray provides basic array functionality and memory management for byte arrays
 *
 */

class SerialMessage : public ByteArray {

public:

    // serial message format
    enum MessageFormat
    {
        SapID_Index             =   0,
        MsgID_Index             =   1,

        Header_Size             =   2,
        Response_Header_Size    =   3,
        CRC_Size                =   2,

        // for reception
        Min_Size                =   ( Header_Size + CRC_Size ),

        // status field in response messages
        EventData_Index         =   2,
        Status_Index            =   2,
        ResponseData_Index      =   3
    };

    enum
    {
        InvalidSapID            =   0xFF,
        InvalidMsgID            =   0xFF,
        InvalidStatus           =   0xFF,
        InvalidLength           =   -1
    };

    /**
     * @brief   class constructor
     */
                SerialMessage();

                SerialMessage( uint8_t sapID, uint8_t msgID );

    /**
     * @brief   check CRC16
     *
     * @return  true  - CRC16 ok
     *          false - CRC16 error
     */
    bool        CheckCRC16() const;

    /**
     * @brief   remove trailing CRC16
     */
    void        RemoveCRC16();

    /**
     * @return  service access point identifier
     */
    uint8_t     GetSapID() const;

    /**
     * @return  message identifier
     */
    uint8_t     GetMsgID() const;

    /**
     * @return  status of response message
     */
    uint8_t     GetResponseStatus() const;

    /**
     * @return  payload length
     */
    int         GetPayloadLength() const;

    /**
     * @return  response payload length
     */
    int         GetResponsePayloadLength() const;

    /**
     * @return  return total message length
     */
    int         GetTotalLength() const;

    /**
     * @return  unsigned 8 Bit value
     *
     * @param   index   index to array
     */
    uint8_t     GetU8( int index ) const;

    /**
     * @return  signed 8 Bit value
     *
     * @param   index   index to array
     */
    int8_t      GetI8( int index ) const;

    /**
     * @return  unsigned 16 Bit value (LSB first)
     *
     * @param   index   index to array
     */
    uint16_t    GetU16( int index ) const;

    /**
     * @return  unsigned 32 Bit value (LSB first)
     *
     * @param   index   index to array
     */
    uint32_t    GetU32( int index ) const;

    /**
     * @return  unsigned 64 Bit value (LSB first)
     *
     * @param   index   index to array
     */
    uint64_t    GetU64( int index ) const;


    /**
     * @return  pointer to const uint8_t* for a given index
     *
     * @param   index   index to array
     */
    const uint8_t*  GetData( int index ) const;


    /**
     * @return  array of bytes from payload field
     *
     * @param   index   index to array
     */
    ByteArray   GetPayload( int index, int size = -1 ) const;


    /**
     * @return  ASCII string
     *
     * @param   index   index to array
     */
    std::string GetString( int index , int size = -1 ) const;

    /**
     * @return  hexadecimal formated string
     *
     * @param   index   index to array
     */
    std::string GetHexString( int index = 0, int size = -1 ) const;

    /**
     * @return  hexadecimal formated string with LSB first order
     *
     * @param   index   index to array
     */
    std::string GetHexString_LSB( int index = 0, int size = -1 ) const;

    /**
     * @return  seconds since epoch as human readable date time string
     *
     * @param   index   index to array
     */
    std::string GetDateTime( int index, const std::string& formatString ) const;

     /**
      * @brief   init request for transmission
      *
      * @param   sapID   service accesspoint identifier
      * @param   msgID   message identifier
      */
    void        InitRequest( uint8_t sapID, uint8_t msgID );

     /**
      * @brief   append U8 value
      *
      * @param   8 Bit value
      *
      * @return  number of appended bytes (1)
      */
    int         Append( uint8_t value );

    /**
      * @brief   append U16 value (LSB first)
      *
      * @param   16 Bit value
      *
      * @return  number of appended bytes (2)
      */
    int         Append( uint16_t value );

    /**
     * @brief   append U32 value (LSB first)
     *
     * @param   32 Bit value
     *
     * @return  number of appended bytes (4)
     */
    int         Append( uint32_t value );

    /**
     * @brief   append U64 value (LSB first)
     *
     * @param   64 Bit value
     *
     * @return  number of appended bytes (8)
     */
    int         Append( uint64_t value );

    /**
     * @brief   append string of hex encoded bytes, seperated by '-'
     *
     * @param   string ( e.g. 00-01-02-03-04... )
     *
     * @return  number of appended bytes
     */
    int         AppendHexString( const std::string& input );

    /**
     * @brief   append string of hex encoded bytes, seperated by '-'
     *
     * @param   string ( e.g. 00-01-02-03-04... )
     *
     * @return  number of appended bytes
     */
    int         AppendHexString_LSB( const std::string& input );

    /**
     * @brief   calculate and append CRC16 for message transmission
     *
     * @return  number of bytes appended (2)
     */
    int         Append_CRC16();
};

#endif // _SerialMessage_H_
