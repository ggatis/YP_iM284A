#ifndef __SerialMsgAccessPoint_H__
#define __SerialMsgAccessPoint_H__

/**
 * @file    SerialMsgAccessPoint.h
 *
 * @brief   Declaration of class SerialMsgAccessPoint
 *
 * @note    This example code is provided by IMST GmbH on an "AS IS" basis without any warranties.
 */

#include "SerialMsgDecoder.h"
#include "SerialMsgEncoder.h"

#include <QMap>

class SerialMsgAccessPoint
{
public:
                            SerialMsgAccessPoint();


    virtual uint8_t         GetID() const = 0;

    virtual void            Init() {}

    void                    Register( uint8_t msgID, SerialMsgDecoder* msgDecoder );

    bool                    Decode( QJsonObject& results, const SerialMessage& serialMsg );

private:

    QMap < uint8_t , SerialMsgDecoder* >    MsgDecoders;

};

#endif // __SerialMsgAccessPoint_H__
