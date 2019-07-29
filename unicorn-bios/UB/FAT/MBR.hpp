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

#ifndef UB_FAT_MBR_HPP
#define UB_FAT_MBR_HPP

#include <memory>
#include <algorithm>
#include <ostream>

namespace UB
{
    class BinaryStream;
    
    namespace FAT
    {
        class MBR
        {
            public:
                
                MBR( void );
                MBR( BinaryStream & stream );
                MBR( const MBR & o );
                MBR( MBR && o ) noexcept;
                ~MBR( void );
                
                MBR & operator =( MBR o );
                
                std::vector< uint8_t > data( void ) const;
                
                uint16_t bytesPerSector( void )        const;
                uint8_t  sectorsPerCluster( void )     const;
                uint16_t reservedSectors( void )       const;
                uint8_t  numberOfFATs( void )          const;
                uint16_t maxRootDirEntries( void )     const;
                uint16_t totalSectors( void )          const;
                uint8_t  mediaDescriptor( void )       const;
                uint16_t sectorsPerFAT( void )         const;
                uint16_t sectorsPerTrack( void )       const;
                uint16_t headsPerCylinder( void )      const;
                uint32_t hiddenSectors( void )         const;
                uint32_t lbaSectors( void )            const;
                uint8_t  driveNumber( void )           const;
                uint8_t  reserved( void )              const;
                uint8_t  extendedBootSignature( void ) const;
                uint32_t volumeSerialNumber( void )    const;
                uint16_t bootSignature( void )         const;
                
                friend void swap( MBR & o1, MBR & o2 );
                
                friend std::ostream & operator <<( std::ostream & os, const MBR & o );
                
            private:
                
                class IMPL;
                std::unique_ptr< IMPL > impl;
        };
    }
}

#endif /* UB_FAT_MBR_HPP */
