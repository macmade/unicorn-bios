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

#ifndef UB_STRING_STREAM_HPP
#define UB_STRING_STREAM_HPP

#include <memory>
#include <algorithm>
#include <ostream>
#include <string>
#include <ios>
#include <ostream>

namespace UB
{
    class StringStream
    {
        public:
            
            StringStream( void );
            StringStream( const StringStream & o );
            StringStream( StringStream && o ) noexcept;
            ~StringStream( void );
            
            StringStream & operator =( StringStream o );
            
            operator std::string()     const;
            std::string string( void ) const;
            
            StringStream & operator <<( const std::string & s );
            StringStream & operator <<( short v );
            StringStream & operator <<( unsigned short v );
            StringStream & operator <<( int v );
            StringStream & operator <<( unsigned int v );
            StringStream & operator <<( long v );
            StringStream & operator <<( unsigned long v );
            StringStream & operator <<( long long v );
            StringStream & operator <<( unsigned long long v );
            StringStream & operator <<( float v );
            StringStream & operator <<( double v );
            StringStream & operator <<( long double v );
            
            StringStream & operator <<( std::ostream & ( * f )( std::ostream & ) );
            StringStream & operator <<( std::ios_base & ( * f )( std::ios_base & ) );
            
            friend void swap( StringStream & o1, StringStream & o2 );
            
        private:
            
            class IMPL;
            std::unique_ptr< IMPL > impl;
    };
}

#endif /* UB_STRING_STREAM_HPP */
