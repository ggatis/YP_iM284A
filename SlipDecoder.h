/**
 * @file    SlipDecoder.h
 *
 * @brief   Declaration of class SlipDecoder
 *
 * @note    This example code is free software: you can redistribute it and/or modify it.
 *
 *          This program is provided by EDI:
 *              on basis of IMST GmbH code,
 *              on an "AS IS" basis without any warranties in the hope that it will be useful.
 * 
 * Gatis Gaigals @ EDI, 2024 
 */

#ifndef _Slip_Decoder_H_
#define _Slip_Decoder_H_

#include "ByteArray.h"

/**
 * @brief   The SlipDecoder class decodes SLIP encoded byte streams.
 */

class SlipDecoder {

public:

    /**
     * @brief The Client class
     */

    class Client {

    public:
        //<! handler for received SLIP messages
        virtual void OnSlipDecoder_MessageReady( const ByteArray& /* message */ ) { }
    };


    /**
     * @brief   class constructor
     */
                SlipDecoder( SlipDecoder::Client* client );

    /**
     * @brief   reset decoder
     */
    void        Reset();

    /**
     * @brief   decode encoded SLIP stream
     *
     * @param   output      decoded frame
     * @param   input       SLIP encoded byte stream
     *
     * @note    on signal "OnFrameReady" the decoded SLIP frame is ready the output array
     */
    void        Decode( ByteArray& output, const ByteArray& input );

    /**
     * @brief   decode a byte from the encoded SLIP stream
     *
     * @param   output      decoded frame
     * @param   input       a byte from the encoded SLIP byte stream
     *
     * @note    on signal "OnFrameReady" the decoded SLIP frame is ready the output array
     */
    void        Decode( ByteArray& output, int input );

    /**
     * @brief   decode an encoded SLIP stream using flagged callback
     *
     * @param   output          decoded frame
     * @param   flByteStream    function returning a byte/-1 from the encoded SLIP byte stream
     *
     * @note    on signal "OnFrameReady" the decoded SLIP frame is ready the output array
     */
    void        Decode( ByteArray& output, int (*const flByteStream)( void ) );

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

    /**
     * decoder states
     */
    enum DecoderState
    {
        Initial = 0,
        InFrame,
        EscState
    };

    //<! decoder client
    SlipDecoder::Client*    _DecoderClient;

    //<! decoder state
    DecoderState            _State;
};

#endif // _Slip_Decoder_H_
