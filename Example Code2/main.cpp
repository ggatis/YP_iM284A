/**
 * @file    main.cpp
 *
 * @brief   Implementation of application entry point "main"
 *
 * @note    This example code is free software: you can redistribute it and/or modify it.
 *
 *          This program is provided by IMST GmbH on an "AS IS" basis without
 *          any warranties in the hope that it will be useful.
 */

#include <QCoreApplication>

#include "LoRa_Mesh_DemoApp.h"
#include "Utils/Console.h"

/**
 * @brief   entry point of this application
 *
 * @param   argc    number of command line arguments
 *
 * @param   argv    array with command line arguments
 *
 * @return  exit code
 */
int
main( int argc, char* argv[] )
{
    Console             console;
    LoRaMesh_DemoApp    demoApp( console );
    QString             portName;

    // port name available on command line?
    if ( argc >= 2 )
    {
        // yes, take port name from command line
        portName = QString( argv[ 1 ] );
    }

    QCoreApplication app( argc, argv );

    demoApp.Enable( portName );

    return app.exec();
}
