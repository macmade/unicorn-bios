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

#ifndef UB_MEMORY_MAP_HPP
#define UB_MEMORY_MAP_HPP

#include <memory>
#include <algorithm>
#include <cstdint>
#include <vector>

namespace UB
{
    namespace BIOS
    {
        class MemoryMap
        {
            public:
                
                class Entry
                {
                    public:
                        
                        enum class Type: uint32_t
                        {
                            Usable   = 0x01,
                            Reserved = 0x02,
                            ACPI     = 0x03
                        };
                        
                        Entry( uint64_t base, uint64_t length, Type type );
                        Entry( const Entry & o );
                        Entry( Entry && o ) noexcept;
                        ~Entry( void );
                        
                        Entry & operator =( Entry o );
                        
                        uint64_t base( void )       const;
                        uint64_t end( void )        const;
                        uint64_t length( void )     const;
                        Type     type( void )       const;
                        uint32_t baseLow( void )    const;
                        uint32_t baseHigh( void )   const;
                        uint32_t lengthLow( void )  const;
                        uint32_t lengthHigh( void ) const;
                        
                        friend void swap( Entry & o1, Entry & o2 );
                        
                    private:
                        
                        class IMPL;
                        std::unique_ptr< IMPL > impl;
                };
                
                MemoryMap( size_t memory );
                MemoryMap( const MemoryMap & o );
                MemoryMap( MemoryMap && o ) noexcept;
                ~MemoryMap( void );
                
                MemoryMap & operator =( MemoryMap o );
                
                std::vector< Entry > entries( void ) const;
                
                friend void swap( MemoryMap & o1, MemoryMap & o2 );
                
            private:
                
                class IMPL;
                std::unique_ptr< IMPL > impl;
        };
    }
}

#endif /* UB_MEMORY_MAP_HPP */
