/**
 * @file    SlipDecoder.cpp
 *
 * @brief   Implementation of class SlipDecoder.
 *
 * @note    This example code is free software: you can redistribute it and/or modify it.
 *
 *          This program is provided by EDI:
 *              on basis of IMST GmbH code,
 *              on an "AS IS" basis without any warranties in the hope that it will be useful.
 * 
 * Gatis Gaigals @ EDI, 2024 
 */

#include "SlipDecoder.h"

/**
 * @brief       class contructor
 *
 * @param       client      pointer to client
 */
SlipDecoder::SlipDecoder( SlipDecoder::Client* client )
           : _DecoderClient( client )
           , _State( SlipDecoder::Initial ) {
}

/**
 * @brief   reset decoder
 */

void
SlipDecoder::Reset() {
    _State = SlipDecoder::Initial;
}

/**
 * @brief   decode SLIP stream
 *
 * @param   output      decoded data
 *
 * @param   input       incoming SLIP encoded data
 *
 * @note    _Client->OnSlipDecoder_MessageReady is called with decoded SLIP message
 *
 */
void
SlipDecoder::Decode( ByteArray& output, const ByteArray& input ) {

    for ( int index = 0; index < input.count(); index++ ) {

        uint8_t byte = (uint8_t)input.at( index );

        switch ( _State ) {
        case SlipDecoder::Initial:

            // begin of SLIP frame ?
            if ( SlipDecoder::Begin == byte ) {
                // reset output buffer
                output.clear();
                _State = SlipDecoder::InFrame;
            }
            break;

        case SlipDecoder::InFrame:

            // end of SLIP frame ?
            if ( SlipDecoder::End == byte ) {
                // notify client that SLIP frame is ready in output buffer
                if ( _DecoderClient && ( output.count() > 0 ) ) {
                    _DecoderClient->OnSlipDecoder_MessageReady( output );
                }
                // reset output buffer
                output.clear();
            }
            // SLIP esc ?
            else if ( SlipDecoder::Esc == byte ) {
                _State = SlipDecoder::EscState;
            }
            // default case
            else {
                output.append( byte );
            }
            break;

        case  SlipDecoder::EscState:

            // end of escape state ?
            if ( SlipDecoder::EscEnd == byte ) {
                output.append( SlipDecoder::End );
                _State = InFrame;
            }
            // end of escape state ?
            else if ( SlipDecoder::EscEsc == byte ) {
                output.append( SlipDecoder::Esc );
                _State = SlipDecoder::InFrame;
            }
            // error
            else {
                // abort frame reception -> return to initial state
                _State = SlipDecoder::Initial;
            }
            break;

        } // switch ( _State )

    } // for...
}

/**
 * @brief   decode a byte from the encoded SLIP stream
 *
 * @param   output      decoded frame
 * @param   input       a byte from the encoded SLIP byte stream
 *
 * @note    on signal "OnFrameReady" the decoded SLIP frame is ready the output array
 */
void
SlipDecoder::Decode( ByteArray& output, int input ) {

    if ( -1 < input ) {

        uint8_t byte = (uint8_t)input;

        switch ( _State ) {
        case SlipDecoder::Initial:

            //begin of SLIP frame ?
            if ( SlipDecoder::Begin == byte ) {
                // reset output buffer
                output.clear();
                _State = SlipDecoder::InFrame;
            }
            break;

        case SlipDecoder::InFrame:

            //end of SLIP frame ?
            if ( SlipDecoder::End == byte ) {
                //notify client that SLIP frame is ready in output buffer
                if ( _DecoderClient && ( output.count() > 0 ) ) {
                    _DecoderClient->OnSlipDecoder_MessageReady( output );
                }
                //reset output buffer
                output.clear();
            }
            //SLIP esc ?
            else if ( SlipDecoder::Esc == byte ) {
                _State = SlipDecoder::EscState;
            }
            //default case
            else {
                output.append( byte );
            }
            break;

        case  SlipDecoder::EscState:

            //end of escape state ?
            if ( SlipDecoder::EscEnd == byte ) {
                output.append( SlipDecoder::End );
                _State = InFrame;
            }
            //end of escape state ?
            else if ( SlipDecoder::EscEsc == byte ) {
                output.append( SlipDecoder::Esc );
                _State = SlipDecoder::InFrame;
            }
            //error
            else {
                //abort frame reception -> return to initial state
                _State = SlipDecoder::Initial;
            }
            break;

        } //switch ( _State )

    }
}

/**
 * @brief   decode an encoded SLIP stream using flagged callback
 *
 * @param   output          decoded frame
 * @param   flByteStream    function returning a byte/-1 from the encoded SLIP byte stream
 *
 * @note    on signal "OnFrameReady" the decoded SLIP frame is ready the output array
 */
void
SlipDecoder::Decode( ByteArray& output, int (*const flByteStream)( void ) ) {

    int input;

    while ( -1 < ( input = flByteStream() ) ) {

        uint8_t byte = (uint8_t)input;

        switch ( _State ) {
        case SlipDecoder::Initial:

            //begin of SLIP frame ?
            if ( SlipDecoder::Begin == byte ) {
                //reset output buffer
                output.clear();
                _State = SlipDecoder::InFrame;
            }
            break;

        case SlipDecoder::InFrame:

            //end of SLIP frame ?
            if ( SlipDecoder::End == byte ) {
                //notify client that SLIP frame is ready in output buffer
                if ( _DecoderClient && ( output.count() > 0 ) ) {
                    _DecoderClient->OnSlipDecoder_MessageReady( output );
                }
                //reset output buffer
                output.clear();
            }
            //SLIP esc ?
            else if ( SlipDecoder::Esc == byte ) {
                _State = SlipDecoder::EscState;
            }
            // default case
            else {
                output.append( byte );
            }
            break;

        case  SlipDecoder::EscState:

            //end of escape state ?
            if ( SlipDecoder::EscEnd == byte ) {
                output.append( SlipDecoder::End );
                _State = InFrame;
            }
            //end of escape state ?
            else if ( SlipDecoder::EscEsc == byte ) {
                output.append( SlipDecoder::Esc );
                _State = SlipDecoder::InFrame;
            }
            //error
            else {
                //abort frame reception -> return to initial state
                _State = SlipDecoder::Initial;
            }
            break;

        } //switch( _State )

    } //while...
}
