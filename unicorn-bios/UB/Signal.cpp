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

#include "UB/Signal.hpp"
#include <mutex>
#include <map>
#include <vector>

static std::recursive_mutex                                          * rmtx;
static std::map< int,  std::vector< std::function< void( int ) > > > * handlers;

static void handle( int sig );

namespace UB
{
    namespace Signal
    {
        void handle( int sig, const std::function< void( int ) > & handler )
        {
            static std::once_flag once;
            
            std::call_once
            (
                once,
                []
                {
                    rmtx     = new std::recursive_mutex();
                    handlers = new std::map< int, std::vector< std::function< void( int ) > > >();
                }
            );
            
            {
                std::lock_guard< std::recursive_mutex > l( *( rmtx ) );
                
                handlers->operator[]( sig ).push_back( handler );
                
                signal( SIGINT, ::handle ); 
            }
        }
    }
}

static void handle( int sig )
{
    std::lock_guard< std::recursive_mutex > l( *( rmtx ) );
    
    for( const auto & f: handlers->operator[]( sig ) )
    {
        f( sig );
    }
}
