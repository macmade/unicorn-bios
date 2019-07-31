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

#include "UB/Arguments.hpp"
#include "UB/Casts.hpp"
#include <vector>

namespace UB
{
    class Arguments::IMPL
    {
        public:
            
            IMPL( int argc, const char * argv[] );
            IMPL( const IMPL & o );
            
            bool        _showHelp;
            bool        _breakOnInterrupt;
            bool        _breakOnInterruptReturn;
            bool        _debugVideo;
            size_t      _memory;
            std::string _bootImage;
    };
    
    Arguments::Arguments( int argc, const char * argv[] ):
        impl( std::make_unique< IMPL >( argc, argv ) )
    {}
    
    Arguments::Arguments( const Arguments & o ):
        impl( std::make_unique< IMPL >( *( o.impl ) ) )
    {}
    
    Arguments::Arguments( Arguments && o ) noexcept:
        impl( std::move( o.impl ) )
    {}
    
    Arguments::~Arguments( void )
    {}
    
    Arguments & Arguments::operator =( Arguments o )
    {
        swap( *( this ), o );
        
        return *( this );
    }
    
    bool Arguments::showHelp( void ) const
    {
        return this->impl->_showHelp;
    }
    
    bool Arguments::breakOnInterrupt( void ) const
    {
        return this->impl->_breakOnInterrupt;
    }
    
    bool Arguments::breakOnInterruptReturn( void ) const
    {
        return this->impl->_breakOnInterruptReturn;
    }
    
    bool Arguments::debugVideo( void ) const
    {
        return this->impl->_debugVideo;
    }
    
    size_t Arguments::memory( void ) const
    {
        return this->impl->_memory;
    }
    
    std::string Arguments::bootImage( void ) const
    {
        return this->impl->_bootImage;
    }
    
    void swap( Arguments & o1, Arguments & o2 )
    {
        using std::swap;
        
        swap( o1.impl, o2.impl );
    }
    
    Arguments::IMPL::IMPL( int argc, const char * argv[] ):
        _showHelp(               false ),
        _breakOnInterrupt(       false ),
        _breakOnInterruptReturn( false ),
        _debugVideo(             false ),
        _memory(                 0 )
    {
        if( argc < 1 )
        {
            return;
        }
        
        for( int i = 1; i < argc; i++ )
        {
            std::string arg( argv[ i ] );
            
            if( arg == "--help" || arg == "-h" )
            {
                this->_showHelp = true;
            }
            else if( arg == "--break-int" )
            {
                this->_breakOnInterrupt = true;
            }
            else if( arg == "--break-iret" )
            {
                this->_breakOnInterruptReturn = true;
            }
            else if( arg == "--debug-video" )
            {
                this->_debugVideo = true;
            }
            else if( arg == "--memory" || arg == "-m" )
            {
                if( ++i < argc )
                {
                    try
                    {
                        this->_memory = static_cast< size_t >( std::atoll( argv[ i ] ) );
                    }
                    catch( ... )
                    {}
                }
            }
            else if( this->_bootImage.length() == 0 )
            {
                this->_bootImage = arg;
            }
        }
    }
    
    Arguments::IMPL::IMPL( const IMPL & o ):
        _showHelp(                o._showHelp ),
        _breakOnInterrupt(        o._breakOnInterrupt ),
        _breakOnInterruptReturn(  o._breakOnInterruptReturn ),
        _debugVideo(              o._debugVideo ),
        _memory(                  o._memory ),
        _bootImage(               o._bootImage )
    {}
}
