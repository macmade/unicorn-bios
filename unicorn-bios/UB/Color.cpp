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

#include "UB/Color.hpp"
#include <ncurses.h>
#include <mutex>

namespace UB
{
    class Color::IMPL
    {
        public:
            
            IMPL( int index );
            IMPL( const IMPL & o );
            ~IMPL( void );
            
            int _index;
    };

    Color Color::clear( void )
    {
        return Color( 1 );
    }
    
    Color Color::black( void )
    {
        return Color( 2 );
    }
    
    Color Color::red( void )
    {
        return Color( 3 );
    }
    
    Color Color::green( void )
    {
        return Color( 4 );
    }
    
    Color Color::yellow( void )
    {
        return Color( 5 );
    }
    
    Color Color::blue( void )
    {
        return Color( 6 );
    }
    
    Color Color::magenta( void )
    {
        return Color( 7 );
    }
    
    Color Color::cyan( void )
    {
        return Color( 8 );
    }
    
    Color Color::white( void )
    {
        return Color( 9 );
    }

    Color::Color( int index ):
        impl( std::make_unique< IMPL >( index ) )
    {}

    Color::Color( const Color & o ):
        impl( std::make_unique< IMPL >( *( o.impl ) ) )
    {}

    Color::Color( Color && o ) noexcept:
        impl( std::move( o.impl ) )
    {}

    Color::~Color( void )
    {}

    Color & Color::operator =( Color o )
    {
        swap( *( this ), o );
        
        return *( this );
    }
    
    int Color::index() const
    {
        return this->impl->_index;
    }

    void swap( Color & o1, Color & o2 )
    {
        using std::swap;
        
        swap( o1.impl, o2.impl );
    }
    
    Color::IMPL::IMPL( int index ):
        _index( index )
    {}

    Color::IMPL::IMPL( const IMPL & o ):
        _index( o._index )
    {}

    Color::IMPL::~IMPL( void )
    {}
}
