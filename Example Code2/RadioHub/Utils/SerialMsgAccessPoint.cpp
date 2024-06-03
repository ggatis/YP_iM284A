/**
 * @file        SerialMsgAccessPoint.cpp
 *
 * @brief       Implementation of class SerialMsgAccessPoint
 *
 * @note        This example code is provided by IMST GmbH on an "AS IS" basis without any warranties.
 */

#include "SerialMsgAccessPoint.h"
#include <QDebug>

SerialMsgAccessPoint::SerialMsgAccessPoint()
{

}

void
SerialMsgAccessPoint::Register( uint8_t msgID, SerialMsgDecoder* msgDecoder )
{
    MsgDecoders.insert( msgID, msgDecoder );
}

bool
SerialMsgAccessPoint::Decode( QJsonObject& results, const SerialMessage& serialMsg )
{
    SerialMsgDecoder* decoder = MsgDecoders.value( serialMsg.GetMsgID(), 0 );

    if ( decoder )
    {
        return decoder->Decode( results, serialMsg );
    }

    return false;
}

