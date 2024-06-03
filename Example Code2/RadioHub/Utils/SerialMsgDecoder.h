#ifndef _SerialMsgDecoder_H__
#define _SerialMsgDecoder_H__

/**
 * @file    SerialMsgDecoder.h
 *
 * @brief   Declaration of class SerialMsgDecoder
 *
 * @note    This example code is provided by IMST GmbH on an "AS IS" basis without any warranties.
 */

#include "SerialMessage.h"

#include <QJsonObject>

class SerialMsgDecoder
{
public:
                    SerialMsgDecoder() {}

    virtual  bool   Decode ( QJsonObject& results, const SerialMessage& serialMsg ) const = 0;

};

#endif // _SerialMsgDecoder_H__
