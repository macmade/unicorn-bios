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

#include "UB/FAT/DAP.hpp"
#include "UB/BinaryStream.hpp"

namespace UB
{
    namespace FAT
    {
        class DAP::IMPL
        {
            public:
                
                IMPL( void );
                IMPL( const IMPL & o );
                IMPL( BinaryStream & stream );
                ~IMPL( void );
                
                uint8_t  _size;
                uint8_t  _zero;
                uint16_t _numberOfSectors;
                uint16_t _destinationOffset;
                uint16_t _destinationSegment;
                uint64_t _logicalBlockAddress;
        };
        
        size_t DAP::DataSize( void )
        {
            return 16;
        }
        
        DAP::DAP( void ):
            impl( std::make_unique< IMPL >() )
        {}
        
        DAP::DAP( const DAP & o ):
            impl( std::make_unique< IMPL >( *( o.impl ) ) )
        {}
        
        DAP::DAP( BinaryStream & stream ):
            impl( std::make_unique< IMPL >( stream ) )
        {}
        
        DAP::DAP( DAP && o ) noexcept:
            impl( std::move( o.impl ) )
        {}
        
        DAP::~DAP( void )
        {}
        
        DAP & DAP::operator =( DAP o )
        {
            swap( *( this ), o );
            
            return *( this );
        }
        
        uint8_t DAP::size( void ) const
        {
            return this->impl->_size;
        }
        
        uint8_t DAP::zero( void ) const
        {
            return this->impl->_zero;
        }
        
        uint16_t DAP::numberOfSectors( void ) const
        {
            return this->impl->_numberOfSectors;
        }
        
        uint16_t DAP::destinationOffset( void ) const
        {
            return this->impl->_destinationOffset;
        }
        
        uint16_t DAP::destinationSegment( void ) const
        {
            return this->impl->_destinationSegment;
        }
        
        uint64_t DAP::logicalBlockAddress( void ) const
        {
            return this->impl->_logicalBlockAddress;
        }
        
        void swap( DAP & o1, DAP & o2 )
        {
            using std::swap;
            
            swap( o1.impl, o2.impl );
        }
        
        DAP::IMPL::IMPL( void ):
            _size(                0 ),
            _zero(                0 ),
            _numberOfSectors(     0 ),
            _destinationOffset(   0 ),
            _destinationSegment(  0 ),
            _logicalBlockAddress( 0 )
        {}
        
        DAP::IMPL::IMPL( BinaryStream & stream ):
            _size(                stream.readUInt8() ),
            _zero(                stream.readUInt8() ),
            _numberOfSectors(     stream.readLittleEndianUInt16() ),
            _destinationOffset(   stream.readLittleEndianUInt16() ),
            _destinationSegment(  stream.readLittleEndianUInt16() ),
            _logicalBlockAddress( stream.readLittleEndianUInt16() )
        {}
        
        DAP::IMPL::IMPL( const IMPL & o ):
            _size(                o._size ),
            _zero(                o._zero ),
            _numberOfSectors(     o._numberOfSectors ),
            _destinationOffset(   o._destinationOffset ),
            _destinationSegment(  o._destinationSegment ),
            _logicalBlockAddress( o._logicalBlockAddress )
        {}
        
        DAP::IMPL::~IMPL( void )
        {}
    }
}
