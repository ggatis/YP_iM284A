'''
 @file    SerialMessage.py
 
 @brief   This class provides basic functions for encoding and decoding of HCI messages.
 
 @note    This example code is free software: you can redistribute it and/or modify it.
 
          This program is provided by IMST GmbH on an "AS IS" basis without
          any warranties in the hope that it will be useful.
 '''

from crccheck.crc import Crc16IsoHdlc
import struct
import time

class SerialMessage:

    Status_Index        =   0x00
    EventData_Index     =   0x00
    ResponseData_Index  =   0x01
      
    def __init__( self, sap_id = None, msg_id = None, payload = None ):
        self.sap_id = sap_id
        self.msg_id = msg_id

        if payload == None:
            self.payload = b''
        else:    
            self.payload = payload

        self.fcs = None

    def calc_crc( self ):
        data = bytes( [ self.sap_id ] ) + bytes( [ self.msg_id ] ) + self.payload
        crc = Crc16IsoHdlc.calc( data )
        self.fcs = struct.pack('<H', crc )

    def encode( self ):
        self.calc_crc()
        data = bytes( [ self.sap_id ] ) + bytes( [ self.msg_id ] ) + self.payload + bytes( self.fcs )
        return data
        
    def decode( self, data ) -> bool:

        if ( len( data ) < 4):
            return False

        self.sap_id = data[ 0 ]
        self.msg_id = data[ 1 ]
        self.payload = data [ 2: -2 ]
        self.calc_crc()

        return ( self.fcs == data[ -2: ] )
         
    def getPayloadLength( self ) -> int:      
        return len( self.payload )
    
    def getU8( self, index : int ) -> int:
        return int( self.payload[ index ] )
    
    def getI8( self, index : int ) -> int:
        i8 = struct.unpack('b', self.payload[ index:index+1 ])[0]
        return int( i8 )
    
    def getStatus( self ) -> int:
        return self.getU8( self.Status_Index )
        
    def getU16( self, index : int ) -> int:
        # little-endian
        u16 = struct.unpack('<H', self.payload[ index:index+2 ])[0]
        return int( u16 )

    def getU16_MSB( self, index : int ) -> int:
        # big-endian
        u16 = struct.unpack('>H', self.payload[ index:index+2 ])[0]
        return int( u16 )

    def getU32( self, index : int ) -> int:
        # litte-endian
        u32 = struct.unpack('<I', self.payload[ index:index+4 ])[0]
        return int( u32 )

    def getU32_MSB( self, index : int ) -> int:
        # big-endian
        u32 = struct.unpack('>I', self.payload[ index:index+4 ])[0]
        return int( u32 )

    def getString( self, index : int, size : int ) -> str:
        if ( size == -1 ):
            size = len( self.payload ) - index
        help = self.payload[ index:index+size]    
        return help.decode( 'utf-8' )

    def getHexString( self, index : int, size : int ) ->str:
        if ( size == -1 ):
            size = len( self.payload ) - index
        help = self.payload[ index:index+size]    
        s = help.hex()
        return '-'.join( s[i:i+2] for i in range( 0, len( s ), 2 ) )

    def getHexString_LSB( self, index : int, size : int ) ->str:
        if ( size == -1 ):
            size = len( self.payload ) - index
        help = self.payload[ index:index+size]    
        help2 = help[::-1]
        s = help2.hex()
        return '-'.join( s[i:i+2] for i in range( 0, len( s ), 2 ) )

    def getDateTime( self, index : int ):
        u32 = struct.unpack('<I', self.payload[ index:index+4 ])[0]
        return time.gmtime( self.getU32( index ) )
    
        
        