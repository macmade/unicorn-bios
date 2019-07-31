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
        class MemoryMap::IMPL
        {
            public:
                
                IMPL( size_t memory );
                IMPL( const IMPL & o );
                ~IMPL( void );
                
                std::vector< Entry > _entries;
        };

        MemoryMap::MemoryMap( size_t memory ):
            impl( std::make_unique< IMPL >( memory ) )
        {}

        MemoryMap::MemoryMap( const MemoryMap & o ):
            impl( std::make_unique< IMPL >( *( o.impl ) ) )
        {}

        MemoryMap::MemoryMap( MemoryMap && o ) noexcept:
            impl( std::move( o.impl ) )
        {}

        MemoryMap::~MemoryMap( void )
        {}

        MemoryMap & MemoryMap::operator =( MemoryMap o )
        {
            swap( *( this ), o );
            
            return *( this );
        }

        std::vector< MemoryMap::Entry > MemoryMap::entries( void ) const
        {
            return this->impl->_entries;
        }
        
        void swap( MemoryMap & o1, MemoryMap & o2 )
        {
            using std::swap;
            
            swap( o1.impl, o2.impl );
        }

        MemoryMap::IMPL::IMPL( size_t memory )
        {
            uint64_t free;
            uint64_t after;
            
            if( memory < 2 * 1024 * 1024 )
            {
                throw std::runtime_error( "Memory must be at least 2MB" );
            }
            
            free  = memory - 0x00100000 - 0x00010000;
            after = memory - 0x00010000;
            
            this->_entries.push_back( { 0x00000000, 0x0009FC00, Entry::Type::Usable } );
            this->_entries.push_back( { 0x0009FC00, 0x00000400, Entry::Type::Reserved } );
            this->_entries.push_back( { 0x000F0000, 0x00010000, Entry::Type::Reserved } );
            this->_entries.push_back( { 0x00100000, free,       Entry::Type::Usable } );
            this->_entries.push_back( { after,      0x00010000, Entry::Type::ACPI } );
            this->_entries.push_back( { 0xFEC00000, 0x00001000, Entry::Type::Reserved } );
            this->_entries.push_back( { 0xFEE00000, 0x00001000, Entry::Type::Reserved } );
        }

        MemoryMap::IMPL::IMPL( const IMPL & o ):
            _entries( o._entries )
        {}

        MemoryMap::IMPL::~IMPL( void )
        {}
    }
}
