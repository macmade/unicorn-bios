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

#ifndef UB_ARGUMENTS_HPP
#define UB_ARGUMENTS_HPP

#include <memory>
#include <algorithm>
#include <string>
#include <vector>

namespace UB
{
    class Arguments
    {
        public:
            
            Arguments( int argc, const char * argv[] );
            Arguments( const Arguments & o );
            Arguments( Arguments && o ) noexcept;
            ~Arguments( void );
            
            Arguments & operator =( Arguments o );
            
            bool                    showHelp( void )               const;
            bool                    breakOnInterrupt( void )       const;
            bool                    breakOnInterruptReturn( void ) const;
            bool                    trap( void )                   const;
            bool                    debugVideo( void )             const;
            bool                    singleStep( void )             const;
            bool                    noUI( void )                   const;
            bool                    noColors( void )               const;
            size_t                  memory( void )                 const;
            std::string             bootImage( void )              const;
            std::vector< uint64_t > breakpoints( void )            const;
            
            friend void swap( Arguments & o1, Arguments & o2 );
            
        private:
            
            class IMPL;
            std::unique_ptr< IMPL > impl;
    };
}

#endif /* UB_ARGUMENTS_HPP */
