/**
 * @file    SlipEncoder.h
 *
 * @brief   Declaration of class SlipEncoder
 *
 * @note    This example code is free software: you can redistribute it and/or modify it.
 *
 *          This program is provided by IMST GmbH on an "AS IS" basis without
 *          any warranties in the hope that it will be useful.
 */

#ifndef __Slip_Encoder_H__
#define __Slip_Encoder_H__

#include <QByteArray>

/**
 * @brief   The SlipEncoder class encodes a byte stream into a SLIP encoded byte stream
 */

class SlipEncoder
{
public:

    /**
     * @brief   encode byte stream
     *
     * @param   output      encoded SLIP stream
     * @param   input       bytes to encode
     *
     * @return  output      updated output buffer with SLIP encoded byte stream
     */
    static QByteArray&  Encode( QByteArray& output, const QByteArray& input );

private:

    /**
     * standard SLIP frame characters
     */
    enum FrameCharacters
    {
        Begin   =   0xC0,
        End     =   0xC0,
        Esc     =   0xDB,
        EscEnd  =   0xDC,
        EscEsc  =   0xDD
    };
};

#endif // __Slip_Encoder_H__
