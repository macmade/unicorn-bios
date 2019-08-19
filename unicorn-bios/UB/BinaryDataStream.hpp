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

#ifndef UB_BINARY_DATA_STREAM_HPP
#define UB_BINARY_DATA_STREAM_HPP

#include "UB/BinaryStream.hpp"
#include <string>
#include <iostream>
#include <cstdint>
#include <memory>
#include <algorithm>

namespace UB
{
    class BinaryDataStream: public BinaryStream
    {
        public:
            
            BinaryDataStream( void );
            BinaryDataStream( const std::vector< uint8_t > & data );
            BinaryDataStream( const BinaryDataStream & o );
            BinaryDataStream( BinaryDataStream && o ) noexcept;
            
            virtual ~BinaryDataStream( void );
            
            BinaryDataStream & operator =( BinaryDataStream o );
            
            using BinaryStream::read;
            
            void   read( uint8_t * buf, size_t size )        override;
            void   seek( ssize_t offset, SeekDirection dir ) override;
            size_t tell( void )                        const override;
            
            BinaryDataStream & operator +=( const BinaryDataStream & stream );
            BinaryDataStream & operator +=( const std::vector< uint8_t > & data );
            
            void append( const BinaryDataStream & stream );
            void append( const std::vector< uint8_t > & data );
            
            friend void swap( BinaryDataStream & o1, BinaryDataStream & o2 );
            
        private:
            
            class IMPL;
            
            std::unique_ptr< IMPL > impl;
    };
}

#endif /* UB_BINARY_DATA_STREAM_HPP */
