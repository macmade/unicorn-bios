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

#include "UB/BIOS/Disk.hpp"
#include "UB/Machine.hpp"
#include "UB/Engine.hpp"
#include "UB/String.hpp"
#include "UB/Casts.hpp"

namespace UB
{
    namespace BIOS
    {
        namespace Disk
        {
            void reset( const Machine & machine, Engine & engine )
            {
                machine.ui().debug() << "Resetting drive " << String::toHex( engine.dl() ) << std::endl;
                
                engine.cf( false );
                engine.ah( 0 );
            }
            
            void readSectors( const Machine & machine, Engine & engine )
            {
                uint8_t  driveNumber( engine.dl() );
                uint8_t  sectors(     engine.al() );
                uint8_t  cylinder(    engine.ch() );
                uint8_t  sector(      engine.cl() );
                uint8_t  head(        engine.dh() );
                uint64_t address(     0 );
                
                if( driveNumber != 0x00 )
                {
                    machine.ui().debug() << "[ ERROR ]> Reading from drive " << String::toHex( driveNumber ) << " is not supported" << std::endl;
                    
                    goto error;
                }
                
                address   = engine.es();
                address <<= 4;
                address  += engine.bx();
                
                {
                    uint64_t           lba( 0 );
                    const FAT::Image & img( machine.bootImage() );
                    uint16_t           hpc( img.mbr().headsPerCylinder() );
                    uint16_t           spt( img.mbr().sectorsPerTrack() );
                    
                    lba = ( ( ( numeric_cast< uint64_t >( cylinder ) * numeric_cast< uint64_t >( hpc ) ) + numeric_cast< uint64_t >( head ) ) * numeric_cast< uint64_t >( spt ) ) + ( numeric_cast< uint64_t >( sector ) - 1 );
                    
                    machine.ui().debug() << "Reading " << static_cast< unsigned int >( sectors ) << " sector" << ( ( sectors > 1 ) ? "s" : "" ) << " from drive " << String::toHex( driveNumber )
                                         << std::endl
                                         << "  - Cylinder:    " << String::toHex( cylinder )
                                         << std::endl
                                         << "  - Head:        " << String::toHex( head )
                                         << std::endl
                                         << "  - Sector:      " << String::toHex( sector )
                                         << std::endl
                                         << "  - LBA:         " << String::toHex( lba )
                                         << std::endl
                                         << "  - Destination: " << String::toHex( address ) << " (" << String::toHex( engine.es() ) << ":" << String::toHex( engine.bx() ) << ")"
                                         << std::endl;
                                         
                                         std::stringstream ss;
                                         ss << img.mbr();
                                         machine.ui().debug() << ss.str();
                }
                
                {
                    std::vector< uint8_t > bytes( machine.bootImage().read( sectors, cylinder, sector, head ) );
                    
                    if( bytes.size() == 0 )
                    {
                        machine.ui().debug() << "[ ERROR ]> No data received" << std::endl;
                        
                        goto error;
                    }
                    
                    machine.ui().debug() << "[ SUCCESS ]> Writing " << bytes.size() << " to memory" << std::endl;
                    
                    memcpy( reinterpret_cast< void * >( address ), &( bytes[ 0 ] ), bytes.size() );
                    
                    {
                        engine.cf( false );
                        engine.ah( 0 );
                        engine.al( sectors );
                    }
                }
                
                error:
                    
                    {
                        engine.cf( true );
                        engine.ah( 1 );
                        engine.al( 0 );
                    }
            }
        }
    }
}
