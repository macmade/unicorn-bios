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

#include "UB/String.hpp"

namespace UB
{
    namespace String
    {
        std::vector< std::string > lines( const std::string & s )
        {
            std::vector< std::string > v;
            std::stringstream          ss( s );
            std::string                l;
            
            while( std::getline( ss, l, '\n' ) )
            {
                v.push_back( l );
            }
            
            return v;
        }
        
        std::string toUpper( const std::string & s )
        {
            std::string upper( s );
            
            std::transform( upper.begin(), upper.end(), upper.begin(), ::toupper );
            
            return upper;
        }
        
        std::string toLower( const std::string & s )
        {
            std::string lower( s );
            
            std::transform( lower.begin(), lower.end(), lower.begin(), ::tolower );
            
            return lower;
        }
    }
}
