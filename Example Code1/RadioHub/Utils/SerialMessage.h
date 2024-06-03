/**
 * @file    SerialMessage.h
 *
 * @brief   Declaration of class SerialMessage
 *
 * @note    This example code is free software: you can redistribute it and/or modify it.
 *
 *          This program is provided by IMST GmbH on an "AS IS" basis without
 *          any warranties in the hope that it will be useful.
 */

#ifndef __SerialMessage_H__
#define __SerialMessage_H__

#include <QByteArray>
#include <stdint.h>

/**
 * @brief   The class SerialMessage extends QByteArray with following functionalities
 *          - access to dedicated message fields: header fields, payload
 *          - CRC16 calculation and checking
 *
 * @note    QByteArray provides basic array functionality and memory management for byte arrays
 *
 */

class SerialMessage : public QByteArray
{
public:

    // serial message format
    enum MessageFormat
    {
        SapID_Index     =   0,
        MsgID_Index     =   1,

        Header_Size     =   2,
        CRC_Size        =   2,

        // for reception
        Min_Size        =   ( Header_Size + CRC_Size ),

        // status field in response messages
        Status_Index    =   2,
    };

    enum
    {
        InvalidSapID    =   0xFF,
        InvalidMsgID    =   0xFF,
        InvalidStatus   =   0xFF,
        InvalidLength   =   -1
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
     * @return  U8 value
     *
     * @param   index   index to array
     */
    uint8_t     GetU8( int index ) const;

    /**
     * @return  U16 value (LSB first)
     *
     * @param   index   index to array
     */
    uint16_t    GetU16( int index ) const;

    /**
     * @return  U32 value (LSB first)
     *
     * @param   index   index to array
     */
    uint32_t    GetU32( int index ) const;

    /**
     * @return  U64 value (LSB first)
     *
     * @param   index   index to array
     */
    uint64_t    GetU64( int index ) const;

    /**
     * @return  array of bytes from payload field
     *
     * @param   index   index to array
     */
    QByteArray  GetPayload( int index , int size = -1 ) const;

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
     * @brief   calculate and append CRC16 for message transmission
     *
     * @return  number of bytes appended (2)
     */
    int         Append_CRC16();
};

#endif // __SerialMessage_H__
