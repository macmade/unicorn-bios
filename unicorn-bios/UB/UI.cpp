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
#include "UB/Window.hpp"
#include <mutex>
#include <optional>
#include <thread>
#include <functional>
#include <optional>
#include <iostream>
#include <condition_variable>

namespace UB
{
    class UI::IMPL
    {
        public:
            
            IMPL( Engine & engine );
            IMPL( const IMPL & o );
            IMPL( const IMPL & o, const std::lock_guard< std::recursive_mutex > & l );
            
            void _setupEngine( void );
            void _setupScreen( void );
            void _displayStatus( void );
            void _displayOutput( void );
            void _displayDebug( void );
            void _displayRegisters( void );
            void _displayFlags( void );
            void _displayStack( void );
            void _displayInstructions( void );
            void _displayDisassembly( void );
            void _displayMemory( void );
            void _memoryScrollUp( size_t n = 1 );
            void _memoryScrollDown( size_t n = 1 );
            void _memoryPageUp( void );
            void _memoryPageDown( void );
            
            bool                          _running;
            Mode                          _mode;
            std::optional< Screen >       _screen;
            Engine                      & _engine;
            StringStream                  _output;
            StringStream                  _debug;
            std::string                   _status;
            size_t                        _memoryOffset;
            size_t                        _memoryBytesPerLine;
            size_t                        _memoryLines;
            std::optional< std::string >  _memoryAddressPrompt;
            std::function< void( int ) >  _waitEnterOrSpaceKeyPress;
            mutable std::recursive_mutex  _rmtx;
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
    
    UI::Mode UI::mode( void ) const
    {
        std::lock_guard< std::recursive_mutex >( this->impl->_rmtx );
        
        return this->impl->_mode;
    }
    
    void UI::mode( Mode mode )
    {
        std::lock_guard< std::recursive_mutex >( this->impl->_rmtx );
        
        if( this->impl->_running )
        {
            throw std::runtime_error( "Cannot change the UI mode while UI is running" );
        }
        
        this->impl->_mode = mode;
    }
    
    void UI::run( void )
    {
        Mode mode;
        
        {
            std::lock_guard< std::recursive_mutex > l( this->impl->_rmtx );
            
            if( this->impl->_running )
            {
                return;
            }
            
            this->impl->_running = true;
            mode                 = this->impl->_mode;
            
            this->impl->_output = {};
            this->impl->_debug  = {};
            
            if( mode == Mode::Interactive )
            {
                this->impl->_setupScreen();
            }
            else
            {
                this->impl->_output.redirect( std::cout );
                this->impl->_debug.redirect(  std::cerr );
            }
        }
        
        {
            std::condition_variable_any cv;
            
            std::thread
            (
                [ & ]
                {
                    if( mode == Mode::Interactive )
                    {
                        this->impl->_screen->start();
                    }
                    
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
    
    int UI::waitForUserResume( void )
    {
        bool                        keyPressed( false );
        std::condition_variable_any cv;
        
        if( this->mode() == Mode::Standard )
        {
            std::cout << "Emulation paused - Press [ENTER] to continue..." << std::endl;
            
            return getchar();
        }
        else
        {
            int pressed( 0 );
            
            {
                std::lock_guard< std::recursive_mutex > l( this->impl->_rmtx );
                std::string                             s;
                
                this->impl->_status                   = "Emulation paused - Press [ENTER] or [SPACE] to continue...";
                this->impl->_waitEnterOrSpaceKeyPress =
                [ & ]( int key )
                {
                    std::lock_guard< std::recursive_mutex > l( this->impl->_rmtx );
                    
                    pressed    = key;
                    keyPressed = true;
                    
                    this->impl->_status = ( this->impl->_engine.running() ) ? "Emulation running..." : "Emulation stopped";
                    
                    cv.notify_all();
                };
            }
            
            {
                std::unique_lock< std::recursive_mutex > l( this->impl->_rmtx );
                
                cv.wait
                (
                    l,
                    [ & ]( void ) -> bool
                    {
                        return keyPressed;
                    }
                );
                
                return pressed;
            }
        }
    }
    
    StringStream & UI::output( void )
    {
        std::lock_guard< std::recursive_mutex > l( this->impl->_rmtx );
        
        return this->impl->_output;
    }
    
    StringStream & UI::debug( void )
    {
        std::lock_guard< std::recursive_mutex > l( this->impl->_rmtx );
        
        return this->impl->_debug;
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
        _mode(               Mode::Interactive ),
        _engine(             engine ),
        _status(             "Emulation not running" ),
        _memoryOffset(       0x7C00 ),
        _memoryBytesPerLine( 0 ),
        _memoryLines(        0 )
    {
        this->_setupEngine();
    }
    
    UI::IMPL::IMPL( const IMPL & o ):
        IMPL( o, std::lock_guard< std::recursive_mutex >( o._rmtx ) )
    {}
    
    UI::IMPL::IMPL( const IMPL & o, const std::lock_guard< std::recursive_mutex > & l ):
        _running(            false ),
        _mode(               o._mode ),
        _screen(             o._screen ),
        _engine(             o._engine ),
        _output(             o._output.string() ),
        _debug(              o._debug.string() ),
        _status(             "Emulation not running" ),
        _memoryOffset(       o._memoryOffset ),
        _memoryBytesPerLine( o._memoryBytesPerLine ),
        _memoryLines(        o._memoryLines )
    {
        ( void )l;
        
        this->_setupEngine();
    }
    
    void UI::IMPL::_setupEngine( void )
    {
        this->_engine.onStart
        (
            [ & ]
            {
                std::lock_guard< std::recursive_mutex > l( this->_rmtx );
                
                this->_status = "Emulation running...";
            }
        );
        
        this->_engine.onStop
        (
            [ & ]
            {
                std::lock_guard< std::recursive_mutex > l( this->_rmtx );
                
                this->_status = "Emulation stopped";
            }
        );
    }
    
    void UI::IMPL::_setupScreen( void )
    {
        this->_screen = Screen();
        
        this->_screen->onUpdate
        (
            [ & ]( void )
            {
                if( this->_screen->width() < 50 || this->_screen->height() < 30 )
                {
                    this->_screen->clear();
                    this->_screen->print( "Screen too small..." );
                    
                    return;
                }
                
                this->_displayRegisters();
                this->_displayFlags();
                this->_displayStack();
                this->_displayInstructions();
                this->_displayDisassembly();
                this->_displayMemory();
                this->_displayOutput();
                this->_displayDebug();
                this->_displayStatus();
            }
        );
        
        this->_screen->onKeyPress
        (
            [ & ]( int key )
            {
                if( key == 'q' )
                {
                    this->_screen->stop();
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
                else if( key == 10 || key == 13 || key == 0x20 )
                {
                    std::lock_guard< std::recursive_mutex > l( this->_rmtx );
                    
                    if( this->_waitEnterOrSpaceKeyPress != nullptr )
                    {
                        this->_waitEnterOrSpaceKeyPress( key );
                    }
                    
                    this->_waitEnterOrSpaceKeyPress = {};
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
        size_t x(      0 );
        size_t y(      this->_screen->height() - 3 );
        size_t width(  this->_screen->width() );
        size_t height( 3 );
        Window win( x, y, width, height );
        
        {
            std::lock_guard< std::recursive_mutex > l( this->_rmtx );
            
            win.box();
            win.move( 2, 1 );
            win.print( this->_status );
        }
        
        this->_screen->refresh();
        win.move( 0, 0 );
        win.refresh();
    }
    
    void UI::IMPL::_displayOutput( void )
    {
        size_t x(      0 );
        size_t y(      21 + ( ( this->_screen->height() - 21 ) / 2 ) );
        size_t width(  this->_screen->width() / 2 );
        size_t height( ( ( this->_screen->height() - 21 ) / 2 ) - 2 );
        Window win( x, y, width, height );
        
        win.box();
        win.move( 2, 1 );
        win.print( "Output:" );
        win.move( 1, 2 );
        win.addHorizontalLine( width - 2 );
        
        y = 3;
        
        {
            std::vector< std::string > lines;
            std::vector< std::string > display;
            size_t                     maxLines( numeric_cast< size_t >( height ) - 4 );
            size_t                     max( 80 );
            
            {
                std::lock_guard< std::recursive_mutex > l( this->_rmtx );
                
                lines = String::lines( this->_output.string() );
            }
            
            if( numeric_cast< size_t >( width - 4 ) < max )
            {
                max = numeric_cast< size_t >( width - 4 );
            }
            
            for( std::string s: lines )
            {
                while( s.length() > max )
                {
                    display.push_back( s.substr( 0, max ) );
                    
                    s = s.substr( max );
                }
                
                display.push_back( s );
            }
            
            if( display.size() > maxLines )
            {
                display = std::vector< std::string >( display.end() - numeric_cast< ssize_t >( maxLines ), display.end() );
            }
            
            for( const auto & s: display )
            {
                win.move( 2, y++ );
                win.print( s );
            }
        }
        
        this->_screen->refresh();
        win.move( 0, 0 );
        win.refresh();
    }
    
    void UI::IMPL::_displayDebug( void )
    {
        size_t x(      this->_screen->width() / 2 );
        size_t y(      21 + ( ( this->_screen->height() - 21 ) / 2 ) );
        size_t width(  this->_screen->width() / 2 );
        size_t height( ( ( this->_screen->height() - 21 ) / 2 ) - 2 );
        Window win( x, y, width, height );
        
        win.box();
        win.move( 2, 1 );
        win.print( "Debug:" );
        win.move( 1, 2 );
        win.addHorizontalLine( width - 2 );
        
        y = 3;
        
        {
            std::vector< std::string > lines;
            size_t                     maxLines( numeric_cast< size_t >( height ) - 4 );
            
            {
                std::lock_guard< std::recursive_mutex > l( this->_rmtx );
                
                lines = String::lines( this->_debug.string() );
            }
            
            if( lines.size() > maxLines )
            {
                lines = std::vector< std::string >( lines.end() - numeric_cast< ssize_t >( maxLines ), lines.end() );
            }
            
            for( const auto & s: lines )
            {
                win.move( 2, y++ );
                win.print( s );
            }
        }
        
        this->_screen->refresh();
        win.move( 0, 0 );
        win.refresh();
    }
    
    void UI::IMPL::_displayRegisters( void )
    {
        size_t x(      0 );
        size_t y(      0 );
        size_t width(  54 );
        size_t height( 21 );
        Window win( x, y, width, height );
        
        if( this->_screen->width() < x + width )
        {
            return;
        }
        
        win.box();
        win.move( 2, 1 );
        win.print( "CPU Registers:" );
        win.move( 1, 2 );
        win.addHorizontalLine( width - 2 );
        
        y = 3;
        
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
            std::string ss( String::toHex( this->_engine.ss() ) );
            std::string es( String::toHex( this->_engine.es() ) );
            std::string fs( String::toHex( this->_engine.fs() ) );
            std::string gs( String::toHex( this->_engine.gs() ) );
            std::string ip( String::toHex( this->_engine.ip() ) );
            std::string eax( String::toHex( this->_engine.eax() ) );
            std::string ebx( String::toHex( this->_engine.ebx() ) );
            std::string ecx( String::toHex( this->_engine.ecx() ) );
            std::string edx( String::toHex( this->_engine.edx() ) );
            std::string esi( String::toHex( this->_engine.esi() ) );
            std::string edi( String::toHex( this->_engine.edi() ) );
            std::string esp( String::toHex( this->_engine.esp() ) );
            std::string ebp( String::toHex( this->_engine.ebp() ) );
            std::string eip( String::toHex( this->_engine.eip() ) );
            uint32_t    eflags32( this->_engine.eflags() );
            std::string eflags( String::toHex( eflags32 ) );
            
            win.move( 2, y++ );
            win.print( "EAX: " + eax + " | AX: " + ax + " | AH: " + ah + " | AL: " + al );
            win.move( 2, y++ );
            win.print( "EBX: " + ebx + " | BX: " + bx + " | BH: " + bh + " | BL: " + bl );
            win.move( 2, y++ );
            win.print( "ECX: " + ecx + " | CX: " + cx + " | CH: " + ch + " | CL: " + cl );
            win.move( 2, y++ );
            win.print( "EDX: " + edx + " | DX: " + dx + " | DH: " + dh + " | DL: " + dl );
            win.move( 1, y++ );
            win.addHorizontalLine( width - 2 );
            win.move( 2, y++ );
            win.print( "ESI: " + esi + " | SI: " + si );
            win.move( 2, y++ );
            win.print( "EDI: " + edi + " | DI: " + di );
            win.move( 1, y++ );
            win.addHorizontalLine( width - 2 );
            win.move( 2, y++ );
            win.print( "EBP: " + ebp + " | BP: " + bp );
            win.move( 2, y++ );
            win.print( "ESP: " + esp + " | SP: " + sp );
            win.move( 1, y++ );
            win.addHorizontalLine( width - 2 );
            win.move( 2, y++ );
            win.print( "CS: " + cs + " | DS: " + ds + " | SS: " + ss );
            win.move( 2, y++ );
            win.print( "ES: " + es + " | FS: " + fs + " | GS: " + gs );
            win.move( 1, y++ );
            win.addHorizontalLine( width - 2 );
            win.move( 2, y++ );
            win.print( "EIP: " + eip + " | IP: " + ip );
            win.move( 1, y++ );
            win.addHorizontalLine( width - 2 );
            win.move( 2, y++ );
            win.print( "EFLAGS: " + eflags );
            win.move( 1, y++ );
        }
        
        this->_screen->refresh();
        win.move( 0, 0 );
        win.refresh();
    }
    
    void UI::IMPL::_displayFlags( void )
    {
        size_t x(      54 );
        size_t y(      0 );
        size_t width(  36 );
        size_t height( 21 );
        Window win( x, y, width, height );
        
        if( this->_screen->width() < x + width )
        {
            return;
        }
        
        win.box();
        win.move( 2, 1 );
        win.print( "CPU Flags:" );
        win.move( 1, 2 );
        win.addHorizontalLine( width - 2 );
        
        y = 3;
        
        {
            uint32_t                                      eflags( this->_engine.eflags() );
            std::vector< std::pair< std::string, bool > > flags;
            
            flags.push_back( { "Carry:                       ", ( eflags & ( 1 <<  0 ) ) != 0 } );
            flags.push_back( { "Parity:                      ", ( eflags & ( 1 <<  2 ) ) != 0 } );
            flags.push_back( { "Adjust:                      ", ( eflags & ( 1 <<  4 ) ) != 0 } );
            flags.push_back( { "Zero:                        ", ( eflags & ( 1 <<  6 ) ) != 0 } );
            flags.push_back( { "Sign:                        ", ( eflags & ( 1 <<  7 ) ) != 0 } );
            flags.push_back( { "Trap:                        ", ( eflags & ( 1 <<  8 ) ) != 0 } );
            flags.push_back( { "Interrupt enable:            ", ( eflags & ( 1 <<  9 ) ) != 0 } );
            flags.push_back( { "Direction:                   ", ( eflags & ( 1 << 10 ) ) != 0 } );
            flags.push_back( { "Overflow:                    ", ( eflags & ( 1 << 11 ) ) != 0 } );
            flags.push_back( { "Resume:                      ", ( eflags & ( 1 << 16 ) ) != 0 } );
            flags.push_back( { "Virtual 8086:                ", ( eflags & ( 1 << 17 ) ) != 0 } );
            flags.push_back( { "Alignment check:             ", ( eflags & ( 1 << 18 ) ) != 0 } );
            flags.push_back( { "Virtual interrupt:           ", ( eflags & ( 1 << 19 ) ) != 0 } );
            flags.push_back( { "Virtual interrupt pending:   ", ( eflags & ( 1 << 20 ) ) != 0 } );
            flags.push_back( { "CPUID:                       ", ( eflags & ( 1 << 21 ) ) != 0 } );
            
            for( const auto & p: flags )
            {
                win.move( 2, y++ );
                win.print( p.first + ( ( p.second ) ? "Yes" : " No" ) );
            }
            
            win.move( 1, y++ );
            win.addHorizontalLine( width - 2 );
            win.move( 2, y++ );
            win.print( String::toBinary( eflags ) );
        }
        
        this->_screen->refresh();
        win.move( 0, 0 );
        win.refresh();
    }
    
    void UI::IMPL::_displayStack( void )
    {
        size_t x(      54 + 36 );
        size_t y(      0 );
        size_t width(  30 );
        size_t height( 21 );
        Window win( x, y, width, height );
        
        if( this->_screen->width() < x + width )
        {
            return;
        }
        
        win.box();
        win.move( 2, 1 );
        win.print( "Stack Frame:" );
        win.move( 1, 2 );
        win.addHorizontalLine( width - 2 );
        
        y = 3;
        
        {
            uint16_t ss( this->_engine.ss() );
            uint64_t bp( Engine::getAddress( ss, this->_engine.bp() ) );
            uint64_t sp( Engine::getAddress( ss, this->_engine.sp() ) );
            
            std::vector< std::pair< uint64_t, uint16_t > > frame;
            
            while( sp + 1 < bp )
            {
                std::vector< uint8_t > data( this->_engine.read( sp, 2 ) );
                uint16_t               i( 0 );
                
                if( data.size() != 2 )
                {
                    break;
                }
                
                i   = data[ 0 ];
                i <<= 8;
                i  |= data[ 1 ];
                
                frame.push_back( { sp, i } );
                
                sp += 2;
            }
            
            if( frame.size() == 0 )
            {
                for( size_t i = y; i < height - 1; i++ )
                {
                    win.move( 2, y++ );
                    
                    for( size_t j = 2; j < width - 2; j++ )
                    {
                        win.print( "." );
                    }
                }
            }
            else
            {
                for( auto p: frame )
                {
                    if( y == height - 1 )
                    {
                        break;
                    }
                    
                    win.move( 2, y++ );
                    win.print( String::toHex( p.first ) + ": " + String::toHex( p.second ) );
                }
            }
        }
        
        this->_screen->refresh();
        win.move( 0, 0 );
        win.refresh();
    }
    
    void UI::IMPL::_displayInstructions( void )
    {
        size_t x(      54 + 36 + 30 );
        size_t y(      0 );
        size_t width(  56 );
        size_t height( 21 );
        Window win( x, y, width, height );
        
        if( this->_screen->width() < x + width )
        {
            return;
        }
        
        win.box();
        win.move( 2, 1 );
        win.print( "Instructions:" );
        win.move( 1, 2 );
        win.addHorizontalLine( width - 2 );
        
        y = 3;
        
        try
        {
            uint64_t                   ip( Engine::getAddress( this->_engine.cs(), this->_engine.ip() ) );
            std::vector< uint8_t >     bytes( this->_engine.read( ip, 512 ) );
            std::vector< std::string > instructions( Capstone::instructions( bytes, ip ) );
            
            for( const auto & s: instructions )
            {
                if( y == height - 1 )
                {
                    break;
                }
                
                win.move( 2, y++ );
                win.print( s );
            }
        }
        catch( ... )
        {}
        
        this->_screen->refresh();
        win.move( 0, 0 );
        win.refresh();
    }
    
    void UI::IMPL::_displayDisassembly( void )
    {
        size_t x( 54 + 36 + 30 + 56 );
        
        if( this->_screen->width() < x + 50 )
        {
            return;
        }
        
        {
            size_t y(      0 );
            size_t width(  this->_screen->width() - x );
            size_t height( 21 );
            Window win( x, y, width, height );
            
            win.box();
            win.move( 2, 1 );
            win.print( "Disassembly:" );
            win.move( 1, 2 );
            win.addHorizontalLine( width - 2 );
            
            y = 3;
            
            try
            {
                uint64_t                   ip( Engine::getAddress( this->_engine.cs(), this->_engine.ip() ) );
                std::vector< uint8_t >     bytes( this->_engine.read( ip, 512 ) );
                std::vector< std::string > instructions( Capstone::disassemble( bytes, ip ) );
                
                for( const auto & s: instructions )
                {
                    if( y == height - 1 )
                    {
                        break;
                    }
                    
                    win.move( 2, y++ );
                    win.print( s );
                }
            }
            catch( ... )
            {}
            
            this->_screen->refresh();
            win.move( 0, 0 );
            win.refresh();
        }
    }
    
    void UI::IMPL::_displayMemory( void )
    {
        size_t x(      0 );
        size_t y(      21 );
        size_t width(  this->_screen->width() );
        size_t height( ( this->_screen->height() - y ) / 2 );
        Window win( x, y, width, height );
        
        win.box();
        win.move( 2, 1 );
        win.print( "Memory:" );
        win.move( 1, 2 );
        win.addHorizontalLine( width - 2 );
        
        y = 3;
        
        if( this->_memoryAddressPrompt.has_value() )
        {
            win.move( 2, 3 );
            win.print( "Enter a memory address:" );
            win.move( 2, 4 );
            win.print( this->_memoryAddressPrompt.value() );
        }
        else
        {
            size_t cols(  this->_screen->width()  - 4 );
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
                        win.move( 2, y++ );
                        win.print( "%016X: ", offset );
                        
                        offset += this->_memoryBytesPerLine;
                    }
                    
                    win.print( "%02X ", numeric_cast< int >( mem[ i ] ) );
                }
                
                y = 3;
                
                win.move( ( this->_memoryBytesPerLine * 3 ) + 4 + 16, y );
                win.addVerticalLine( lines );
                
                for( size_t i = 0; i < mem.size(); i++ )
                {
                    char c = static_cast< char >( mem[ i ] );
                    
                    if( i % this->_memoryBytesPerLine == 0 )
                    {
                        win.move( ( this->_memoryBytesPerLine * 3 ) + 4 + 18, y++ );
                    }
                    
                    if( isprint( c ) == false || isspace( c ) )
                    {
                        c = '.';
                    }
                    
                    win.print( "%c", c );
                }
            }
        }
        
        this->_screen->refresh();
        win.move( 0, 0 );
        win.refresh();
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
