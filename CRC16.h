/**
 * @file    CRC16.h
 *
 * @brief   Declaration of class CRC16
 *
 * @note    This example code is free software: you can redistribute it and/or modify it.
 *
 *          This program is provided by EDI:
 *              on basis of IMST GmbH code,
 *              on an "AS IS" basis without any warranties in the hope that it will be useful.
 * 
 * Gatis Gaigals @ EDI, 2024 
 */

#ifndef _CRC16_H_
#define _CRC16_H_

#include <stdint.h>
#include "ByteArray.h"

/**
 * @brief   The CRC16 class provides methods for CRC calculation and checking. The implemented CRC uses the well known
 *          16 Bit CCITT Polynom. For performance reason a lookup-table is used which was generated by means of this polynom.
 */

class CRC16 {

    public:

        enum {
            Init_Value  =  0xFFFF,    //!< initial value for CRC algorithem
            Good_Value  =  0x0F47,    //!< constant compare value for check
            Polynom     =  0x8408     //!< 16 Bit CRC CCITT Generator Polynom, used for table generation
        };

        /**
          * @brief  class constructor
          *
          * @param  initValue   initial CRC16 value
         */
                    CRC16( uint16_t initValue = CRC16::Init_Value );

        /**
         * @brief   calculate CRC16
         *
         * @param   data  input data
         *
         * @return  crc16
         */
        uint16_t    Calc( const ByteArray &data );

        /**
         * @brief   calculate CRC16 according to X25 recommandation
         *
         * @param   data  input data
         *
         * @return  1's complement of crc16
         */
        uint16_t    Calc_X25( const ByteArray &data );

        /**
         * @brief   calculate and check CRC16
         *
         * @param   data  input data
         *
         * @return  true  - CRC16 ok
         *          false - CRC16 error
         */
        bool        Check_X25( const ByteArray &data );

    private:

        //<!  crc value
        uint16_t                _CRC;

        //<! static lookup table for fast calculation
        static const uint16_t   _Table[];

};

#endif // _CRC16_H_
