/**
 * @file    SlipEncoder.cpp
 *
 * @brief   Implementation of class SlipEncoder.
 *
 * @note    This example code is free software: you can redistribute it and/or modify it.
 *
 *          This program is provided by IMST GmbH on an "AS IS" basis without
 *          any warranties in the hope that it will be useful.
 */

#include "SlipEncoder.h"

#ifdef SlipEncoder_BufferLess_Version

/**
 * @brief   class constructor
 */

SlipEncoder::SlipEncoder()
           : _State         ( SlipEncoder::Idle )
           , _Input         ( nullptr )
           , _Index         ( 0 )
           , _NumWakeupChars( 0 )

{
}

/**
 * @brief   prepare encoder for SLIP message
 *
 * @param   input               array with message to be encoded
 * @param   numWakeupChars      optional number of wakeup chars which s
 *                              should be transmitted first
 */

bool
SlipEncoder::SetInput( const QByteArray* input, uint16_t numWakeupChars )
{
    if ( ( _State == SlipEncoder::Idle ) && ( input != nullptr ) )
    {
        _Input          = input;
        _Index          = 0;
        _NumWakeupChars = numWakeupChars;


        if ( _NumWakeupChars > 0 )
        {
            _State = SlipEncoder::Wakeup;
        }
        else
        {
           _State = SlipEncoder::Start;
        }
        return true;
    }
    return false;
}

/**
 * @brief   return a single SLIP encoded byte
 *
 * @return  if >= 0 -> SLIP encoded byte in lower 8 bit
 *          else       last byte has already been encoded -> UART transmitter can be configured
 *                     for final TX SHIFT REGISTER empty interrupt now
 *
 * @note    don't forget to call OnCompleteIndication() to cleanup encoding state for next message
 */

int16_t
SlipEncoder::GetEncodedByte()
{
    switch ( _State )
    {
        // send wakeup chars first, so that PLL and baudrate generator on peer device can settle
        case SlipEncoder::Wakeup:


            if ( 0 >= --_NumWakeupChars )
            {
                _State = Start;
            }
            return SlipEncoder::End;

        // start of frame --> send SLIP_END
        case SlipEncoder::Start:

            _State = SlipEncoder::InFrame;
            return SlipEncoder::End;

        // second step of SLIP_END coding --> send SLIP_ESC_END
        case SlipEncoder::EscEndState:

            _State = SlipEncoder::InFrame;
            return SlipEncoder::EscEnd;

        // second step of SLIP_ESC coding --> send SLIP_ESC_ESC
        case SlipEncoder::EscEscState:

            _State = SlipEncoder::InFrame;
            return SlipEncoder::EscEsc;

        // last byte transmitted --> return EOF so that UART can be disabled
        case SlipEncoder::WaitForCompletion:

            _State = SlipEncoder::EndState;  // wait for final UART TX SHIFT Register empty interrupt!
            return -1;

        // normal coding
        case    SlipEncoder::InFrame:
            {
                // eof ?
                if ( _Input->count() <= _Index )
                {
                    // end of frame --> send terminating SLIP_END
                    _State = SlipEncoder::WaitForCompletion;
                    return SlipEncoder::End;
                }

                // get next txByte
                uint8_t txByte = _Input->at( _Index++ );

                // special character --> send SLIP_ESC
                if ( txByte == SlipEncoder::End )
                {
                    _State = SlipEncoder::EscEndState;
                    return SlipEncoder::Esc;
                }
                if ( txByte == SlipEncoder::Esc )
                {
                    _State = SlipEncoder::EscEscState;
                    return SlipEncoder::Esc;
                }

                // normal character --> send it
                return (int16_t)txByte;
            }

        case Idle:
        case EndState:
        default:

            // return EOF to disable driver
            return -1;
    }
}

/**
 * @brief   handle completion event which should occur after
 *          the last byte has left the UART TX SHIFT register
 */

void
SlipEncoder::OnCompleteIndication()
{
    if ( _State == SlipEncoder::EndState )
    {
        _Input = nullptr;
        _Index = 0;
        _State = Idle;
    }
}

#else
/**
 * @brief   encode message
 *
 * @param   output      buffer for encoded message
 *
 * @param   input       buffer with original messsage
 *
 * @return  reference to output buffer
 */
QByteArray&
SlipEncoder::Encode( QByteArray& output, const QByteArray& input )
{
    output.append( SlipEncoder::End );

    for ( int index = 0; index < input.count(); index++ )
    {
        uint8_t byte = (uint8_t)input.at( index );

        switch ( byte )
        {
            case  SlipEncoder::End:
                    output.append( SlipEncoder::Esc );
                    output.append( SlipEncoder::EscEnd );
                    break;

            case  SlipEncoder::Esc:
                    output.append( SlipEncoder::Esc );
                    output.append( SlipEncoder::EscEsc );
                    break;

            default:
                    output.append( byte );
                    break;
        }
    }

    output.append( SlipEncoder::End );

    return output;
}
#endif
