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

#include "UB/Window.hpp"
#include "UB/Casts.hpp"
#include <ncurses.h>

namespace UB
{
    class Window::IMPL
    {
        public:
            
            IMPL( size_t x, size_t y, size_t width, size_t height );
            IMPL( const IMPL & o );
            ~IMPL( void );
            
            size_t     _x;
            size_t     _y;
            size_t     _width;
            size_t     _height;
            ::WINDOW * _win;
    };

    Window::Window( size_t x, size_t y, size_t width, size_t height ):
        impl( std::make_unique< IMPL >( x, y, width, height ) )
    {}

    Window::Window( const Window & o ):
        impl( std::make_unique< IMPL >( *( o.impl ) ) )
    {}

    Window::Window( Window && o ) noexcept:
        impl( std::move( o.impl ) )
    {}

    Window::~Window( void )
    {}

    Window & Window::operator =( Window o )
    {
        swap( *( this ), o );
        
        return *( this );
    }
    
    void Window::refresh( void )
    {
        ::wrefresh( this->impl->_win );
    }
    
    void Window::move( size_t x, size_t y )
    {
        ::wmove( this->impl->_win, numeric_cast< int >( y ), numeric_cast< int >( x ) );
    }
    
    void Window::print( const std::string & s )
    {
        ::wprintw( this->impl->_win, s.c_str() );
    }
    
    void Window::print( const char * format, ... )
    {
        va_list ap;
        
        va_start( ap, format );
        ::vw_printw( this->impl->_win, format, ap );
        va_end( ap );
    }
    
    void Window::box( void )
    {
        ::box( this->impl->_win, 0, 0 );
    }
    
    void Window::addHorizontalLine( size_t width )
    {
        ::whline( this->impl->_win, 0, numeric_cast< int >( width ) );
    }
    
    void Window::addVerticalLine( size_t height )
    {
        ::wvline( this->impl->_win, 0, numeric_cast< int >( height ) );
    }

    void swap( Window & o1, Window & o2 )
    {
        using std::swap;
        
        swap( o1.impl, o2.impl );
    }

    Window::IMPL::IMPL( size_t x, size_t y, size_t width, size_t height ):
        _x(      x ),
        _y(      y ),
        _width(  width ),
        _height( height ),
        _win(    ::newwin( numeric_cast< int >( height ), numeric_cast< int >( width ), numeric_cast< int >( y ), numeric_cast< int >( x ) ) )
    {}

    Window::IMPL::IMPL( const IMPL & o ):
        _x(      o._x ),
        _y(      o._y ),
        _width(  o._width ),
        _height( o._height ),
        _win(    ::newwin( numeric_cast< int >( o._height ), numeric_cast< int >( o._width ), numeric_cast< int >( o._y ), numeric_cast< int >( o._x ) ) )
    {}

    Window::IMPL::~IMPL( void )
    {
        ::delwin( this->_win );
    }
}
