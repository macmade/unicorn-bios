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

#ifndef UB_FAT_DAP_HPP
#define UB_FAT_DAP_HPP

#include <memory>
#include <algorithm>

namespace UB
{
    class BinaryStream;
    
    namespace FAT
    {
        class DAP
        {
            public:
                
                static size_t DataSize( void );
                
                DAP( void );
                DAP( BinaryStream & stream );
                DAP( const DAP & o );
                DAP( DAP && o ) noexcept;
                ~DAP( void );
                
                DAP & operator =( DAP o );
                
                uint8_t  size( void )                const;
                uint8_t  zero( void )                const;
                uint16_t numberOfSectors( void )     const;
                uint16_t destinationOffset( void )   const;
                uint16_t destinationSegment( void )  const;
                uint64_t logicalBlockAddress( void ) const;
                
                friend void swap( DAP & o1, DAP & o2 );
                
            private:
                
                class IMPL;
                std::unique_ptr< IMPL > impl;
        };
    }
}

#endif /* UB_FAT_DAP_HPP */
