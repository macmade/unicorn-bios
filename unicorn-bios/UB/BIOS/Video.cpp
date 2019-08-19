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

#include "UB/BIOS/Video.hpp"
#include "UB/BIOS/VESAInfo.hpp"
#include "UB/Machine.hpp"
#include "UB/Engine.hpp"
#include "UB/String.hpp"
#include <cctype>

namespace UB
{
    namespace BIOS
    {
        namespace Video
        {
            bool setCursorPosition( const Machine & machine, Engine & engine )
            {
                if( machine.debugVideo() )
                {
                    machine.ui().debug() << "Setting cursor position:"
                                         << std::endl
                                         << "    - Page:   " << std::to_string( static_cast< unsigned int >( engine.bh() ) )
                                         << std::endl
                                         << "    - Row:    " << std::to_string( static_cast< unsigned int >( engine.dh() ) )
                                         << std::endl
                                         << "    - Column: " << std::to_string( static_cast< unsigned int >( engine.dl() ) )
                                         << std::endl;
                }
                
                return true;
            }
            
            bool ttyOutput( const Machine & machine, Engine & engine )
            {
                char c( static_cast< char >( engine.al() ) );
                
                if( machine.debugVideo() )
                {
                    machine.ui().debug() << "TTY output: " << String::toHex( engine.al() ) << std::endl;
                }
                
                if( std::isprint( c ) || std::isspace( c ) )
                {
                    machine.ui().output() << std::string( 1, c );
                }
                else
                {
                    machine.ui().output() << ".";
                }
                
                return true;
            }
            
            bool palette( const Machine & machine, Engine & engine )
            {
                if( engine.al() == 0x10 )
                {
                    if( machine.debugVideo() )
                    {
                        machine.ui().debug() << "Setting DAC color: " << String::toHex( engine.bx() )
                                             << std::endl
                                             << "    - R: " << String::toHex( engine.dh() )
                                             << std::endl
                                             << "    - G: " << String::toHex( engine.ch() )
                                             << std::endl
                                             << "    - B: " << String::toHex( engine.cl() )
                                             << std::endl;
                    }
                    
                    return true;
                }
                
                return false;
            }
            
            bool writeCharacterAndAttributeAtCursor( const Machine & machine, Engine & engine )
            {
                if( machine.debugVideo() )
                {
                    machine.ui().debug() << "Writing character: " << String::toHex( engine.al() )
                                         << std::endl
                                         << "    - Page:  " << std::to_string( static_cast< unsigned int >( engine.bh() ) )
                                         << std::endl
                                         << "    - Color: " << String::toHex( engine.bl() )
                                         << std::endl
                                         << "    - Times: "<< std::to_string( static_cast< unsigned int >( engine.cx() ) )
                                         << std::endl;
                }
                
                return true;
            }
            
            bool writeCharacterOnlyAtCursor( const Machine & machine, Engine & engine )
            {
                if( machine.debugVideo() )
                {
                    machine.ui().debug() << "Writing character: " << String::toHex( engine.al() )
                                         << std::endl
                                         << "    - Page:  " << std::to_string( static_cast< unsigned int >( engine.bh() ) )
                                         << std::endl
                                         << "    - Times: "<< std::to_string( static_cast< unsigned int >( engine.cx() ) )
                                         << std::endl;
                }
                
                return true;
            }
            
            bool getVBEControllerInfo( const Machine & machine, Engine & engine )
            {
                uint64_t               destination( Engine::getAddress( engine.es(), engine.di() ) );
                VESAInfo               vesa;
                std::vector< uint8_t > data( vesa.data() );
                
                machine.ui().debug() << "Getting VBE controller info: "
                                     << std::endl
                                     << "    - Destination: " << String::toHex( destination ) << " (" << String::toHex( engine.es() ) << ":" << String::toHex( engine.di() ) << ")"
                                     << std::endl;
                
                engine.write( destination, data );
                
                return true;
            }
        }
    }
}
