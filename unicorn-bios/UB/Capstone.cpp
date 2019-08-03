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

#include "UB/Capstone.hpp"
#include "UB/String.hpp"
#include <sstream>
#include <iomanip>

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdocumentation"
#pragma clang diagnostic ignored "-Wnested-anon-types"
#endif
#include <capstone/capstone.h>
#ifdef __clang__
#pragma clang diagnostic pop
#endif

namespace UB
{
    namespace Capstone
    {
        std::vector< std::pair< std::string, std::string > > disassemble( const std::vector< uint8_t > & data, uint64_t org )
        {
            csh       handle;
            cs_insn * instruction;
            size_t    count;
            
            std::vector< std::pair< std::string, std::string > > v;
            
            if( cs_open( CS_ARCH_X86, CS_MODE_16, &handle ) != CS_ERR_OK )
            {
                return {};
            }
            
            count = cs_disasm( handle, &( data[ 0 ] ), data.size(), org, 0, &instruction );
            
            if( count == 0 )
            {
                cs_close( &handle );
                
                return {};
            }
            
            for( size_t i = 0; i < count; i++ )
            {
                v.push_back
                (
                    {
                        String::toHex( instruction[ i ].address ),
                        instruction[ i ].mnemonic + std::string( " " ) + instruction[ i ].op_str
                    }
                );
            }
            
            cs_free( instruction, count );
            cs_close( &handle );
            
            return v;
        }
        
        std::vector< std::pair< std::string, std::string > > instructions( const std::vector< uint8_t > & data, uint64_t org )
        {
            csh       handle;
            cs_insn * instruction;
            size_t    count;
            
            std::vector< std::pair< std::string, std::string > > v;
            
            if( cs_open( CS_ARCH_X86, CS_MODE_16, &handle ) != CS_ERR_OK )
            {
                return {};
            }
            
            count = cs_disasm( handle, &( data[ 0 ] ), data.size(), org, 0, &instruction );
            
            if( count == 0 )
            {
                cs_close( &handle );
                
                return {};
            }
            
            for( size_t i = 0; i < count; i++ )
            {
                std::stringstream ss;
                
                for( size_t j = 0; j < instruction[ i ].size; j++ )
                {
                    ss << std::hex
                       << std::uppercase
                       << std::setw( 2 )
                       << std::setfill( '0' )
                       << static_cast< unsigned int >( instruction[ i ].bytes[ j ] );
                }
                
                v.push_back
                (
                    {
                        String::toHex( instruction[ i ].address ),
                        ss.str()
                    }
                );
            }
            
            cs_free( instruction, count );
            cs_close( &handle );
            
            return v;
        }
    }
}
