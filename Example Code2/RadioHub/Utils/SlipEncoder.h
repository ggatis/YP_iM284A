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

//#define SlipEncoder_BufferLess_Version

/**
 * @brief   The SlipEncoder class encodes a byte stream into a SLIP encoded byte stream
 */

class SlipEncoder
{
public:

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

#ifdef  SlipEncoder_BufferLess_Version
    /**
     * @brief   class constructor
     */
                            SlipEncoder();

    /**
     * @brief   prepare encoder for SLIP message
     *
     * @param   input               array with message to be encoded
     * @param   numWakeupChars      optional number of wakeup chars which s
     *                              should be transmitted first
     */

    bool                    SetInput( const QByteArray* input, uint16_t numWakeupChars = 0 );

    /**
     * @brief   return a single SLIP encoded byte
     *
     * @return  if >= 0 -> SLIP encoded byte in lower 8 bit
     *          else       last byte has already been encoded -> UART transmitter can be configured
     *                     for final TX SHIFT REGISTER empty interrupt now
     *
     * @note    don't forget to call OnCompleteIndication() to cleanup encoding state for next message
     */

    int16_t                 GetEncodedByte();

    /**
     * @brief   handle completion event which should occur after
     *          the last byte has left the UART TX SHIFT register
     */

    void                    OnCompleteIndication();

#else

    /**
     * @brief   encode byte stream ( this version is only recommended if RAM is not an expensive resource )
     *
     * @param   output      encoded SLIP stream
     * @param   input       bytes to encode
     *
     * @return  output      updated output buffer with SLIP encoded byte stream
     */

    static QByteArray&      Encode( QByteArray& output, const QByteArray& input );

#endif

private:

#ifdef  SlipEncoder_BufferLess_Version
    enum EncoderState
    {
        Idle    =   0,
        Wakeup,
        Start,
        InFrame,
        EscEndState,
        EscEscState,
        WaitForCompletion,
        EndState
    };

    EncoderState            _State;
    const QByteArray*       _Input;
    int                     _Index;
    int                     _NumWakeupChars;
#endif
};

#endif // __Slip_Encoder_H__
