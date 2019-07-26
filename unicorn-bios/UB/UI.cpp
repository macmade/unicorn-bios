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
#include "UB/Engine.hpp"
#include "UB/Casts.hpp"
#include "UB/Capstone.hpp"
#include <ncurses.h>
#include <sstream>
#include <mutex>
#include <optional>
#include <thread>
#include <condition_variable>

namespace UB
{
    class UI::IMPL
    {
        public:
            
            IMPL( Engine & engine );
            IMPL( const IMPL & o );
            IMPL( const IMPL & o, const std::lock_guard< std::recursive_mutex > & l );
            
            void _setup( void );
            void _displayStatus( void );
            void _displayOutput( void );
            void _displayDebug( void );
            void _displayRegisters( void );
            void _displayInstructions( void );
            void _displayDisassembly( void );
            void _displayMemory( void );
            void _memoryScrollUp( size_t n = 1 );
            void _memoryScrollDown( size_t n = 1 );
            void _memoryPageUp( void );
            void _memoryPageDown( void );
            
            bool                         _running;
            Screen                       _screen;
            Engine                     & _engine;
            std::stringstream            _output;
            std::stringstream            _debug;
            size_t                       _memoryOffset;
            size_t                       _memoryBytesPerLine;
            size_t                       _memoryLines;
            std::optional< std::string > _memoryAddressPrompt;
            mutable std::recursive_mutex _rmtx;
    };
    
    UI::UI( Engine & engine ):
        impl( std::make_unique< IMPL >( engine ) )
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
        
        {
            std::condition_variable_any cv;
            
            std::thread
            (
                [ & ]
                {
                    this->impl->_screen.start();
                    
                    {
                        std::lock_guard< std::recursive_mutex > l( this->impl->_rmtx );
                        
                        this->impl->_running = false;
                        
                        cv.notify_all();
                    }
                }
            )
            .detach();
            
            {
                std::unique_lock< std::recursive_mutex > l( this->impl->_rmtx );
                
                cv.wait
                (
                    l,
                    [ & ]( void ) -> bool
                    {
                        return this->impl->_running == false;
                    }
                );
            }
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
    
    UI::IMPL::IMPL( Engine & engine ):
        _running(            false ),
        _engine(             engine ),
        _memoryOffset(       0x7C00 ),
        _memoryBytesPerLine( 0 ),
        _memoryLines(        0 )
    {
        this->_setup();
    }
    
    UI::IMPL::IMPL( const IMPL & o ):
        IMPL( o, std::lock_guard< std::recursive_mutex >( o._rmtx ) )
    {}
    
    UI::IMPL::IMPL( const IMPL & o, const std::lock_guard< std::recursive_mutex > & l ):
        _running(            false ),
        _screen(             o._screen ),
        _engine(             o._engine ),
        _output(             o._output.str() ),
        _debug(              o._debug.str() ),
        _memoryOffset(       o._memoryOffset ),
        _memoryBytesPerLine( o._memoryBytesPerLine ),
        _memoryLines(        o._memoryLines )
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
                this->_displayRegisters();
                this->_displayInstructions();
                this->_displayDisassembly();
                this->_displayMemory();
                this->_displayOutput();
                this->_displayDebug();
                this->_displayStatus();
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
                else if( key == 'm' )
                {
                    if( this->_memoryAddressPrompt.has_value() )
                    {
                        this->_memoryAddressPrompt = {};
                    }
                    else
                    {
                        this->_memoryAddressPrompt = "";
                    }
                }
                else if( ( key == 10 || key == 13 ) && this->_memoryAddressPrompt.has_value() )
                {
                    std::string prompt( this->_memoryAddressPrompt.value() );
                    
                    if( prompt.length() > 0 )
                    {
                        this->_memoryOffset = String::fromHex< size_t >( prompt );
                    }
                    
                    this->_memoryAddressPrompt = {};
                }
                else if( key == 127 && this->_memoryAddressPrompt.has_value() )
                {
                    std::string prompt( this->_memoryAddressPrompt.value() );
                    
                    if( prompt.length() > 0 )
                    {
                        this->_memoryAddressPrompt = prompt.substr( 0, prompt.length() - 1 );
                    }
                }
                else
                {
                    if( this->_memoryAddressPrompt.has_value() && key >= 0 && key < 128 && isprint( key ) )
                    {
                        this->_memoryAddressPrompt = this->_memoryAddressPrompt.value() + numeric_cast< char >( key );
                    }
                    else if( key == 'a' )
                    {
                        this->_memoryScrollUp();
                    }
                    else if( key == 's' )
                    {
                        this->_memoryScrollDown();
                    }
                    else if( key == 'd' )
                    {
                        this->_memoryPageUp();
                    }
                    else if( key == 'f' )
                    {
                        this->_memoryPageDown();
                    }
                    else if( key == 'g' )
                    {
                        this->_memoryOffset = 0;
                    }
                }
            }
        );
    }
    
    void UI::IMPL::_displayStatus( void )
    {
        int x(      0 );
        int y(      static_cast< int >( this->_screen.height() ) - 3 );
        int width(  static_cast< int >( this->_screen.width() ) );
        int height( 3 );
        
        {
            ::WINDOW * win( ::newwin( height, width, y, x ) );
            
            {
                ::box( win, 0, 0 );
                ::wmove( win, 1, 2 );
                ::wprintw( win, "Status: running..." );
                ::wmove( win, 2, 1 );
                ::whline( win, 0, width - 2 );
            }
            
            this->_screen.refresh();
            ::wmove( win, 0, 0 );
            ::wrefresh( win );
            ::delwin( win );
        }
    }
    
    void UI::IMPL::_displayOutput( void )
    {
        int x(      0 );
        int y(      15 + ( ( static_cast< int >( this->_screen.height() ) - 15 ) / 2 ) );
        int width(  static_cast< int >( this->_screen.width() ) / 2 );
        int height( ( ( static_cast< int >( this->_screen.height() ) - 15 ) / 2 ) - 2 );
        
        {
            ::WINDOW * win( ::newwin( height, width, y, x ) );
            
            {
                ::box( win, 0, 0 );
                ::wmove( win, 1, 2 );
                ::wprintw( win, "Output:" );
                ::wmove( win, 2, 1 );
                ::whline( win, 0, width - 2 );
                
                y = 3;
            }
            
            {
                std::vector< std::string > lines;
                size_t                     maxLines( numeric_cast< size_t >( height ) - 4 );
                
                {
                    std::lock_guard< std::recursive_mutex > l( this->_rmtx );
                    
                    lines = String::lines( this->_output.str() );
                }
                
                if( lines.size() > maxLines )
                {
                    lines = std::vector< std::string >( lines.end() - numeric_cast< ssize_t >( maxLines ), lines.end() );
                }
                
                for( const auto & s: lines )
                {
                    ::wmove( win, y++, 2 );
                    ::wprintw( win, s.c_str() );
                }
            }
            
            this->_screen.refresh();
            ::wmove( win, 0, 0 );
            ::wrefresh( win );
            ::delwin( win );
        }
    }
    
    void UI::IMPL::_displayDebug( void )
    {
        int x(      static_cast< int >( this->_screen.width() ) / 2 );
        int y(      15 + ( ( static_cast< int >( this->_screen.height() ) - 15 ) / 2 ) );
        int width(  static_cast< int >( this->_screen.width() ) / 2 );
        int height( ( ( static_cast< int >( this->_screen.height() ) - 15 ) / 2 ) - 2 );
        
        {
            ::WINDOW * win( ::newwin( height, width, y, x ) );
            
            {
                ::box( win, 0, 0 );
                ::wmove( win, 1, 2 );
                ::wprintw( win, "Debug:" );
                ::wmove( win, 2, 1 );
                ::whline( win, 0, width - 2 );
                
                y = 3;
            }
            
            {
                std::vector< std::string > lines;
                size_t                     maxLines( numeric_cast< size_t >( height ) - 4 );
                
                {
                    std::lock_guard< std::recursive_mutex > l( this->_rmtx );
                    
                    lines = String::lines( this->_debug.str() );
                }
                
                if( lines.size() > maxLines )
                {
                    lines = std::vector< std::string >( lines.end() - numeric_cast< ssize_t >( maxLines ), lines.end() );
                }
                
                for( const auto & s: lines )
                {
                    ::wmove( win, y++, 2 );
                    ::wprintw( win, s.c_str() );
                }
            }
            
            this->_screen.refresh();
            ::wmove( win, 0, 0 );
            ::wrefresh( win );
            ::delwin( win );
        }
    }
    
    void UI::IMPL::_displayRegisters( void )
    {
        int x(      0 );
        int y(      0 );
        int width(  36 );
        int height( 15 );
        
        {
            ::WINDOW * win( ::newwin( height, width, y, x ) );
            
            {
                ::box( win, 0, 0 );
                ::wmove( win, 1, 2 );
                ::wprintw( win, "CPU Registers:" );
                ::wmove( win, 2, 1 );
                ::whline( win, 0, width - 2 );
                
                y = 3;
            }
            
            {
                std::string ah( String::toHex( this->_engine.ah() ) );
                std::string al( String::toHex( this->_engine.al() ) );
                std::string bh( String::toHex( this->_engine.bh() ) );
                std::string bl( String::toHex( this->_engine.bl() ) );
                std::string ch( String::toHex( this->_engine.ch() ) );
                std::string cl( String::toHex( this->_engine.cl() ) );
                std::string dh( String::toHex( this->_engine.dh() ) );
                std::string dl( String::toHex( this->_engine.dl() ) );
                std::string ax( String::toHex( this->_engine.ax() ) );
                std::string bx( String::toHex( this->_engine.bx() ) );
                std::string cx( String::toHex( this->_engine.cx() ) );
                std::string dx( String::toHex( this->_engine.dx() ) );
                std::string si( String::toHex( this->_engine.si() ) );
                std::string di( String::toHex( this->_engine.di() ) );
                std::string sp( String::toHex( this->_engine.sp() ) );
                std::string bp( String::toHex( this->_engine.bp() ) );
                std::string cs( String::toHex( this->_engine.cs() ) );
                std::string ds( String::toHex( this->_engine.ds() ) );
                std::string es( String::toHex( this->_engine.es() ) );
                std::string ss( String::toHex( this->_engine.ss() ) );
                std::string ip( String::toHex( this->_engine.ip() ) );
                std::string fl( String::toHex( this->_engine.eflags() ) );
                
                ::wmove( win, y++, 2 );
                ::wprintw( win, ( "AX: " + ax + " | AH: " + ah + " | AL: " + al ).c_str() );
                ::wmove( win, y++, 2 );
                ::wprintw( win, ( "BX: " + bx + " | BH: " + bh + " | BL: " + bl ).c_str() );
                ::wmove( win, y++, 2 );
                ::wprintw( win, ( "CX: " + cx + " | CH: " + ch + " | CL: " + cl ).c_str() );
                ::wmove( win, y++, 2 );
                ::wprintw( win, ( "DX: " + dx + " | DH: " + dh + " | DL: " + dl ).c_str() );
                ::wmove( win, y++, 1 );
                ::whline( win, 0, width - 2 );
                ::wmove( win, y++, 2 );
                ::wprintw( win, ( "SI: " + si + " | DI: " + di ).c_str() );
                ::wmove( win, y++, 2 );
                ::wprintw( win, ( "SP: " + sp + " | BP: " + bp ).c_str() );
                ::wmove( win, y++, 2 );
                ::wprintw( win, ( "CS: " + cs + " | DS: " + ds ).c_str() );
                ::wmove( win, y++, 2 );
                ::wprintw( win, ( "ES: " + es + " | SS: " + ss ).c_str() );
                ::wmove( win, y++, 1 );
                ::whline( win, 0, width - 2 );
                ::wmove( win, y++, 2 );
                ::wprintw( win, ( "IP: " + ip + " | Flags: " + fl ).c_str() );
            }
            
            this->_screen.refresh();
            ::wmove( win, 0, 0 );
            ::wrefresh( win );
            ::delwin( win );
        }
    }
    
    void UI::IMPL::_displayInstructions( void )
    {
        int x(      36 );
        int y(      0 );
        int width(  56 );
        int height( 15 );
        
        {
            ::WINDOW * win( ::newwin( height, width, y, x ) );
            
            {
                ::box( win, 0, 0 );
                ::wmove( win, 1, 2 );
                ::wprintw( win, "Instructions:" );
                ::wmove( win, 2, 1 );
                ::whline( win, 0, width - 2 );
                
                y = 3;
            }
            
            try
            {
                uint16_t                   ip( this->_engine.ip() );
                std::vector< uint8_t >     bytes( this->_engine.read( ip, 512 ) );
                std::vector< std::string > instructions( Capstone::instructions( bytes, ip ) );
                
                for( const auto & s: instructions )
                {
                    if( y == height - 1 )
                    {
                        break;
                    }
                    
                    ::wmove( win, y++, 2 );
                    ::wprintw( win, s.c_str() );
                }
            }
            catch( ... )
            {}
            
            this->_screen.refresh();
            ::wmove( win, 0, 0 );
            ::wrefresh( win );
            ::delwin( win );
        }
    }
    
    void UI::IMPL::_displayDisassembly( void )
    {
        int x(      36 + 56 );
        int y(      0 );
        int width(  static_cast< int >( this->_screen.width() ) - x );
        int height( 15 );
        
        {
            ::WINDOW * win( ::newwin( height, width, y, x ) );
            
            {
                ::box( win, 0, 0 );
                ::wmove( win, 1, 2 );
                ::wprintw( win, "Disassembly:" );
                ::wmove( win, 2, 1 );
                ::whline( win, 0, width - 2 );
                
                y = 3;
            }
            
            try
            {
                uint16_t                   ip( this->_engine.ip() );
                std::vector< uint8_t >     bytes( this->_engine.read( ip, 512 ) );
                std::vector< std::string > instructions( Capstone::disassemble( bytes, ip ) );
                
                for( const auto & s: instructions )
                {
                    if( y == height - 1 )
                    {
                        break;
                    }
                    
                    ::wmove( win, y++, 2 );
                    ::wprintw( win, s.c_str() );
                }
            }
            catch( ... )
            {}
            
            this->_screen.refresh();
            ::wmove( win, 0, 0 );
            ::wrefresh( win );
            ::delwin( win );
        }
    }
    
    void UI::IMPL::_displayMemory( void )
    {
        int x(      0 );
        int y(      15 );
        int width(  static_cast< int >( this->_screen.width() ) );
        int height( ( static_cast< int >( this->_screen.height() ) - y ) / 2 );
        
        {
            ::WINDOW * win( ::newwin( height, width, y, x ) );
            
            {
                ::box( win, 0, 0 );
                ::wmove( win, 1, 2 );
                ::wprintw( win, "Memory:" );
                ::wmove( win, 2, 1 );
                ::whline( win, 0, width - 2 );
                
                y = 3;
            }
            
            if( this->_memoryAddressPrompt.has_value() )
            {
                ::wmove( win, 3, 2 );
                ::wprintw( win, "Enter a memory address:" );
                ::wmove( win, 4, 2 );
                ::wprintw( win, this->_memoryAddressPrompt.value().c_str() );
            }
            else
            {
                size_t cols(  this->_screen.width()  - 4 );
                size_t lines( numeric_cast< size_t >( height ) - 4 );
                
                this->_memoryBytesPerLine = ( cols / 4 ) - 5;
                this->_memoryLines        = lines;
                
                {
                    size_t                 size(   this->_memoryBytesPerLine * lines );
                    size_t                 offset( this->_memoryOffset );
                    std::vector< uint8_t > mem(    this->_engine.read( offset, size ) );
                    
                    for( size_t i = 0; i < mem.size(); i++ )
                    {
                        if( i % this->_memoryBytesPerLine == 0 )
                        {
                            ::wmove( win, y++, 2 );
                            ::wprintw( win, "%016X: ", offset );
                            
                            offset += this->_memoryBytesPerLine;
                        }
                        
                        ::wprintw( win, "%02X ", numeric_cast< int >( mem[ i ] ) );
                    }
                    
                    y = 3;
                    
                    ::wmove( win, y, numeric_cast< int >( this->_memoryBytesPerLine * 3 ) + 4 + 16 );
                    ::wvline( win, 0, numeric_cast< int >( lines ) );
                    
                    for( size_t i = 0; i < mem.size(); i++ )
                    {
                        char c = static_cast< char >( mem[ i ] );
                        
                        if( i % this->_memoryBytesPerLine == 0 )
                        {
                            ::wmove( win, y++, numeric_cast< int >( this->_memoryBytesPerLine * 3 ) + 4 + 18 );
                        }
                        
                        if( isprint( c ) == false || isspace( c ) )
                        {
                            c = '.';
                        }
                        
                        ::wprintw( win, "%c", c );
                    }
                }
            }
            
            this->_screen.refresh();
            ::wmove( win, 0, 0 );
            ::wrefresh( win );
            ::delwin( win );
        }
    }
    
    void UI::IMPL::_memoryScrollUp( size_t n )
    {
        if( this->_memoryOffset > ( this->_memoryBytesPerLine * n ) )
        {
            this->_memoryOffset -= ( this->_memoryBytesPerLine * n );
        }
        else
        {
            this->_memoryOffset = 0;
        }
    }
    
    void UI::IMPL::_memoryScrollDown( size_t n )
    {
        if( ( this->_memoryOffset + ( this->_memoryBytesPerLine * n ) ) < this->_engine.memory() )
        {
            this->_memoryOffset += ( this->_memoryBytesPerLine * n );
        }
    }
    
    void UI::IMPL::_memoryPageUp( void )
    {
        this->_memoryScrollUp( this->_memoryLines );
    }
    
    void UI::IMPL::_memoryPageDown( void )
    {
        this->_memoryScrollDown( this->_memoryLines );
    }
}
