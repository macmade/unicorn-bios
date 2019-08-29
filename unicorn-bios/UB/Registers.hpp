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
            
            uint8_t  ah(  void ) const;
            uint8_t  al(  void ) const;
            uint16_t ax(  void ) const;
            uint32_t eax( void ) const;
            uint64_t rax( void ) const;

            uint8_t  bh(  void ) const;
            uint8_t  bl(  void ) const;
            uint16_t bx(  void ) const;
            uint32_t ebx( void ) const;
            uint64_t rbx( void ) const;

            uint8_t  ch(  void ) const;
            uint8_t  cl(  void ) const;
            uint16_t cx(  void ) const;
            uint32_t ecx( void ) const;
            uint64_t rcx( void ) const;

            uint8_t  dh(  void ) const;
            uint8_t  dl(  void ) const;
            uint16_t dx(  void ) const;
            uint32_t edx( void ) const;
            uint64_t rdx( void ) const;

            uint8_t  sil( void ) const;
            uint16_t si(  void ) const;
            uint32_t esi( void ) const;
            uint64_t rsi( void ) const;

            uint8_t  dil( void ) const;
            uint16_t di(  void ) const;
            uint32_t edi( void ) const;
            uint64_t rdi( void ) const;

            uint8_t  bpl( void ) const;
            uint16_t bp(  void ) const;
            uint32_t ebp( void ) const;
            uint64_t rbp( void ) const;

            uint8_t  spl( void ) const;
            uint16_t sp(  void ) const;
            uint32_t esp( void ) const;
            uint64_t rsp( void ) const;

            uint16_t ip(  void ) const;
            uint32_t eip( void ) const;
            uint64_t rip( void ) const;

            uint16_t cs( void ) const;
            uint16_t ds( void ) const;
            uint16_t es( void ) const;
            uint16_t fs( void ) const;
            uint16_t gs( void ) const;
            uint16_t ss( void ) const;

            uint32_t eflags( void ) const;

            uint8_t  r8b( void ) const;
            uint16_t r8w( void ) const;
            uint32_t r8d( void ) const;
            uint64_t r8(  void ) const;

            uint8_t  r9b( void ) const;
            uint16_t r9w( void ) const;
            uint32_t r9d( void ) const;
            uint64_t r9(  void ) const;

            uint8_t  r10b( void ) const;
            uint16_t r10w( void ) const;
            uint32_t r10d( void ) const;
            uint64_t r10(  void ) const;

            uint8_t  r11b( void ) const;
            uint16_t r11w( void ) const;
            uint32_t r11d( void ) const;
            uint64_t r11(  void ) const;

            uint8_t  r12b( void ) const;
            uint16_t r12w( void ) const;
            uint32_t r12d( void ) const;
            uint64_t r12(  void ) const;

            uint8_t  r13b( void ) const;
            uint16_t r13w( void ) const;
            uint32_t r13d( void ) const;
            uint64_t r13(  void ) const;

            uint8_t  r14b( void ) const;
            uint16_t r14w( void ) const;
            uint32_t r14d( void ) const;
            uint64_t r14(  void ) const;

            uint8_t  r15b( void ) const;
            uint16_t r15w( void ) const;
            uint32_t r15d( void ) const;
            uint64_t r15(  void ) const;
            
            bool cf( void ) const;
            
            friend void swap( Registers & o1, Registers & o2 );
            
        private:
            
            class IMPL;
            std::unique_ptr< IMPL > impl;
    };
}

#endif /* UB_REGISTERS_HPP */
