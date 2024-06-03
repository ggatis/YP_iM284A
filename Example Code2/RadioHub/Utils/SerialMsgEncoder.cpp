/**
 * @file        SerialMsgEncoder.cpp
 *
 * @brief       Implementation of class SerialMsgEncoder
 *
 * @note        This example code is provided by IMST GmbH on an "AS IS" basis without any warranties.
 */

#include "SerialMsgEncoder.h"
#include <QVariant>

QMap < QString, SerialMsgEncoder::Encoder > SerialMsgEncoder::Encoders;

SerialMsgEncoder::SerialMsgEncoder()
{
}

void
SerialMsgEncoder::Init()
{
    if ( Encoders.isEmpty() )
    {
        Encoders.insert( "U8" ,         &Encode_U8 );
        Encoders.insert( "U16" ,        &Encode_U16 );
        Encoders.insert( "U32" ,        &Encode_U32 );
        Encoders.insert( "U64" ,        &Encode_U64 );
        Encoders.insert( "Bool",        &Encode_Bool );
        Encoders.insert( "ByteArray",   &Encode_ByteArray );
    }
}

bool
SerialMsgEncoder::Encode_Items( SerialMessage& serialMsg, const QJsonObject& config, const QStringList& items )
{
    for ( int i = 0; i < items.count(); i++ )
    {
        // every item consits of "key:type"
        QStringList item = items.at( i ).split( ":" );

        if ( item.count() == 2 )
        {
            QString key     = item.at( 0 );
            QString type    = item.at( 1 );

            if ( Encode_Item( serialMsg, config, key, type ) == false )
            {
                return false;
            }
        }
        else
        {
            return false;
        }
   }

   return true;
}

bool
SerialMsgEncoder::Encode_Item( SerialMessage& serialMsg, const QJsonObject& config, const QString& key, const QString& type )
{
    // find encoder for type
    Encoder encoder = Encoders.value( type, 0 );

    if ( encoder != 0 )
    {
        // call encoder
        return ( *encoder )( serialMsg, config, key );
    }

    return false;
}

bool
SerialMsgEncoder::Encode_U8( SerialMessage& serialMsg, const QJsonObject& config, const QString& key )
{
    QVariant help = config.value( key ).toVariant();

    bool ok;

    uint8_t val = (uint8_t)help.toUInt( &ok );

    if ( ok )
    {
        serialMsg.Append( val );
    }

    return ok;
}

bool
SerialMsgEncoder::Encode_U16( SerialMessage& serialMsg, const QJsonObject& config, const QString& key )
{
    QVariant help = config.value( key ).toVariant();

    bool ok;

    uint16_t val = (uint16_t)help.toUInt( &ok );

    if ( ok )
    {
        serialMsg.Append( val );
    }

    return ok;
}

bool
SerialMsgEncoder::Encode_U32( SerialMessage& serialMsg, const QJsonObject& config, const QString& key )
{
    QVariant help = config.value( key ).toVariant();

    bool ok;

    uint32_t val = (uint32_t)help.toUInt( &ok );

    if ( ok )
    {
        serialMsg.Append( val );
    }

    return ok;
}

bool
SerialMsgEncoder::Encode_U64( SerialMessage& serialMsg, const QJsonObject& config, const QString& key )
{
    QString help = config.value( key ).toString();

    bool ok;

    quint64 val = help.toULongLong( &ok );

    if ( ok )
    {
        serialMsg.Append( (uint64_t)val );
    }

    return ok;
}

bool
SerialMsgEncoder::Encode_Bool( SerialMessage& serialMsg, const QJsonObject& config, const QString& key )
{
    QJsonValue help = config.value( key );

    if ( help.isBool() )
    {
        uint8_t val = ( help.toBool( false ) == true ) ? 1 : 0;

        serialMsg.Append( val );

        return true;
    }
    return false;
}

bool
SerialMsgEncoder::Encode_ByteArray( SerialMessage& serialMsg, const QJsonObject& config, const QString& key )
{
    QByteArray payload = config.value( key ).toString().toUtf8();
    QByteArray data = QByteArray::fromBase64( payload );

    if ( !data.isEmpty() )
    {
        serialMsg.append( data );

        return true;
    }
    return false;
}
