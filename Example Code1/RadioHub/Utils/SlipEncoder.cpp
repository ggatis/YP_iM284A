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
