#ifndef __SerialMsgEncoder_H__
#define __SerialMsgEncoder_H__

/**
 * @file    SerialMsgEncoder.h
 *
 * @brief   Declaration of class SerialMsgEncoder
 *
 * @note    This example code is provided by IMST GmbH on an "AS IS" basis without any warranties.
 */

#include "SerialMessage.h"

#include <QString>
#include <QStringList>
#include <QJsonObject>
#include <QMap>

class SerialMsgEncoder
{
public:
                    SerialMsgEncoder();

    static void     Init();
    static bool     Encode_Items        ( SerialMessage& serialMsg, const QJsonObject& config, const QStringList& items );
    static bool     Encode_Item         ( SerialMessage& serialMsg, const QJsonObject& config, const QString& item, const QString& type );

private:

    static bool     Encode_U8           ( SerialMessage& serialMsg, const QJsonObject& config, const QString& key );
    static bool     Encode_U16          ( SerialMessage& serialMsg, const QJsonObject& config, const QString& key );
    static bool     Encode_U32          ( SerialMessage& serialMsg, const QJsonObject& config, const QString& key );
    static bool     Encode_U64          ( SerialMessage& serialMsg, const QJsonObject& config, const QString& key );

    static bool     Encode_Bool         ( SerialMessage& serialMsg, const QJsonObject& config, const QString& key );
    static bool     Encode_ByteArray    ( SerialMessage& serialMsg, const QJsonObject& config, const QString& key );

    static bool     Encode_Modulation   ( SerialMessage& serialMsg, const QJsonObject& config, const QString& key );

private:

    typedef bool ( *Encoder )( SerialMessage& serialMsg, const QJsonObject& config, const QString& key );

    static QMap < QString, Encoder > Encoders;
};

#endif // __SerialMsgEncoder_H__
