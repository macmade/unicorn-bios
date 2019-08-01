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

#ifndef UB_MACHINE_HPP
#define UB_MACHINE_HPP

#include <memory>
#include <algorithm>
#include "UB/FAT/Image.hpp"
#include "UB/BIOS/MemoryMap.hpp"
#include "UB/UI.hpp"

namespace UB
{
    class Machine
    {
        public:
            
            Machine( size_t memory, const FAT::Image & fat );
            Machine( const Machine & o );
            Machine( Machine && o ) noexcept;
            ~Machine( void );
            
            Machine & operator =( Machine o );
            
            const FAT::Image      & bootImage( void ) const;
            const BIOS::MemoryMap & memoryMap( void ) const;
            
            UI & ui( void ) const;
            
            void run( UI::Mode mode );
            
            bool breakOnInterrupt( void )       const;
            bool breakOnInterruptReturn( void ) const;
            bool trap( void )                   const;
            bool debugVideo( void )             const;
            bool singleStep( void )             const;
            
            void breakOnInterrupt( bool value );
            void breakOnInterruptReturn( bool value );
            void trap( bool value );
            void debugVideo( bool value );
            void singleStep( bool value );
            
            void addBreakpoint(    uint64_t address );
            void removeBreakpoint( uint64_t address );
            
            friend void swap( Machine & o1, Machine & o2 );
            
        private:
            
            class IMPL;
            std::unique_ptr< IMPL > impl;
    };
}

#endif /* UB_MACHINE_HPP */
