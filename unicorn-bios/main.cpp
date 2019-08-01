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

#include <iostream>
#include "UB/Arguments.hpp"
#include "UB/Machine.hpp"

static void showHelp( void );

int main( int argc, const char * argv[] )
{
    try
    {
        UB::Arguments args( argc, argv );
        
        if( args.showHelp() || args.bootImage().length() == 0 )
        {
            showHelp();
            
            return EXIT_SUCCESS;
        }
        
        {
            UB::Machine machine( args.memory() * 1024 * 1024, args.bootImage() );
            
            machine.breakOnInterrupt( args.breakOnInterrupt() );
            machine.breakOnInterruptReturn( args.breakOnInterruptReturn() );
            machine.trap( args.trap() );
            machine.debugVideo( args.debugVideo() );
            machine.singleStep( args.singleStep() );
            
            if( args.noUI() )
            {
                machine.run( UB::UI::Mode::Standard );
            }
            else
            {
                machine.run( UB::UI::Mode::Interactive );
            }
        }
        
        return EXIT_SUCCESS;
    }
    catch( const std::exception & e )
    {
        std::cerr << "Error: " << e.what() << std::endl;
        
        return EXIT_FAILURE;
    }
    catch( ... )
    {
        std::cerr << "Unknown error" << std::endl;
        
        return EXIT_FAILURE;
    }
}

static void showHelp( void )
{
    std::cout << "Usage: unicorn-bios [OPTIONS] BOOT_IMG"
              << std::endl
              << std::endl
              << "Options:"
              << std::endl
              << std::endl
              << "    --help   / -h:  Displays help."
              << std::endl
              << "    --memory / -m:  The amount of memory to allocate for the virtual machine"
              << std::endl
              << "                    (in megabytes). Defaults to 64MB, minimum 2MB."
              << std::endl
              << "    --break-int:    Breaks on interrupt calls."
              << std::endl
              << "    --break-iret:   Breaks on interrupt returns."
              << std::endl
              << "    --trap:         Raises a trap when breaking."
              << std::endl
              << "    --debug-video:  Turns on debug output for video services."
              << std::endl
              << "    --single-step:  Breaks on every instruction."
              << std::endl
              << "    --no-ui:        Don't start the user interface (only output will be displayed to stdout)."
              << std::endl;
}
