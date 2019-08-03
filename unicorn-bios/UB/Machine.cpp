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

#include "UB/Machine.hpp"
#include "UB/Engine.hpp"
#include "UB/Screen.hpp"
#include "UB/Interrupts.hpp"
#include "UB/FAT/MBR.hpp"
#include "UB/String.hpp"
#include "UB/CPU/Functions.hpp"
#include <sstream>
#include <atomic>
#include <csignal>
#include <vector>
#include <iostream>

namespace UB
{
    class Machine::IMPL
    {
        public:
            
            IMPL( size_t memory, const FAT::Image & fat, UI::Mode mode );
            IMPL( const IMPL & o );
            ~IMPL( void );
            
            static size_t memorySizeOrDefault( size_t memory );
            
            void _setup( const Machine & machine );
            void _break( const std::string & message = "" );
            
            size_t                  _memory;
            FAT::Image              _fat;
            UI::Mode                _mode;
            Engine                  _engine;
            UI                      _ui;
            BIOS::MemoryMap         _memoryMap;
            std::atomic< bool >     _breakOnInterrupt;
            std::atomic< bool >     _breakOnInterruptReturn;
            std::atomic< bool >     _trap;
            std::atomic< bool >     _debugVideo;
            std::atomic< bool >     _singleStep;
            std::vector< uint64_t > _breakpoints;
    };

    Machine::Machine( size_t memory, const FAT::Image & fat, UI::Mode mode ):
        impl( std::make_unique< IMPL >( memory, fat, mode ) )
    {
        this->impl->_setup( *( this ) );
    }

    Machine::Machine( const Machine & o ):
        impl( std::make_unique< IMPL >( *( o.impl ) ) )
    {
        this->impl->_setup( *( this ) );
    }

    Machine::Machine( Machine && o ) noexcept:
        impl( std::move( o.impl ) )
    {}

    Machine::~Machine( void )
    {}

    Machine & Machine::operator =( Machine o )
    {
        swap( *( this ), o );
        
        return *( this );
    }
    
    const FAT::Image & Machine::bootImage( void ) const
    {
        return this->impl->_fat;
    }
    
    const BIOS::MemoryMap & Machine::memoryMap( void ) const
    {
        return this->impl->_memoryMap;
    }
    
    UI & Machine::ui( void ) const
    {
        return this->impl->_ui;
    }
    
    void Machine::run( void )
    {
        if( this->impl->_engine.start( 0x7C00 ) == false )
        {
            throw std::runtime_error( "Cannot start engine" );
        }
        
        this->impl->_ui.mode( this->impl->_mode );
        this->impl->_ui.run();
        this->impl->_engine.stop();
    }
    
    bool Machine::breakOnInterrupt( void ) const
    {
        return this->impl->_breakOnInterrupt;
    }
    
    bool Machine::breakOnInterruptReturn( void ) const
    {
        return this->impl->_breakOnInterruptReturn;
    }
    
    bool Machine::trap( void ) const
    {
        return this->impl->_trap;
    }
    
    bool Machine::debugVideo( void ) const
    {
        return this->impl->_debugVideo;
    }
    
    bool Machine::singleStep( void ) const
    {
        return this->impl->_singleStep;
    }
    
    void Machine::breakOnInterrupt( bool value )
    {
        this->impl->_breakOnInterrupt = value;
    }
    
    void Machine::breakOnInterruptReturn( bool value )
    {
        this->impl->_breakOnInterruptReturn = value;
    }
    
    void Machine::trap( bool value )
    {
        this->impl->_trap = value;
    }
    
    void Machine::debugVideo( bool value )
    {
        this->impl->_debugVideo = value;
    }
    
    void Machine::singleStep( bool value )
    {
        this->impl->_singleStep = value;
    }
    
    void Machine::addBreakpoint( uint64_t address )
    {
        this->impl->_breakpoints.push_back( address );
    }
    
    void Machine::removeBreakpoint( uint64_t address )
    {
        this->impl->_breakpoints.erase
        (
            std::remove
            (
                this->impl->_breakpoints.begin(),
                this->impl->_breakpoints.end(),
                address
            ),
            this->impl->_breakpoints.end()
        );
    }
    
    void swap( Machine & o1, Machine & o2 )
    {
        using std::swap;
        
        swap( o1.impl, o2.impl );
    }

    Machine::IMPL::IMPL( size_t memory, const FAT::Image & fat, UI::Mode mode ):
        _memory(                 memorySizeOrDefault( memory ) ),
        _fat(                    fat ),
        _mode(                   mode ),
        _engine(                 memorySizeOrDefault( memory ) ),
        _ui(                     this->_engine ),
        _memoryMap(              memorySizeOrDefault( memory ) ),
        _breakOnInterrupt(       false ),
        _breakOnInterruptReturn( false ),
        _trap(                   false ),
        _debugVideo(             false ),
        _singleStep(             false )
    {}

    Machine::IMPL::IMPL( const IMPL & o ):
        _memory(                 o._memory ),
        _fat(                    o._fat ),
        _mode(                   o._mode ),
        _engine(                 o._memory ),
        _ui(                     this->_engine ),
        _memoryMap(              o._memoryMap ),
        _breakOnInterrupt(       o._breakOnInterrupt.load() ),
        _breakOnInterruptReturn( o._breakOnInterruptReturn.load() ),
        _trap(                   o._trap.load() ),
        _debugVideo(             o._debugVideo.load() ),
        _singleStep(             o._singleStep.load() )
    {}

    Machine::IMPL::~IMPL( void )
    {}
    
    size_t Machine::IMPL::memorySizeOrDefault( size_t memory )
    {
        if( memory == 0 )
        {
            memory = 64;
        }
        else if( memory == 1 )
        {
            memory = 2;
        }
        
        return memory * 1024 * 1024;
    }
    
    void Machine::IMPL::_setup( const Machine & machine )
    {
        FAT::MBR               mbr( this->_fat.mbr() );
        std::vector< uint8_t > mbrData( mbr.data() );
        
        if( mbrData.size() != 512 )
        {
            throw std::runtime_error( "Invalid MBR size: " + std::to_string( mbrData.size() ) );
        }
        
        this->_engine.write( 0x7C00, mbrData );
        
        this->_engine.onException
        (
            [ & ]( const std::exception & e ) -> bool
            {
                this->_ui.debug() << "[ ERROR ]> Exception caught: " << e.what() << std::endl;
                
                return true;
            }
        );
        
        this->_engine.beforeInstruction
        (
            [ & ]( uint64_t address, const std::vector< uint8_t > & instruction )
            {
                ( void )address;
                ( void )instruction;
                
                if( this->_singleStep )
                {
                    this->_break();
                }
                else
                {
                    uint64_t ip( this->_engine.eip() );
                    
                    if( std::find( this->_breakpoints.begin(), this->_breakpoints.end(), ip ) != this->_breakpoints.end() )
                    {
                        this->_break( String::toHex( ip ) );
                    }
                }
            }
        );
        
        this->_engine.afterInstruction
        (
            [ & ]( uint64_t address, const Registers & registers, const std::vector< uint8_t > & instruction )
            {
                ( void )address;
                
                if
                (
                       instruction.size() == 2
                    && instruction[ 0 ]   == 0x0F
                    && instruction[ 1 ]   == 0xA2
                )
                {
                    CPU::cpuid( this->_engine, registers );
                }
            }
        );
        
        this->_engine.onInterrupt
        (
            [ & ]( uint32_t i ) -> bool
            {
                bool ret( false );
                
                if( this->_breakOnInterrupt )
                {
                    this->_break( "Interrupt " + String::toHex( i ) );
                }
                
                switch( i )
                {
                    case 0x05: ret = Interrupts::int0x05( machine, this->_engine ); break;
                    case 0x10: ret = Interrupts::int0x10( machine, this->_engine ); break;
                    case 0x11: ret = Interrupts::int0x11( machine, this->_engine ); break;
                    case 0x12: ret = Interrupts::int0x12( machine, this->_engine ); break;
                    case 0x13: ret = Interrupts::int0x13( machine, this->_engine ); break;
                    case 0x14: ret = Interrupts::int0x14( machine, this->_engine ); break;
                    case 0x15: ret = Interrupts::int0x15( machine, this->_engine ); break;
                    case 0x16: ret = Interrupts::int0x16( machine, this->_engine ); break;
                    case 0x17: ret = Interrupts::int0x17( machine, this->_engine ); break;
                    case 0x18: ret = Interrupts::int0x18( machine, this->_engine ); break;
                    case 0x19: ret = Interrupts::int0x19( machine, this->_engine ); break;
                    case 0x1A: ret = Interrupts::int0x1A( machine, this->_engine ); break;
                    
                    default: break;
                }
                
                if( this->_breakOnInterruptReturn )
                {
                    this->_break( "Return from interrupt" );
                }
                
                return ret;
            }
        );
        
        this->_engine.onValidMemoryAccess
        (
            [ & ]( uint64_t address, size_t size )
            {
                if( this->_engine.running() == false )
                {
                    return;
                }
                
                for( const auto & entry: this->_memoryMap.entries() )
                {
                    uint64_t end( address + size );
                    
                    if( entry.type() == BIOS::MemoryMap::Entry::Type::Usable )
                    {
                        continue;
                    }
                    
                    if( ( address >= entry.base() && address <= entry.end() ) || ( end >= entry.base() && end <= entry.end() ) )
                    {
                        throw std::runtime_error( "Access to invalid memory at address " + String::toHex( address ) );
                    }
                }
            }
        );
        
        this->_engine.onInvalidMemoryAccess
        (
            [ & ]( uint64_t address, size_t size )
            {
                ( void )size;
                
                throw std::runtime_error( "Access to invalid memory at address " + String::toHex( address ) );
            }
        );
        
        if( this->_mode == UI::Mode::Interactive )
        {
            /*
            Screen::shared().onKeyPress
            (
                [ & ]( int key )
                {
                    if( key == 0x20 )
                    {
                        this->_singleStep = true;
                    }
                }
            );
            */
        }
    }
    
    void Machine::IMPL::_break( const std::string & message )
    {
        if( message.length() > 0 )
        {
            this->_ui.debug() << "[ BREAK ]> " << message << std::endl;
        }
        
        if( this->_trap )
        {
            raise( SIGTRAP );
        }
        else
        {
            if( this->_ui.waitForUserResume() == 0x20 )
            {
                this->_singleStep = true;
            }
            else
            {
                this->_singleStep = false;
            }
        }
    }
}
