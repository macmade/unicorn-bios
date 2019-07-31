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

#include "UB/BIOS/MemoryMap.hpp"

namespace UB
{
    namespace BIOS
    {
        class MemoryMap::Entry::IMPL
        {
            public:
                
                IMPL( uint64_t base, uint64_t length, Type type );
                IMPL( const IMPL & o );
                ~IMPL( void );
                
                uint64_t _base;
                uint64_t _length;
                Type     _type;
        };

        MemoryMap::Entry::Entry( uint64_t base, uint64_t length, Type type ):
            impl( std::make_unique< IMPL >( base, length, type ) )
        {}

        MemoryMap::Entry::Entry( const Entry & o ):
            impl( std::make_unique< IMPL >( *( o.impl ) ) )
        {}

        MemoryMap::Entry::Entry( Entry && o ) noexcept:
            impl( std::move( o.impl ) )
        {}

        MemoryMap::Entry::~Entry( void )
        {}

        MemoryMap::Entry & MemoryMap::Entry::operator =( Entry o )
        {
            swap( *( this ), o );
            
            return *( this );
        }
        
        uint64_t MemoryMap::Entry::base( void ) const
        {
            return this->impl->_base;
        }
        
        uint64_t MemoryMap::Entry::end( void ) const
        {
            if( this->impl->_length == 0 )
            {
                return this->impl->_base;
            }
            
            return this->impl->_base + ( this->impl->_length - 1 );
        }
        
        uint64_t MemoryMap::Entry::length( void ) const
        {
            return this->impl->_length;
        }
        
        MemoryMap::Entry::Type MemoryMap::Entry::type( void ) const
        {
            return this->impl->_type;
        }
        
        uint32_t MemoryMap::Entry::baseLow( void ) const
        {
            return this->impl->_base & 0xFFFFFFFF;
        }
        
        uint32_t MemoryMap::Entry::baseHigh( void ) const
        {
            return this->impl->_base >> 32;
        }
        
        uint32_t MemoryMap::Entry::lengthLow( void ) const
        {
            return this->impl->_length & 0xFFFFFFFF;
        }
        
        uint32_t MemoryMap::Entry::lengthHigh( void ) const
        {
            return this->impl->_base >> 32;
        }
        
        void swap( MemoryMap::Entry & o1, MemoryMap::Entry & o2 )
        {
            using std::swap;
            
            swap( o1.impl, o2.impl );
        }
        
        MemoryMap::Entry::IMPL::IMPL( uint64_t base, uint64_t length, Type type ):
            _base(   base ),
            _length( length ),
            _type(   type )
        {}
        
        MemoryMap::Entry::IMPL::IMPL( const IMPL & o ):
            _base(   o._base ),
            _length( o._length ),
            _type(   o._type )
        {}
        
        MemoryMap::Entry::IMPL::~IMPL( void )
        {}
    }
}
