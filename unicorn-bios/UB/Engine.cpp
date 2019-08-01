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

/*!
 * @file        Engine.cpp
 * @copyright   (c) 2019, Jean-David Gadina - www.xs-labs.com
 */

#include "UB/Engine.hpp"
#include "UB/String.hpp"
#include "UB/Casts.hpp"
#include <unicorn/unicorn.h>
#include <map>
#include <mutex>
#include <condition_variable>
#include <thread>

namespace UB
{
    class Engine::IMPL
    {
        public:
            
            IMPL( size_t memory );
            ~IMPL( void );
            
            static void _handleInterrupt(   uc_engine * uc, uint32_t i, void * data );
            static void _handleInstruction( uc_engine * uc, uint64_t address, uint32_t size, void * data );
            static void _handleInvalidMemoryAccess( uc_engine * uc, uc_mem_type type, uint64_t address, int size, int64_t value, void * data );
            static void _handleValidMemoryAccess( uc_engine * uc, uc_mem_type type, uint64_t address, int size, int64_t value, void * data );
            
            std::vector< uint8_t > _read( size_t address, size_t size );
            void                   _write( size_t address, const uint8_t * bytes, size_t size );
            
            size_t                                                         _memory;
            std::vector< std::function< void( void ) > >                   _onStart;
            std::vector< std::function< void( void ) > >                   _onStop;
            std::vector< std::function< bool( uint32_t ) > >               _interruptHandlers;
            std::vector< std::function< bool( const std::exception & ) > > _exceptionHandlers;
            std::vector< std::function< void( uint64_t, uint32_t ) > >     _instructionHandlers;
            std::vector< std::function< void( uint64_t, size_t ) > >       _invalidMemoryHandlers;
            std::vector< std::function< void( uint64_t, size_t ) > >       _validMemoryHandlers;
            uc_engine                                                    * _uc;
            bool                                                           _running;
            mutable std::recursive_mutex                                   _rmtx;
            std::condition_variable_any                                    _cv;
            
            template< typename _T_ >
            _T_ _readRegister( int reg ) const
            {
                _T_                                     v( 0 );
                uc_err                                  e;
                std::lock_guard< std::recursive_mutex > l( this->_rmtx );
                
                if( ( e = uc_reg_read( this->_uc, reg, &v ) ) != UC_ERR_OK )
                {
                    throw std::runtime_error( uc_strerror( e ) );
                }
                
                return v;
            }
            
            template< typename _T_ >
            void _writeRegister( int reg, _T_ value )
            {
                uc_err                                  e;
                std::lock_guard< std::recursive_mutex > l( this->_rmtx );
                
                if( ( e = uc_reg_write( this->_uc, reg, &value ) ) != UC_ERR_OK )
                {
                    throw std::runtime_error( uc_strerror( e ) );
                }
            }
    };
    
    uint64_t Engine::getAddress( uint16_t segment, uint16_t offset )
    {
        uint64_t address( segment );
        
        address <<= 4;
        address  += offset;
        
        return address;
    }
    
    Engine::Engine( size_t memory ):
        impl( std::make_unique< IMPL >( memory ) )
    {
        uc_hook h1;
        uc_hook h2;
        uc_hook h3;
        uc_hook h4;
        uc_err  e;
        
        if( ( e = uc_hook_add( this->impl->_uc, &h1, UC_HOOK_INTR, reinterpret_cast< void * >( &IMPL::_handleInterrupt ), this, 1, 0 ) ) != UC_ERR_OK )
        {
            throw std::runtime_error( uc_strerror( e ) );
        }
        
        if( ( e = uc_hook_add( this->impl->_uc, &h2, UC_HOOK_CODE, reinterpret_cast< void * >( &IMPL::_handleInstruction ), this, 1, 0 ) ) != UC_ERR_OK )
        {
            throw std::runtime_error( uc_strerror( e ) );
        }
        
        if( ( e = uc_hook_add( this->impl->_uc, &h3, UC_HOOK_MEM_INVALID, reinterpret_cast< void * >( &IMPL::_handleInvalidMemoryAccess ), this, 1, 0 ) ) != UC_ERR_OK )
        {
            throw std::runtime_error( uc_strerror( e ) );
        }
        
        if( ( e = uc_hook_add( this->impl->_uc, &h4, UC_HOOK_MEM_VALID, reinterpret_cast< void * >( &IMPL::_handleValidMemoryAccess ), this, 1, 0 ) ) != UC_ERR_OK )
        {
            throw std::runtime_error( uc_strerror( e ) );
        }
    }
    
    Engine::~Engine( void )
    {}
    
    size_t Engine::memory( void ) const
    {
        return this->impl->_memory;
    }
    
    bool Engine::cf( void ) const
    {
        uint32_t flags( this->eflags() );
        
        return ( flags & 0x01 ) != 0;
    }
    
    uint8_t Engine::ah( void ) const
    {
        return this->impl->_readRegister< uint8_t >( UC_X86_REG_AH );
    }
    
    uint8_t Engine::al( void ) const
    {
        return this->impl->_readRegister< uint8_t >( UC_X86_REG_AL );
    }
    
    uint8_t Engine::bh( void ) const
    {
        return this->impl->_readRegister< uint8_t >( UC_X86_REG_BH );
    }
    
    uint8_t Engine::bl( void ) const
    {
        return this->impl->_readRegister< uint8_t >( UC_X86_REG_BL );
    }
    
    uint8_t Engine::ch( void ) const
    {
        return this->impl->_readRegister< uint8_t >( UC_X86_REG_CH );
    }
    
    uint8_t Engine::cl( void ) const
    {
        return this->impl->_readRegister< uint8_t >( UC_X86_REG_CL );
    }
    
    uint8_t Engine::dh( void ) const
    {
        return this->impl->_readRegister< uint8_t >( UC_X86_REG_DH );
    }
    
    uint8_t Engine::dl( void ) const
    {
        return this->impl->_readRegister< uint8_t >( UC_X86_REG_DL );
    }
    
    uint16_t Engine::ax( void ) const
    {
        return this->impl->_readRegister< uint16_t >( UC_X86_REG_AX );
    }
    
    uint16_t Engine::bx( void ) const
    {
        return this->impl->_readRegister< uint16_t >( UC_X86_REG_BX );
    }
    
    uint16_t Engine::cx( void ) const
    {
        return this->impl->_readRegister< uint16_t >( UC_X86_REG_CX );
    }
    
    uint16_t Engine::dx( void ) const
    {
        return this->impl->_readRegister< uint16_t >( UC_X86_REG_DX );
    }
    
    uint16_t Engine::si( void ) const
    {
        return this->impl->_readRegister< uint16_t >( UC_X86_REG_SI );
    }
    
    uint16_t Engine::di( void ) const
    {
        return this->impl->_readRegister< uint16_t >( UC_X86_REG_DI );
    }
    
    uint16_t Engine::sp( void ) const
    {
        return this->impl->_readRegister< uint16_t >( UC_X86_REG_SP );
    }
    
    uint16_t Engine::bp( void ) const
    {
        return this->impl->_readRegister< uint16_t >( UC_X86_REG_BP );
    }
    
    uint16_t Engine::cs( void ) const
    {
        return this->impl->_readRegister< uint16_t >( UC_X86_REG_CS );
    }
    
    uint16_t Engine::ds( void ) const
    {
        return this->impl->_readRegister< uint16_t >( UC_X86_REG_DS );
    }
    
    uint16_t Engine::es( void ) const
    {
        return this->impl->_readRegister< uint16_t >( UC_X86_REG_ES );
    }
    
    uint16_t Engine::ss( void ) const
    {
        return this->impl->_readRegister< uint16_t >( UC_X86_REG_SS );
    }
    
    uint16_t Engine::ip( void ) const
    {
        return this->impl->_readRegister< uint16_t >( UC_X86_REG_IP );
    }
    
    uint32_t Engine::eax( void ) const
    {
        return this->impl->_readRegister< uint32_t >( UC_X86_REG_EAX );
    }
    
    uint32_t Engine::ebx( void ) const
    {
        return this->impl->_readRegister< uint32_t >( UC_X86_REG_EBX );
    }
    
    uint32_t Engine::ecx( void ) const
    {
        return this->impl->_readRegister< uint32_t >( UC_X86_REG_ECX );
    }
    
    uint32_t Engine::edx( void ) const
    {
        return this->impl->_readRegister< uint32_t >( UC_X86_REG_EDX );
    }
    
    uint32_t Engine::esi( void ) const
    {
        return this->impl->_readRegister< uint32_t >( UC_X86_REG_ESI );
    }
    
    uint32_t Engine::edi( void ) const
    {
        return this->impl->_readRegister< uint32_t >( UC_X86_REG_EDI );
    }
    
    uint32_t Engine::esp( void ) const
    {
        return this->impl->_readRegister< uint32_t >( UC_X86_REG_ESP );
    }
    
    uint32_t Engine::ebp( void ) const
    {
        return this->impl->_readRegister< uint32_t >( UC_X86_REG_EBP );
    }
    
    uint32_t Engine::eip( void ) const
    {
        return this->impl->_readRegister< uint32_t >( UC_X86_REG_EIP );
    }
    
    uint32_t Engine::eflags( void ) const
    {
        return this->impl->_readRegister< uint32_t >( UC_X86_REG_EFLAGS );
    }
    
    void Engine::cf( bool value )
    {
        std::lock_guard< std::recursive_mutex > l( this->impl->_rmtx );
        uint32_t                                flags( this->eflags() );
        
        if( value )
        {
            flags |= 0x01;
        }
        else
        {
            flags &= ~static_cast< uint32_t >( 0x01 );
        }
        
        this->eflags( flags );
    }
    
    void Engine::ah( uint8_t value )
    {
        this->impl->_writeRegister( UC_X86_REG_AH, value );
    }
    
    void Engine::al( uint8_t value )
    {
        this->impl->_writeRegister( UC_X86_REG_AL, value );
    }
    
    void Engine::bh( uint8_t value )
    {
        this->impl->_writeRegister( UC_X86_REG_BH, value );
    }
    
    void Engine::bl( uint8_t value )
    {
        this->impl->_writeRegister( UC_X86_REG_BL, value );
    }
    
    void Engine::ch( uint8_t value )
    {
        this->impl->_writeRegister( UC_X86_REG_CH, value );
    }
    
    void Engine::cl( uint8_t value )
    {
        this->impl->_writeRegister( UC_X86_REG_CL, value );
    }
    
    void Engine::dh( uint8_t value )
    {
        this->impl->_writeRegister( UC_X86_REG_DH, value );
    }
    
    void Engine::dl( uint8_t value )
    {
        this->impl->_writeRegister( UC_X86_REG_DL, value );
    }
    
    void Engine::ax( uint16_t value )
    {
        this->impl->_writeRegister( UC_X86_REG_AX, value );
    }
    
    void Engine::bx( uint16_t value )
    {
        this->impl->_writeRegister( UC_X86_REG_BX, value );
    }
    
    void Engine::cx( uint16_t value )
    {
        this->impl->_writeRegister( UC_X86_REG_CX, value );
    }
    
    void Engine::dx( uint16_t value )
    {
        this->impl->_writeRegister( UC_X86_REG_DX, value );
    }
    
    void Engine::si( uint16_t value )
    {
        this->impl->_writeRegister( UC_X86_REG_SI, value );
    }
    
    void Engine::di( uint16_t value )
    {
        this->impl->_writeRegister( UC_X86_REG_DI, value );
    }
    
    void Engine::sp( uint16_t value )
    {
        this->impl->_writeRegister( UC_X86_REG_SP, value );
    }
    
    void Engine::bp( uint16_t value )
    {
        this->impl->_writeRegister( UC_X86_REG_BP, value );
    }
    
    void Engine::cs( uint16_t value )
    {
        this->impl->_writeRegister( UC_X86_REG_CS, value );
    }
    
    void Engine::ds( uint16_t value )
    {
        this->impl->_writeRegister( UC_X86_REG_DS, value );
    }
    
    void Engine::es( uint16_t value )
    {
        this->impl->_writeRegister( UC_X86_REG_ES, value );
    }
    
    void Engine::ss( uint16_t value )
    {
        this->impl->_writeRegister( UC_X86_REG_SS, value );
    }
    
    void Engine::ip( uint16_t value )
    {
        this->impl->_writeRegister( UC_X86_REG_IP, value );
    }
    
    void Engine::eax( uint32_t value )
    {
        this->impl->_writeRegister( UC_X86_REG_EAX, value );
    }
    
    void Engine::ebx( uint32_t value )
    {
        this->impl->_writeRegister( UC_X86_REG_EBX, value );
    }
    
    void Engine::ecx( uint32_t value )
    {
        this->impl->_writeRegister( UC_X86_REG_ECX, value );
    }
    
    void Engine::edx( uint32_t value )
    {
        this->impl->_writeRegister( UC_X86_REG_EDX, value );
    }
    
    void Engine::esi( uint32_t value )
    {
        this->impl->_writeRegister( UC_X86_REG_ESI, value );
    }
    
    void Engine::edi( uint32_t value )
    {
        this->impl->_writeRegister( UC_X86_REG_EDI, value );
    }
    
    void Engine::esp( uint32_t value )
    {
        this->impl->_writeRegister( UC_X86_REG_ESP, value );
    }
    
    void Engine::ebp( uint32_t value )
    {
        this->impl->_writeRegister( UC_X86_REG_EBP, value );
    }
    
    void Engine::eip( uint32_t value )
    {
        this->impl->_writeRegister( UC_X86_REG_EIP, value );
    }
    
    void Engine::eflags( uint32_t value )
    {
        this->impl->_writeRegister( UC_X86_REG_EFLAGS, value );
    }
    
    bool Engine::running( void ) const
    {
        std::lock_guard< std::recursive_mutex > l( this->impl->_rmtx );
        
        return this->impl->_running;
    }
    
    void Engine::onStart( const std::function< void( void ) > f )
    {
        std::lock_guard< std::recursive_mutex > l( this->impl->_rmtx );
        
        this->impl->_onStart.push_back( f );
    }
    
    void Engine::onStop( const std::function< void( void ) > f )
    {
        std::lock_guard< std::recursive_mutex > l( this->impl->_rmtx );
        
        this->impl->_onStop.push_back( f );
    }
    
    void Engine::onInterrupt( const std::function< bool( uint32_t ) > handler )
    {
        std::lock_guard< std::recursive_mutex > l( this->impl->_rmtx );
        
        this->impl->_interruptHandlers.push_back( handler );
    }
    
    void Engine::onException( const std::function< bool( const std::exception & ) > handler )
    {
        std::lock_guard< std::recursive_mutex > l( this->impl->_rmtx );
        
        this->impl->_exceptionHandlers.push_back( handler );
    }
    
    void Engine::onInstruction( const std::function< void( uint64_t, uint32_t ) > handler )
    {
        std::lock_guard< std::recursive_mutex > l( this->impl->_rmtx );
        
        this->impl->_instructionHandlers.push_back( handler );
    }
    
    void Engine::onInvalidMemoryAccess( const std::function< void( uint64_t, size_t ) > handler )
    {
        std::lock_guard< std::recursive_mutex > l( this->impl->_rmtx );
        
        this->impl->_invalidMemoryHandlers.push_back( handler );
    }
    
    void Engine::onValidMemoryAccess( const std::function< void( uint64_t, size_t ) > handler )
    {
        std::lock_guard< std::recursive_mutex > l( this->impl->_rmtx );
        
        this->impl->_validMemoryHandlers.push_back( handler );
    }
    
    std::vector< uint8_t > Engine::read( size_t address, size_t size )
    {
        return this->impl->_read( address, size );
    }
    
    void Engine::write( size_t address, const std::vector< uint8_t > & bytes )
    {
        this->impl->_write( address, &( bytes[ 0 ] ), bytes.size() );
    }
    
    void Engine::write( size_t address, const uint8_t * bytes, size_t size )
    {
        this->impl->_write( address, bytes, size );
    }
    
    bool Engine::start( size_t address )
    {
        {
            std::lock_guard< std::recursive_mutex > l( this->impl->_rmtx );
            
            if( this->impl->_running )
            {
                return false;
            }
            
            this->impl->_running = true;
            
            this->impl->_cv.notify_all();
            
            for( const auto & f: this->impl->_onStart )
            {
                f();
            }
        }
        
        std::thread
        (
            [ = ]
            {
                try
                {
                    uc_err e;
                    
                    if( ( e = uc_emu_start( this->impl->_uc, address, std::numeric_limits< uint64_t >::max(), 0, 0 ) ) != UC_ERR_OK )
                    {
                        throw std::runtime_error( uc_strerror( e ) );
                    }
                }
                catch( const std::exception & e )
                {
                    std::vector< std::function< bool( const std::exception & ) > > handlers;
                    bool                                                           handled( false );
                    
                    {
                        std::lock_guard< std::recursive_mutex > l( this->impl->_rmtx );
                        
                        handlers = this->impl->_exceptionHandlers;
                    }
                    
                    for( const auto & f: handlers )
                    {
                        if( f( e ) )
                        {
                            handled = true;
                        }
                    }
                    
                    if( handled == false )
                    {
                        throw e;
                    }
                }
                
                {
                    std::lock_guard< std::recursive_mutex > l( this->impl->_rmtx );
                    
                    this->impl->_running = false;
                    
                    this->impl->_cv.notify_all();
                    
                    for( const auto & f: this->impl->_onStop )
                    {
                        f();
                    }
                }
            }
        )
        .detach();
        
        return true;
    }
    
    void Engine::stop( void )
    {
        std::lock_guard< std::recursive_mutex > l( this->impl->_rmtx );
        
        if( this->impl->_running == false )
        {
            return;
        }
        
        uc_emu_stop( this->impl->_uc );
    }
    
    void Engine::waitUntilFinished( void ) const
    {
        std::unique_lock< std::recursive_mutex > l( this->impl->_rmtx );
        
        this->impl->_cv.wait
        (
            l,
            [ & ]( void ) -> bool
            {
                return this->impl->_running == false;
            }
        );
    }
    
    Engine::IMPL::IMPL( size_t memory ):
        _memory( memory ),
        _uc( nullptr ),
        _running( false )
    {
        uc_err e;
        
        if( ( e = uc_open( UC_ARCH_X86, UC_MODE_16, &( this->_uc ) ) ) != UC_ERR_OK )
        {
            throw std::runtime_error( uc_strerror( e ) );
        }
        
        if( memory > 0 )
        {
            if( ( e = uc_mem_map( this->_uc, 0, memory, UC_PROT_ALL ) ) != UC_ERR_OK )
            {
                throw std::runtime_error( uc_strerror( e ) );
            }
        }
    }
    
    Engine::IMPL::~IMPL( void )
    {
        if( this->_uc != nullptr )
        {
            uc_close( this->_uc );
        }
    }
    
    void Engine::IMPL::_handleInterrupt( uc_engine * uc, uint32_t i, void * data )
    {
        Engine                                         * engine;
        std::vector< std::function< bool( uint32_t ) > > handlers;
        
        ( void )uc;
        
        engine = static_cast< Engine * >( data );
        
        if( engine == nullptr )
        {
            throw std::runtime_error( "Fatal internal error: unknown engine" );
        }
        
        {
            std::lock_guard< std::recursive_mutex > l( engine->impl->_rmtx );
            
            handlers = engine->impl->_interruptHandlers;
        }
        
        for( const auto & f: handlers )
        {
            if( f( i ) )
            {
                return;
            }
        }
        
        throw std::runtime_error( "Unhandled interrupt: " + String::toHex( i ) );
    }
    
    void Engine::IMPL::_handleInstruction( uc_engine * uc, uint64_t address, uint32_t size, void * data )
    {
        Engine                                                   * engine;
        std::vector< std::function< void( uint64_t, uint32_t ) > > handlers;
        
        ( void )uc;
        
        engine = static_cast< Engine * >( data );
        
        if( engine == nullptr )
        {
            throw std::runtime_error( "Fatal internal error: unknown engine" );
        }
        
        {
            std::lock_guard< std::recursive_mutex > l( engine->impl->_rmtx );
            
            handlers = engine->impl->_instructionHandlers;
        }
        
        for( const auto & f: handlers )
        {
            f( address, size );
        }
    }
    
    void Engine::IMPL::_handleInvalidMemoryAccess( uc_engine * uc, uc_mem_type type, uint64_t address, int size, int64_t value, void * data )
    {
        Engine                                                 * engine;
        std::vector< std::function< void( uint64_t, size_t ) > > handlers;
        
        ( void )uc;
        ( void )type;
        ( void )value;
        
        engine = static_cast< Engine * >( data );
        
        if( engine == nullptr )
        {
            throw std::runtime_error( "Fatal internal error: unknown engine" );
        }
        
        {
            std::lock_guard< std::recursive_mutex > l( engine->impl->_rmtx );
            
            handlers = engine->impl->_invalidMemoryHandlers;
        }
        
        for( const auto & f: handlers )
        {
            f( address, numeric_cast< size_t >( size ) );
        }
    }
    
    void Engine::IMPL::_handleValidMemoryAccess( uc_engine * uc, uc_mem_type type, uint64_t address, int size, int64_t value, void * data )
    {
        Engine                                                 * engine;
        std::vector< std::function< void( uint64_t, size_t ) > > handlers;
        
        ( void )uc;
        ( void )type;
        ( void )value;
        
        engine = static_cast< Engine * >( data );
        
        if( engine == nullptr )
        {
            throw std::runtime_error( "Fatal internal error: unknown engine" );
        }
        
        {
            std::lock_guard< std::recursive_mutex > l( engine->impl->_rmtx );
            
            handlers = engine->impl->_validMemoryHandlers;
        }
        
        for( const auto & f: handlers )
        {
            f( address, numeric_cast< size_t >( size ) );
        }
    }
    
    std::vector< uint8_t > Engine::IMPL::_read( size_t address, size_t size )
    {
        uc_err                                  e;
        std::lock_guard< std::recursive_mutex > l( this->_rmtx );
        
        if( address + size >= this->_memory )
        {
            throw std::runtime_error( "Cannot read from address " + String::toHex( address ) + " - Not enough memory allocated" );
        }
        
        {
            std::vector< uint8_t > bytes( size, 0 );
            
            if( ( e = uc_mem_read( this->_uc, address, &( bytes[ 0 ] ), size ) ) != UC_ERR_OK )
            {
                throw std::runtime_error( uc_strerror( e ) );
            }
            
            return bytes;
        }
    }
    
    void Engine::IMPL::_write( size_t address, const uint8_t * bytes, size_t size )
    {
        uc_err                                  e;
        std::lock_guard< std::recursive_mutex > l( this->_rmtx );
        
        if( address >= this->_memory )
        {
            throw std::runtime_error( "Cannot write to address " + String::toHex( address ) + " - Not enough memory allocated" );
        }
        
        if( ( e = uc_mem_write( this->_uc, address, bytes, size ) ) != UC_ERR_OK )
        {
            throw std::runtime_error( uc_strerror( e ) );
        }
    }
}
