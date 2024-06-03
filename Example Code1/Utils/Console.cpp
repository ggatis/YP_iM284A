/**
 * @file    Console.cpp
 *
 * @brief   Implementation of class Console
 *
 * @note    This example code is free software: you can redistribute it and/or modify it.
 *
 *          This program is provided by IMST GmbH on an "AS IS" basis without
 *          any warranties in the hope that it will be useful.
 */

#include "Console.h"
#include <iostream>
#include <iomanip>
#include <stdlib.h>
#include <conio.h>  // for windows os, Linux, macOS might need a different soution

using namespace std;

enum Spaces
{
    LeftSpace   = 24,
    RightSpace  = 40
};

Console::Console()
       : _KeyEventHandler( nullptr )
{
}

bool
Console::RegisterKeyEventHandler( Console::KeyEventHandler* handler )
{
    if ( _KeyEventHandler == nullptr )
    {
        _KeyEventHandler = handler;

        // start peridic timer to poll for keyboard input events
        startTimer( 100 );

        return true;
    }
    return false;
}

void
Console::timerEvent( QTimerEvent* )
{
    // keyboard event pending ?
    if ( _kbhit() )
    {
        // yes, get it and forward it to registered handler
        int keyCode = getch();

        if ( _KeyEventHandler != nullptr )
        {
            _KeyEventHandler->OnKeyEvent( keyCode );
        }
    }
}

void
Console::ClearScreen()
{
    // clear screen, this might not work on every operating system !
    system( "@cls||clear" );
}

void
Console::Print( const QString& string )
{
    cout << string.toUtf8().data() << endl;
}

void
Console::PrintLine()
{
    Print( QByteArray( 80, '-' ) );
}

void
Console::Print( const char* key, const char* value )
{
    cout << setw( LeftSpace ) << right << key << "  :  " << left << setw( RightSpace ) << value << endl;
}
void
Console::Print( const QString& key, const QString& value )
{
    Print( key.toUtf8().data(), value.toUtf8().data() );
}

void
Console::Print( const QString& key, const QByteArray& data )
{
    Print( key.toUtf8().data(), data.toHex( ' ' ).data() );
}

void
Console::PrintResults( const QStringList& strings )
{
    for( QString string : strings )
    {

        QStringList help = string.split( ":" );

        if ( help.count() >= 2 )
        {
            Print( help.at( 0 ), help.at( 1 ) );
        }
        else
        {
            Print( string );
        }
    }
}

void
Console::PrintResults( const QJsonObject& results )
{
    QStringList keys = results.keys();

    for ( QString key : keys )
    {
        QJsonValue value = results.value( key );

        if ( value.isObject() )
        {
            cout << setw( LeftSpace ) << right << key.toUtf8().data() << endl;
            PrintResults( value.toObject() );
            cout << endl;
        }
        else if ( value.isString() )
        {
            Print( key, value.toString() );
        }
        else if ( value.isDouble() )
        {
            Print( key, QString::number( value.toDouble() ) );
        }
    }
}
