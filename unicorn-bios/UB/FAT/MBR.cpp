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

#include "UB/FAT/MBR.hpp"
#include "UB/BinaryStream.hpp"
#include "UB/Casts.hpp"
#include "UB/String.hpp"
#include <array>

namespace UB
{
    namespace FAT
    {
        class MBR::IMPL
        {
            public:
                
                IMPL( void );
                IMPL( BinaryStream & stream );
                IMPL( const IMPL & o );
                
                std::vector< uint8_t >     _data;
                std::array< uint8_t, 3 >   _jmp;
                std::array< uint8_t, 8 >   _oemID;
                uint16_t                   _bytesPerSector;
                uint8_t                    _sectorsPerCluster;
                uint16_t                   _reservedSectors;
                uint8_t                    _numberOfFATs;
                uint16_t                   _maxRootDirEntries;
                uint16_t                   _totalSectors;
                uint8_t                    _mediaDescriptor;
                uint16_t                   _sectorsPerFAT;
                uint16_t                   _sectorsPerTrack;
                uint16_t                   _headsPerCylinder;
                uint32_t                   _hiddenSectors;
                uint32_t                   _lbaSectors;
                uint8_t                    _driveNumber;
                uint8_t                    _reserved;
                uint8_t                    _extendedBootSignature;
                uint32_t                   _volumeSerialNumber;
                std::array< uint8_t, 11 >  _volumeLabel;
                std::array< uint8_t, 8 >   _fileSystem;
                std::array< uint8_t, 448 > _bootCode;
                uint16_t                   _bootSignature;
        };
        
        MBR::MBR( void ):
            impl( std::make_unique< IMPL >() )
        {}
        
        MBR::MBR( BinaryStream & stream ):
            impl( std::make_unique< IMPL >( stream ) )
        {}
        
        MBR::MBR( const MBR & o ):
            impl( std::make_unique< IMPL >( *( o.impl ) ) )
        {}
        
        MBR::MBR( MBR && o ) noexcept:
            impl( std::move( o.impl ) )
        {}
        
        MBR::~MBR( void )
        {}
        
        MBR & MBR::operator =( MBR o )
        {
            swap( *( this ), o );
            
            return *( this );
        }
        
        std::vector< uint8_t > MBR::data( void ) const
        {
            return this->impl->_data;
        }
        
        uint16_t MBR::bytesPerSector( void ) const
        {
            return this->impl->_bytesPerSector;
        }
        
        uint8_t MBR::sectorsPerCluster( void ) const
        {
            return this->impl->_sectorsPerCluster;
        }
        
        uint16_t MBR::reservedSectors( void ) const
        {
            return this->impl->_reservedSectors;
        }
        
        uint8_t MBR::numberOfFATs( void ) const
        {
            return this->impl->_numberOfFATs;
        }
        
        uint16_t MBR::maxRootDirEntries( void ) const
        {
            return this->impl->_maxRootDirEntries;
        }
        
        uint16_t MBR::totalSectors( void ) const
        {
            return this->impl->_totalSectors;
        }
        
        uint8_t MBR::mediaDescriptor( void ) const
        {
            return this->impl->_mediaDescriptor;
        }
        
        uint16_t MBR::sectorsPerFAT( void ) const
        {
            return this->impl->_sectorsPerFAT;
        }
        
        uint16_t MBR::sectorsPerTrack( void ) const
        {
            return this->impl->_sectorsPerTrack;
        }
        
        uint16_t MBR::headsPerCylinder( void ) const
        {
            return this->impl->_headsPerCylinder;
        }
        
        uint32_t MBR::hiddenSectors( void ) const
        {
            return this->impl->_hiddenSectors;
        }
        
        uint32_t MBR::lbaSectors( void ) const
        {
            return this->impl->_lbaSectors;
        }
        
        uint8_t MBR::driveNumber( void ) const
        {
            return this->impl->_driveNumber;
        }
        
        uint8_t MBR::reserved( void ) const
        {
            return this->impl->_reserved;
        }
        
        uint8_t MBR::extendedBootSignature( void ) const
        {
            return this->impl->_extendedBootSignature;
        }
        
        uint32_t MBR::volumeSerialNumber( void ) const
        {
            return this->impl->_volumeSerialNumber;
        }
        
        uint16_t MBR::bootSignature( void ) const
        {
            return this->impl->_bootSignature;
        }
        
        bool MBR::isValid( void ) const
        {
            if
            (
                   this->impl->_bytesPerSector != 512
                && this->impl->_bytesPerSector != 1024
                && this->impl->_bytesPerSector != 2048
                && this->impl->_bytesPerSector != 4096
            )
            {
                return false;
            }
            
            if
            (
                   this->impl->_sectorsPerCluster != 1
                && this->impl->_sectorsPerCluster != 2
                && this->impl->_sectorsPerCluster != 4
                && this->impl->_sectorsPerCluster != 8
                && this->impl->_sectorsPerCluster != 16
                && this->impl->_sectorsPerCluster != 32
                && this->impl->_sectorsPerCluster != 64
                && this->impl->_sectorsPerCluster != 128
            )
            {
                return false;
            }
            
            return true;
        }
        
        void swap( MBR & o1, MBR & o2 )
        {
            using std::swap;
            
            swap( o1.impl, o2.impl );
        }
        
        std::ostream & operator <<( std::ostream & os, const MBR & o )
        {
            std::string oemID(       reinterpret_cast< char * >( o.impl->_oemID.data() ),       o.impl->_oemID.size() );
            std::string volumeLabel( reinterpret_cast< char * >( o.impl->_volumeLabel.data() ), o.impl->_volumeLabel.size() );
            std::string fileSystem(  reinterpret_cast< char * >( o.impl->_fileSystem.data() ),  o.impl->_fileSystem.size() );
            
            os << "{"                                                                                      << std::endl
               << "    OEM ID:                  " << oemID                                                 << std::endl
               << "    Bytes per sector:        " << o.impl->_bytesPerSector                               << std::endl
               << "    Sectors per cluster:     " << static_cast< uint16_t >( o.impl->_sectorsPerCluster ) << std::endl
               << "    Reserved sectors:        " << o.impl->_reservedSectors                              << std::endl
               << "    Number of FATs:          " << static_cast< uint16_t >( o.impl->_numberOfFATs )      << std::endl
               << "    Max root dir entries:    " << o.impl->_maxRootDirEntries                            << std::endl
               << "    Total sectors:           " << o.impl->_totalSectors                                 << std::endl
               << "    Media descriptor:        " << String::toHex( o.impl->_mediaDescriptor )             << std::endl
               << "    Sectors per FAT:         " << o.impl->_sectorsPerFAT                                << std::endl
               << "    Sectors per track:       " << o.impl->_sectorsPerTrack                              << std::endl
               << "    Heads per cylinder:      " << o.impl->_headsPerCylinder                             << std::endl
               << "    Hidden sectors:          " << o.impl->_hiddenSectors                                << std::endl
               << "    LBA sectors:             " << o.impl->_lbaSectors                                   << std::endl
               << "    Drive number:            " << static_cast< uint16_t >( o.impl->_driveNumber )       << std::endl
               << "    Reserved:                " << String::toHex( o.impl->_reserved )                    << std::endl
               << "    Extended boot signature: " << String::toHex( o.impl->_extendedBootSignature )       << std::endl
               << "    Volume serial number:    " << String::toHex( o.impl->_volumeSerialNumber )          << std::endl
               << "    Volume label:            " << volumeLabel                                           << std::endl
               << "    Filesystem:              " << fileSystem                                            << std::endl
               << "    Boot signature:          " << String::toHex( o.impl->_bootSignature )               << std::endl
               << "}";
            
            return os;
        }
        
        MBR::IMPL::IMPL( void ):
            _bytesPerSector(        0 ),
            _sectorsPerCluster(     0 ),
            _reservedSectors(       0 ),
            _numberOfFATs(          0 ),
            _maxRootDirEntries(     0 ),
            _totalSectors(          0 ),
            _mediaDescriptor(       0 ),
            _sectorsPerFAT(         0 ),
            _sectorsPerTrack(       0 ),
            _headsPerCylinder(      0 ),
            _hiddenSectors(         0 ),
            _lbaSectors(            0 ),
            _driveNumber(           0 ),
            _reserved(              0 ),
            _extendedBootSignature( 0 ),
            _volumeSerialNumber(    0 ),
            _bootSignature(         0 )
        {
            memset( this->_jmp.data(),         0, this->_jmp.size() );
            memset( this->_oemID.data(),       0, this->_oemID.size() );
            memset( this->_volumeLabel.data(), 0, this->_volumeLabel.size() );
            memset( this->_fileSystem.data(),  0, this->_fileSystem.size() );
            memset( this->_bootCode.data(),    0, this->_bootCode.size() );
        }
        
        MBR::IMPL::IMPL( BinaryStream & stream ):
            IMPL()
        {
            {
                size_t pos( stream.tell() );
                
                this->_data = stream.read( 512 );
                
                stream.seek( numeric_cast< ssize_t >( pos ), BinaryStream::SeekDirection::Begin );
            }
            
            stream.read( this->_jmp.data(),   this->_jmp.size() );
            stream.read( this->_oemID.data(), this->_oemID.size() );
            
            this->_bytesPerSector        = stream.readLittleEndianUInt16();
            this->_sectorsPerCluster     = stream.readUInt8();
            this->_reservedSectors       = stream.readLittleEndianUInt16();
            this->_numberOfFATs          = stream.readUInt8();
            this->_maxRootDirEntries     = stream.readLittleEndianUInt16();
            this->_totalSectors          = stream.readLittleEndianUInt16();
            this->_mediaDescriptor       = stream.readUInt8();
            this->_sectorsPerFAT         = stream.readLittleEndianUInt16();
            this->_sectorsPerTrack       = stream.readLittleEndianUInt16();
            this->_headsPerCylinder      = stream.readLittleEndianUInt16();
            this->_hiddenSectors         = stream.readLittleEndianUInt32();
            this->_lbaSectors            = stream.readLittleEndianUInt32();
            this->_driveNumber           = stream.readUInt8();
            this->_reserved              = stream.readUInt8();
            this->_extendedBootSignature = stream.readUInt8();
            this->_volumeSerialNumber    = stream.readLittleEndianUInt32();
            
            stream.read( this->_volumeLabel.data(), this->_volumeLabel.size() );
            stream.read( this->_fileSystem.data(),  this->_fileSystem.size() );
            stream.read( this->_bootCode.data(),    this->_bootCode.size() );
            
            this->_bootSignature = stream.readLittleEndianUInt16();
        }
        
        MBR::IMPL::IMPL( const IMPL & o ):
            _data(                  o._data ),
            _jmp(                   o._jmp ),
            _oemID(                 o._oemID ),
            _bytesPerSector(        o._bytesPerSector ),
            _sectorsPerCluster(     o._sectorsPerCluster ),
            _reservedSectors(       o._reservedSectors ),
            _numberOfFATs(          o._numberOfFATs ),
            _maxRootDirEntries(     o._maxRootDirEntries ),
            _totalSectors(          o._totalSectors ),
            _mediaDescriptor(       o._mediaDescriptor ),
            _sectorsPerFAT(         o._sectorsPerFAT ),
            _sectorsPerTrack(       o._sectorsPerTrack ),
            _headsPerCylinder(      o._headsPerCylinder ),
            _hiddenSectors(         o._hiddenSectors ),
            _lbaSectors(            o._lbaSectors ),
            _driveNumber(           o._driveNumber ),
            _reserved(              o._reserved ),
            _extendedBootSignature( o._extendedBootSignature ),
            _volumeSerialNumber(    o._volumeSerialNumber ),
            _volumeLabel(           o._volumeLabel ),
            _fileSystem(            o._fileSystem ),
            _bootCode(              o._bootCode ),
            _bootSignature(         o._bootSignature )
        {}
    }
}
