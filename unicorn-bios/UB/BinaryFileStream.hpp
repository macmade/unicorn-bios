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

#ifndef UB_BINARY_FILE_STREAM_HPP
#define UB_BINARY_FILE_STREAM_HPP

#include "UB/BinaryStream.hpp"
#include <string>
#include <iostream>
#include <cstdint>
#include <memory>
#include <algorithm>

namespace UB
{
    class BinaryFileStream: public BinaryStream
    {
        public:
            
            BinaryFileStream( std::string path );
            
            virtual ~BinaryFileStream( void );
            
            BinaryFileStream( const BinaryFileStream & o )              = delete;
            BinaryFileStream( BinaryFileStream && o )                   = delete;
            BinaryFileStream & operator =( const BinaryFileStream & o ) = delete;
            BinaryFileStream & operator =( BinaryFileStream && o )      = delete;
            
            using BinaryStream::read;
            
            void   read( uint8_t * buf, size_t size )        override;
            void   seek( ssize_t offset, SeekDirection dir ) override;
            size_t tell( void )                        const override;
            
        private:
            
            class IMPL;
            
            std::unique_ptr< IMPL > impl;
    };
}

#endif /* UB_BINARY_FILE_STREAM_HPP */
