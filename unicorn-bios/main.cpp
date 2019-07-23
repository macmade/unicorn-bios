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

#include <cstdlib>
#include <iostream>
#include "UB/Engine.hpp"

int main( int argc, const char * argv[] )
{
    ( void )argc;
    ( void )argv;
    
    {
        UB::Engine engine( 1024 * 1024 * 64 );
        
        engine.cx( 42 );
        engine.dx( 42 );
        
        std::cout << "CX: " << engine.cx() << std::endl;
        std::cout << "DX: " << engine.dx() << std::endl;
        
        engine.onInterrupt
        (
            42,
            []( uint32_t i, UB::Engine & e ) -> bool
            {
                ( void )e;
                
                std::cout << "Interrupt " << i << " called" << std::endl;
                
                return true;
            }
        );
        
        engine.write( 0, { 0x66, 0xFF, 0xC1, 0x66, 0xFF, 0xCA, 0xCD, 0x2A } );
        engine.start( 0 );
        
        std::cout << "CX: " << engine.cx() << std::endl;
        std::cout << "DX: " << engine.dx() << std::endl;
    }
    
    return EXIT_SUCCESS;
}
