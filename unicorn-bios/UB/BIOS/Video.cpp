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
            bool setVideoMode( const Machine & machine, Engine & engine )
            {
                uint8_t mode( engine.al() );
                uint8_t maskedMode( mode & 0x7f );
                
                if( machine.debugVideo() )
                {
                    std::string description( "Unknown mode" );
                    
                    machine.ui().debug() << "Setting video mode to " << String::toHex( mode ) << ":" << std::endl;
                    
                    switch( mode )
                    {
                        case 0x00: description = "40x25 B/W text (CGA,EGA,MCGA,VGA)";                break;
                        case 0x01: description = "40x25 16 color text (CGA,EGA,MCGA,VGA)";           break;
                        case 0x02: description = "80x25 16 shades of gray text (CGA,EGA,MCGA,VGA)";  break;
                        case 0x03: description = "80x25 16 color text (CGA,EGA,MCGA,VGA)";           break;
                        case 0x04: description = "320x200 4 color graphics (CGA,EGA,MCGA,VGA)";      break;
                        case 0x05: description = "320x200 4 color graphics (CGA,EGA,MCGA,VGA)";      break;
                        case 0x06: description = "640x200 B/W graphics (CGA,EGA,MCGA,VGA)";          break;
                        case 0x07: description = "80x25 Monochrome text (MDA,HERC,EGA,VGA)";         break;
                        case 0x08: description = "160x200 16 color graphics (PCjr)";                 break;
                        case 0x09: description = "320x200 16 color graphics (PCjr)";                 break;
                        case 0x0A: description = "640x200 4 color graphics (PCjr)";                  break;
                        case 0x0B: description = "Reserved (EGA BIOS function 11)";                  break;
                        case 0x0C: description = "Reserved (EGA BIOS function 11)";                  break;
                        case 0x0D: description = "320x200 16 color graphics (EGA,VGA)";              break;
                        case 0x0E: description = "640x200 16 color graphics (EGA,VGA)";              break;
                        case 0x0F: description = "640x350 Monochrome graphics (EGA,VGA)";            break;
                        case 0x10: description = "640x350 16 color graphics (EGA or VGA with 128K)"; break;
                        case 0x11: description = "640x480 B/W graphics (MCGA,VGA)";                  break;
                        case 0x12: description = "640x480 16 color graphics (VGA)";                  break;
                        case 0x13: description = "320x200 256 color graphics (MCGA,VGA)";            break;
                        
                        default: break;
                    }
                    
                    machine.ui().debug() << "    - " << description << std::endl;
                }
                
                if( maskedMode > 7 )
                {
                    engine.al( 0x20 );
                }
                else if( maskedMode == 6 )
                {
                    engine.al( 0x3F );
                }
                else
                {
                    engine.al( 0x30 );
                }
                
                return true;
            }
            
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
