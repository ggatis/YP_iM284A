/**
 * @file    Console.h
 *
 * @brief   Declaration of class Console
 *
 * @note    This example code is free software: you can redistribute it and/or modify it.
 *
 *          This program is provided by IMST GmbH on an "AS IS" basis without
 *          any warranties in the hope that it will be useful.
 */

#ifndef __Console_H__
#define __Console_H__

#include <QObject>
#include <QString>
#include <QStringList>
#include <QJsonObject>

/**
 * @brief   The Console class offers methods for simple string output
 */

class Console : public QObject
{
public:

    /**
     * @brief   simple keyboard event handler
     */
    class KeyEventHandler
    {
    public:

        /**
         * @brief   handler for keyboared input events
         */
        virtual void OnKeyEvent( int /* keyCode */ ){}
    };

    /**
     * @brief class constructor
     */
    Console();

    /**
     * @brief   register a key event handler
     * @param   handler
     * @return  true/false
     */
    bool RegisterKeyEventHandler( Console::KeyEventHandler* handler );

    /**
     * @brief   clear the console screen
     */
    void ClearScreen();

    /**
      * @brief  print a string
      *
      * @param  string      the string to print
      */
    void Print( const QString& string );


    /**
     *  @brief  print a line
     */
    void PrintLine();

    /**
      * @brief  print a string and value
      *
      * @param  string      the string to print
      * @param  value       the value to print
      */
    void Print( const char* key, const char* value );

    /**
      * @brief  print a string and value
      *
      * @param  string      the string to print
      * @param  value       the value to print
      */
    void Print( const QString& key, const QString& value );

    /**
      * @brief  print a string and array of bytes
      *
      * @param  string      the string to print
      * @param  value       the byte array to print
      */
    void Print( const QString& key, const QByteArray& value );


    /**
     * @brief   print a list of key value pairs
     *
     * @param   strings     the list of strings to print
     */
    void PrintResults( const QStringList& strings );

    /**
     * @brief   print a json object
     *
     * @param   results     the json object to print
     */
    void PrintResults( const QJsonObject& results );

private:

    /**
     * @brief   check for keyboard input in periodic timer event
     */
    void timerEvent( QTimerEvent* ) override;

private:

    //<! handler for keyboard input events
    KeyEventHandler*    _KeyEventHandler;
};

#endif // __Console_H__
