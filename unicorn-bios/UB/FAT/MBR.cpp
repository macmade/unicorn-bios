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

#include "UB/FAT/MBR.hpp"

namespace UB
{
    namespace FAT
    {
        class MBR::IMPL
        {
            public:
                
                IMPL( void );
                IMPL( const IMPL & o );
        };
        
        MBR::MBR( void ):
            impl( std::make_unique< IMPL >() )
        {}
        
        MBR::MBR( const MBR & o ):
            impl( std::make_unique< IMPL >( *( o.impl ) ) )
        {}
        
        MBR::MBR( MBR && o ):
            impl( std::move( o.impl ) )
        {}
        
        MBR::~MBR( void )
        {}
        
        MBR & MBR::operator =( MBR o )
        {
            swap( *( this ), o );
            
            return *( this );
        }
        
        void swap( MBR & o1, MBR & o2 )
        {
            using std::swap;
            
            swap( o1.impl, o2.impl );
        }
        
        MBR::IMPL::IMPL( void )
        {}
        
        MBR::IMPL::IMPL( const IMPL & o )
        {
            ( void )o;
        }
    }
}
