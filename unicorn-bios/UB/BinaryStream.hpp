/*******************************************************************************
 * The MIT License (MIT)
 * 
 * Copyright (c) 2019 Jean-David Gadina - www.xs-labs.com
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 ******************************************************************************/

#ifndef UB_BINARY_STREAM_HPP
#define UB_BINARY_STREAM_HPP

#include <string>
#include <cstdint>
#include <vector>

namespace UB
{
    class BinaryStream
    {
        public:
            
            enum class SeekDirection
            {
                Current,
                Begin,
                End
            };
            
            virtual ~BinaryStream( void ) = default;
            
            virtual void   Read( uint8_t * buf, size_t size )        = 0;
            virtual void   Seek( ssize_t offset, SeekDirection dir ) = 0;
            virtual size_t Tell( void )                        const = 0;
            
            bool   HasBytesAvailable( void );
            size_t AvailableBytes( void );
            
            void Seek( ssize_t offset );
            
            std::vector< uint8_t > Read( size_t size );
            std::vector< uint8_t > ReadAll( void );
            
            uint8_t ReadUInt8( void );
            int8_t  ReadInt8( void );
            
            uint16_t ReadUInt16( void );
            int16_t  ReadInt16( void );
            uint16_t ReadBigEndianUInt16( void );
            uint16_t ReadLittleEndianUInt16( void );
            
            uint32_t ReadUInt32( void );
            int32_t  ReadInt32( void );
            uint32_t ReadBigEndianUInt32( void );
            uint32_t ReadLittleEndianUInt32( void );
            
            uint64_t ReadUInt64( void );
            int64_t  ReadInt64( void );
            uint64_t ReadBigEndianUInt64( void );
            uint64_t ReadLittleEndianUInt64( void );
            
            float ReadBigEndianFixedPoint( unsigned int integerLength, unsigned int fractionalLength );
            float ReadLittleEndianFixedPoint( unsigned int integerLength, unsigned int fractionalLength );
            
            std::string ReadNULLTerminatedString( void );
            std::string ReadPascalString( void );
            std::string ReadString( size_t length );
    };
}

#endif /* UB_BINARY_STREAM_HPP */
