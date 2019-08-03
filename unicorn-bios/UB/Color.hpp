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

#ifndef UB_COLOR_HPP
#define UB_COLOR_HPP

#include <memory>
#include <algorithm>

namespace UB
{
    class Color
    {
        public:
            
            static Color clear( void );
            static Color black( void );
            static Color red( void );
            static Color green( void );
            static Color yellow( void );
            static Color blue( void );
            static Color magenta( void );
            static Color cyan( void );
            static Color white( void );
            
            Color( const Color & o );
            Color( Color && o ) noexcept;
            ~Color( void );
            
            Color & operator =( Color o );
            
            int index( void ) const;
            
            friend void swap( Color & o1, Color & o2 );
            
        private:
            
            explicit Color( int index );
            
            class IMPL;
            std::unique_ptr< IMPL > impl;
    };
}

#endif /* UB_COLOR_HPP */
