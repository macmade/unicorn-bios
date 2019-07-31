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

#include "UB/BIOS/SystemServices.hpp"
#include "UB/Machine.hpp"
#include "UB/Engine.hpp"
#include "UB/String.hpp"

namespace UB
{
    namespace BIOS
    {
        namespace SystemServices
        {
            bool getMemoryMap( const Machine & machine, Engine & engine )
            {
                uint64_t                        destination( Engine::getAddress( engine.es(), engine.di() ) );
                uint32_t                        index( engine.ebx() );
                uint32_t                        size( engine.ecx() );
                uint32_t                        signature( engine.edx() );
                const MemoryMap               & map( machine.memoryMap() );
                std::vector< MemoryMap::Entry > entries( map.entries() );
                
                machine.ui().debug() << "Getting memory map:"
                                     << std::endl
                                     << "    - Continuation: " << String::toHex( index )
                                     << std::endl
                                     << "    - Destination:  " << String::toHex( destination ) << " (" << String::toHex( engine.es() ) << ":" << String::toHex( engine.di() ) << ")"
                                     << std::endl
                                     << "    - Buffer size:  " << String::toHex( size )
                                     << std::endl
                                     << "    - Signature:    " << String::toHex( signature )
                                     << std::endl;
                
                if( signature != 0x534D4150 )
                {
                    goto error;
                }
                
                if( map.entries().size() == 0 )
                {
                    goto error;
                }
                
                if( index >= map.entries().size() )
                {
                    goto error;
                }
                
                if( size < 0x14 )
                {
                    goto error;
                }
                
                {
                    MemoryMap::Entry entry( entries[ index ] );
                    std::string      type;
                    
                    if( entry.type() == MemoryMap::Entry::Type::Usable )
                    {
                        type = "Usable";
                    }
                    else if( entry.type() == MemoryMap::Entry::Type::Reserved )
                    {
                        type = "Reserved";
                    }
                    else if( entry.type() == MemoryMap::Entry::Type::ACPI )
                    {
                        type = "ACPI";
                    }
                    else
                    {
                        type = "Unknown";
                    }
                    
                    machine.ui().debug() << "Current entry: "
                                         << String::toHex( entry.base() )
                                         << " -> "
                                         << String::toHex( entry.end() )
                                         << " ("
                                         << type
                                         << ")"
                                         << std::endl;
                    
                    
                    {
                        std::array< uint32_t, 5 > data( entry.data() );
                        
                        engine.write( destination, reinterpret_cast< const uint8_t * >( data.data() ), data.size() );
                    }
                    
                    engine.cf( false );
                    engine.eax( 0x534D4150 );
                    engine.ecx( 0x00000014 );
                    
                    if( index == entries.size() - 1 )
                    {
                        engine.ebx( 0 );
                    }
                    else
                    {
                        engine.ebx( index + 1 );
                    }
                    
                    machine.ui().debug() << "[ SUCCESS ]> Wrote 20 bytes at "
                                         << String::toHex( destination )
                                         << " -> "
                                         << String::toHex( destination + 20 )
                                         << std::endl;
                }
                
                return true;
                
                error:
                    
                    engine.cf( true );
                    engine.eax( 0x534D4150 );
                    engine.ebx( 0x00000000 );
                    engine.ecx( 0x00000014 );
                    
                    return false;
            }
        }
    }
}
