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

#include "UB/Interrupts.hpp"
#include "UB/Engine.hpp"
#include "UB/Machine.hpp"
#include "UB/BIOS/Video.hpp"
#include "UB/BIOS/Disk.hpp"
#include "UB/BIOS/Keyboard.hpp"

namespace UB
{
    namespace Interrupts
    {
        bool int0x05( const Machine & machine, Engine & engine )
        {
            ( void )machine;
            ( void )engine;
            
            return false;
        }
        
        bool int0x10( const Machine & machine, Engine & engine )
        {
            switch( engine.ah() )
            {
                case 0x0E: BIOS::Video::ttyOutput( machine, engine ); return true;
                default:   break;
            }
            
            return false;
        }
        
        bool int0x11( const Machine & machine, Engine & engine )
        {
            ( void )machine;
            ( void )engine;
            
            return false;
        }
        
        bool int0x12( const Machine & machine, Engine & engine )
        {
            ( void )machine;
            ( void )engine;
            
            return false;
        }
        
        bool int0x13( const Machine & machine, Engine & engine )
        {
            switch( engine.ah() )
            {
                case 0x00: BIOS::Disk::reset( machine, engine );       return true;
                case 0x02: BIOS::Disk::readSectors( machine, engine ); return true;
                default:   break;
            }
            
            return false;
        }
        
        bool int0x14( const Machine & machine, Engine & engine )
        {
            ( void )machine;
            ( void )engine;
            
            return false;
        }
        
        bool int0x15( const Machine & machine, Engine & engine )
        {
            ( void )machine;
            ( void )engine;
            
            return false;
        }
        
        bool int0x16( const Machine & machine, Engine & engine )
        {
            switch( engine.ah() )
            {
                case 0x00: BIOS::Keyboard::readKey( machine, engine ); return true;
                default:   break;
            }
            
            return false;
        }
        
        bool int0x17( const Machine & machine, Engine & engine )
        {
            ( void )machine;
            ( void )engine;
            
            return false;
        }
        
        bool int0x18( const Machine & machine, Engine & engine )
        {
            ( void )machine;
            
            machine.ui().debug() << "Stopping emulation" << std::endl;
            engine.stop();
            
            return true;
        }
        
        bool int0x19( const Machine & machine, Engine & engine )
        {
            ( void )machine;
            
            machine.ui().debug() << "Stopping emulation" << std::endl;
            engine.stop();
            
            return true;
        }
        
        bool int0x1A( const Machine & machine, Engine & engine )
        {
            ( void )machine;
            ( void )engine;
            
            return false;
        }
    }
}
