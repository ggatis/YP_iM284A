#ifndef __SerialRequest_H__
#define __SerialRequest_H__

/**
 * @file    SerialRequest.h
 *
 * @brief   Declaration of class SerialRequest
 *
 * @note    This example code is provided by IMST GmbH on an "AS IS" basis without any warranties.
 */

#include "SerialMessage.h"
#include <QJsonObject>

class SerialRequest
{
public:
                    SerialRequest(){};

    virtual bool    Encode( SerialMessage& /* serialMsg */ ) const { return false; }
    virtual bool    Encode( SerialMessage& /* serialMsg */, const QJsonObject& /* parameters */ ) const { return false; }

};

#endif // __SerialRequest_H__
