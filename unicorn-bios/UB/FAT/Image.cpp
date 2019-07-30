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

#include "UB/FAT/Image.hpp"
#include "UB/FAT/Functions.hpp"
#include "UB/BinaryFileStream.hpp"

namespace UB
{
    namespace FAT
    {
        class Image::IMPL
        {
            public:
                
                IMPL( const std::string & path );
                IMPL( const IMPL & o );
                
                std::string _path;
                MBR         _mbr;
        };
        
        Image::Image( const std::string & path ):
            impl( std::make_unique< IMPL >( path ) )
        {}
        
        Image::Image( const Image & o ):
            impl( std::make_unique< IMPL >( *( o.impl ) ) )
        {}
        
        Image::Image( Image && o ) noexcept:
            impl( std::move( o.impl ) )
        {}
        
        Image::~Image( void )
        {}
        
        Image & Image::operator =( Image o )
        {
            swap( *( this ), o );
            
            return *( this );
        }
        
        std::string Image::path( void ) const
        {
            return this->impl->_path;
        }
        
        MBR Image::mbr( void ) const
        {
            return this->impl->_mbr;
        }
        
        std::vector< uint8_t > Image::read( uint8_t cylinder, uint8_t head, uint8_t sector, uint8_t sectors )
        {
            uint64_t lba( chsToLBA( this->impl->_mbr, cylinder, sector, head ) );
            
            return this->read( lba * this->impl->_mbr.bytesPerSector(), sectors * this->impl->_mbr.bytesPerSector() );
        }
        
        std::vector< uint8_t > Image::read( uint64_t offset, uint64_t size )
        {
            ( void )offset;
            ( void )size;
            
            return {};
        }
        
        void swap( Image & o1, Image & o2 )
        {
            using std::swap;
            
            swap( o1.impl, o2.impl );
        }
        
        Image::IMPL::IMPL( const std::string & path ):
            _path( path )
        {
            BinaryFileStream stream( path );
            
            this->_mbr = MBR( stream );
        }
        
        Image::IMPL::IMPL( const IMPL & o ):
            _path( o._path ),
            _mbr(  o._mbr )
        {}
    }
}
