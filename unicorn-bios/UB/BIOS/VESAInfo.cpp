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

#include "UB/BIOS/VESAInfo.hpp"

namespace UB
{
    namespace BIOS
    {
        class VESAInfo::IMPL
        {
            public:
                
                IMPL( void );
                IMPL( const IMPL & o );
                ~IMPL( void );
                
                std::array< uint8_t, 4 > _signature;
                uint16_t                 _version;
        };

        VESAInfo::VESAInfo( void ):
            impl( std::make_unique< IMPL >() )
        {}

        VESAInfo::VESAInfo( const VESAInfo & o ):
            impl( std::make_unique< IMPL >( *( o.impl ) ) )
        {}

        VESAInfo::VESAInfo( VESAInfo && o ) noexcept:
            impl( std::move( o.impl ) )
        {}

        VESAInfo::~VESAInfo( void )
        {}

        VESAInfo & VESAInfo::operator =( VESAInfo o )
        {
            swap( *( this ), o );
            
            return *( this );
        }
        
        std::vector< uint8_t > VESAInfo::data( void ) const
        {
            return {};
        }
        
        std::array< uint8_t, 4 > VESAInfo::signature( void ) const
        {
            return this->impl->_signature;
        }
        
        uint16_t VESAInfo::version( void ) const
        {
            return this->impl->_version;
        }
        
        void swap( VESAInfo & o1, VESAInfo & o2 )
        {
            using std::swap;
            
            swap( o1.impl, o2.impl );
        }

        VESAInfo::IMPL::IMPL( void ):
            _signature( { 'V', 'E', 'S', 'A' } ),
            _version(   0x0200 )
        {}

        VESAInfo::IMPL::IMPL( const IMPL & o ):
            _signature( o._signature ),
            _version( o._version )
        {}

        VESAInfo::IMPL::~IMPL( void )
        {}
    }
}
