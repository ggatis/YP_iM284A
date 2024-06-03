/**
 * @file    SlipDecoder.cpp
 *
 * @brief   Implementation of class SlipDecoder.
 *
 * @note    This example code is free software: you can redistribute it and/or modify it.
 *
 *          This program is provided by IMST GmbH on an "AS IS" basis without
 *          any warranties in the hope that it will be useful.
 */

#include "SlipDecoder.h"

SlipDecoder::SlipDecoder( SlipDecoder::Client* client )
           : DecoderClient( client )
           , State( SlipDecoder::Initial )
{

}

void
SlipDecoder::Reset()
{
    State = SlipDecoder::Initial;
}

void
SlipDecoder::Decode( QByteArray& output, const QByteArray& input )
{
    for ( int index = 0; index < input.count(); index++ )
    {
        uint8_t byte = (uint8_t)input.at( index );

        switch ( State )
        {
            case SlipDecoder::Initial:

                // begin of SLIP frame ?
                if ( byte == SlipDecoder::Begin )
                {
                    // reset output buffer
                    output.clear();

                    State = SlipDecoder::InFrame;
                }
                break;

            case SlipDecoder::InFrame:

                // end of SLIP frame ?
                if ( byte == SlipDecoder::End )
                {
                    State = SlipDecoder::Initial;

                    // notify client that SLIP frame is ready in output buffer
                    if ( DecoderClient )
                    {
                        DecoderClient->OnSlipDecoder_MessageReady( output );
                    }

                    // reset output buffer
                    output.clear();
                }
                // SLIP esc ?
                else if ( byte == SlipDecoder::Esc )
                {
                    State = SlipDecoder::EscState;
                }
                else
                {
                    // default case
                    output.append( byte );
                }
                break;

            case  SlipDecoder::EscState:

                // end of escape state ?
                if ( byte == SlipDecoder::EscEnd )
                {
                    output.append( SlipDecoder::End );

                    State = InFrame;
                }
                // end of escape state ?
                else if ( byte == SlipDecoder::EscEsc )
                {
                    output.append( SlipDecoder::Esc );

                    State = SlipDecoder::InFrame;
                }
                else // error
                {
                    // abort frame reception -> return to initial state
                    State = SlipDecoder::Initial;
                }
                break;

        } // switch ( State )

    } // for...

}
