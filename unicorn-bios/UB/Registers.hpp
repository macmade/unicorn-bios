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

#ifndef UB_REGISTERS_HPP
#define UB_REGISTERS_HPP

#include <memory>
#include <algorithm>
#include <cstdint>

namespace UB
{
    class Engine;
    
    class Registers
    {
        public:
            
            Registers( void );
            Registers( const Engine & engine );
            Registers( const Registers & o );
            Registers( Registers && o ) noexcept;
            ~Registers( void );
            
            Registers & operator =( Registers o );
            
            bool cf( void ) const;
            
            uint8_t ah( void ) const;
            uint8_t al( void ) const;
            uint8_t bh( void ) const;
            uint8_t bl( void ) const;
            uint8_t ch( void ) const;
            uint8_t cl( void ) const;
            uint8_t dh( void ) const;
            uint8_t dl( void ) const;
            
            uint16_t ax( void ) const;
            uint16_t bx( void ) const;
            uint16_t cx( void ) const;
            uint16_t dx( void ) const;
            uint16_t si( void ) const;
            uint16_t di( void ) const;
            uint16_t sp( void ) const;
            uint16_t bp( void ) const;
            uint16_t cs( void ) const;
            uint16_t ds( void ) const;
            uint16_t ss( void ) const;
            uint16_t es( void ) const;
            uint16_t fs( void ) const;
            uint16_t gs( void ) const;
            uint16_t ip( void ) const;
            
            uint32_t eax( void )    const;
            uint32_t ebx( void )    const;
            uint32_t ecx( void )    const;
            uint32_t edx( void )    const;
            uint32_t esi( void )    const;
            uint32_t edi( void )    const;
            uint32_t esp( void )    const;
            uint32_t ebp( void )    const;
            uint32_t eip( void )    const;
            uint32_t eflags( void ) const;
            
            friend void swap( Registers & o1, Registers & o2 );
            
        private:
            
            class IMPL;
            std::unique_ptr< IMPL > impl;
    };
}

#endif /* UB_REGISTERS_HPP */
