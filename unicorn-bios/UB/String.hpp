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

#ifndef UB_STRING_HPP
#define UB_STRING_HPP

#include <string>
#include <vector>
#include <sstream>
#include <iomanip>
#include <type_traits>

namespace UB
{
    namespace String
    {
        std::vector< std::string > lines( const std::string & s );
        
        std::string toUpper( const std::string & s );
        std::string toLower( const std::string & s );
        
        template< typename _T_ >
        _T_ fromHex( const std::string & s, typename std::enable_if< std::is_integral< _T_ >::value >::type * = 0 )
        {
            _T_               v( 0 );
            std::stringstream ss;
            
            ss << std::hex << s;
            ss >> v;
            
            return v;
        }
        
        template< typename _T_ >
        std::string toHex( _T_ v, typename std::enable_if< std::is_integral< _T_ >::value >::type * = 0 )
        {
            std::stringstream ss;
            
            ss << "0x"
               << std::hex
               << std::uppercase
               << std::setfill( '0' )
               << std::setw( sizeof( _T_ ) * 2 )
               << v;
            
            return ss.str();
        }
    }
}

#endif /* UB_STRING_HPP */
