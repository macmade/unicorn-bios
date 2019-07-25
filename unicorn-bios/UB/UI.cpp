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

#include "UB/UI.hpp"
#include "UB/Screen.hpp"
#include "UB/String.hpp"
#include "UB/Casts.hpp"
#include <ncurses.h>
#include <sstream>
#include <mutex>

namespace UB
{
    class UI::IMPL
    {
        public:
            
            IMPL( void );
            IMPL( const IMPL & o );
            IMPL( const IMPL & o, const std::lock_guard< std::recursive_mutex > & l );
            
            void _setup( void );
            void _displayOutput( void );
            void _displayDebug( void );
            void _displayRegisters( void );
            void _displayDisassembly( void );
            void _displayMemory( void );
            
            bool                         _running;
            Screen                       _screen;
            std::stringstream            _output;
            std::stringstream            _debug;
            mutable std::recursive_mutex _rmtx;
    };
    
    UI::UI( void ):
        impl( std::make_unique< IMPL >() )
    {}
    
    UI::UI( const UI & o ):
        impl( std::make_unique< IMPL >( *( o.impl ) ) )
    {}
    
    UI::UI( UI && o ) noexcept
    {
        std::lock_guard< std::recursive_mutex >( o.impl->_rmtx );
        
        this->impl = std::move( o.impl );
    }
    
    UI::~UI( void )
    {}
    
    UI & UI::operator =( UI o )
    {
        swap( *( this ), o );
        
        return *( this );
    }
    
    void UI::run( void )
    {
        {
            std::lock_guard< std::recursive_mutex > l( this->impl->_rmtx );
            
            if( this->impl->_running )
            {
                return;
            }
            
            this->impl->_running = true;
        }
        
        this->impl->_screen.start();
        
        {
            std::lock_guard< std::recursive_mutex > l( this->impl->_rmtx );
            
            this->impl->_running = false;
        }
    }
    
    void UI::output( const std::string & s )
    {
        std::lock_guard< std::recursive_mutex > l( this->impl->_rmtx );
        
        this->impl->_output << s;
    }
    
    void UI::debug( const std::string & s )
    {
        std::lock_guard< std::recursive_mutex > l( this->impl->_rmtx );
        
        this->impl->_debug << s;
    }
    
    void swap( UI & o1, UI & o2 )
    {
        std::lock( o1.impl->_rmtx, o2.impl->_rmtx );
        
        {
            std::lock_guard< std::recursive_mutex > l1( o1.impl->_rmtx, std::adopt_lock );
            std::lock_guard< std::recursive_mutex > l2( o2.impl->_rmtx, std::adopt_lock );
            
            using std::swap;
            
            swap( o1.impl, o2.impl );
        }
    }
    
    UI::IMPL::IMPL( void ):
        _running( false )
    {
        this->_setup();
    }
    
    UI::IMPL::IMPL( const IMPL & o ):
        IMPL( o, std::lock_guard< std::recursive_mutex >( o._rmtx ) )
    {}
    
    UI::IMPL::IMPL( const IMPL & o, const std::lock_guard< std::recursive_mutex > & l ):
        _running( false ),
        _screen(  o._screen ),
        _output(  o._output.str() ),
        _debug(   o._debug.str() )
    {
        ( void )l;
        
        this->_setup();
    }
    
    void UI::IMPL::_setup( void )
    {
        this->_screen.onUpdate
        (
            [ & ]( void )
            {
                this->_displayOutput();
                this->_displayDebug();
                this->_displayRegisters();
                this->_displayDisassembly();
                this->_displayMemory();
            }
        );
        
        this->_screen.onKeyPress
        (
            [ & ]( int key )
            {
                if( key == 'q' )
                {
                    this->_screen.stop();
                }
            }
        );
    }
    
    void UI::IMPL::_displayOutput( void )
    {}
    
    void UI::IMPL::_displayDebug( void )
    {}
    
    void UI::IMPL::_displayRegisters( void )
    {}
    
    void UI::IMPL::_displayDisassembly( void )
    {}
    
    void UI::IMPL::_displayMemory( void )
    {}
}
