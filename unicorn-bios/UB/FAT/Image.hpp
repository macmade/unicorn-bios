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

#ifndef UB_FAT_IMAGE_HPP
#define UB_FAT_IMAGE_HPP

#include <memory>
#include <algorithm>
#include <string>
#include <cstdint>
#include <vector>
#include "UB/FAT/MBR.hpp"

namespace UB
{
    namespace FAT
    {
        class Image
        {
            public:
                
                Image( const std::string & path );
                Image( const Image & o );
                Image( Image && o ) noexcept;
                ~Image( void );
                
                Image & operator =( Image o );
                
                std::string path( void ) const;
                MBR         mbr( void )  const;
                
                std::vector< uint8_t > read( uint8_t sectors, uint8_t cylinder, uint8_t sector, uint8_t head ) const;
                
                friend void swap( Image & o1, Image & o2 );
                
            private:
                
                class IMPL;
                std::unique_ptr< IMPL > impl;
        };
    }
}

#endif /* UB_FAT_IMAGE_HPP */
