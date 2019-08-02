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

#include "UB/Registers.hpp"
#include "UB/Engine.hpp"

namespace UB
{
    class Registers::IMPL
    {
        public:
            
            IMPL( void );
            IMPL( const Engine & engine );
            IMPL( const IMPL & o );
            ~IMPL( void );
            
            bool     _cf;
            uint8_t  _ah;
            uint8_t  _al;
            uint8_t  _bh;
            uint8_t  _bl;
            uint8_t  _ch;
            uint8_t  _cl;
            uint8_t  _dh;
            uint8_t  _dl;
            uint16_t _ax;
            uint16_t _bx;
            uint16_t _cx;
            uint16_t _dx;
            uint16_t _si;
            uint16_t _di;
            uint16_t _sp;
            uint16_t _bp;
            uint16_t _cs;
            uint16_t _ds;
            uint16_t _ss;
            uint16_t _es;
            uint16_t _fs;
            uint16_t _gs;
            uint16_t _ip;
            uint32_t _eax;
            uint32_t _ebx;
            uint32_t _ecx;
            uint32_t _edx;
            uint32_t _esi;
            uint32_t _edi;
            uint32_t _esp;
            uint32_t _ebp;
            uint32_t _eip;
            uint32_t _eflags;
    };

    Registers::Registers( void ):
        impl( std::make_unique< IMPL >() )
    {}

    Registers::Registers( const Engine & engine ):
        impl( std::make_unique< IMPL >( engine ) )
    {}

    Registers::Registers( const Registers & o ):
        impl( std::make_unique< IMPL >( *( o.impl ) ) )
    {}

    Registers::Registers( Registers && o ) noexcept:
        impl( std::move( o.impl ) )
    {}

    Registers::~Registers( void )
    {}

    Registers & Registers::operator =( Registers o )
    {
        swap( *( this ), o );
        
        return *( this );
    }
    
    bool Registers::cf( void ) const
    {
        return this->impl->_cf;
    }
    
    uint8_t Registers::ah( void ) const
    {
        return this->impl->_ah;
    }
    
    uint8_t Registers::al( void ) const
    {
        return this->impl->_al;
    }
    
    uint8_t Registers::bh( void ) const
    {
        return this->impl->_bh;
    }
    
    uint8_t Registers::bl( void ) const
    {
        return this->impl->_bl;
    }
    
    uint8_t Registers::ch( void ) const
    {
        return this->impl->_ch;
    }
    
    uint8_t Registers::cl( void ) const
    {
        return this->impl->_cl;
    }
    
    uint8_t Registers::dh( void ) const
    {
        return this->impl->_dh;
    }
    
    uint8_t Registers::dl( void ) const
    {
        return this->impl->_dl;
    }
    
    uint16_t Registers::ax( void ) const
    {
        return this->impl->_ax;
    }
    
    uint16_t Registers::bx( void ) const
    {
        return this->impl->_bx;
    }
    
    uint16_t Registers::cx( void ) const
    {
        return this->impl->_cx;
    }
    
    uint16_t Registers::dx( void ) const
    {
        return this->impl->_dx;
    }
    
    uint16_t Registers::si( void ) const
    {
        return this->impl->_si;
    }
    
    uint16_t Registers::di( void ) const
    {
        return this->impl->_di;
    }
    
    uint16_t Registers::sp( void ) const
    {
        return this->impl->_sp;
    }
    
    uint16_t Registers::bp( void ) const
    {
        return this->impl->_bp;
    }
    
    uint16_t Registers::cs( void ) const
    {
        return this->impl->_cs;
    }
    
    uint16_t Registers::ds( void ) const
    {
        return this->impl->_ds;
    }
    
    uint16_t Registers::ss( void ) const
    {
        return this->impl->_ss;
    }
    
    uint16_t Registers::es( void ) const
    {
        return this->impl->_es;
    }
    
    uint16_t Registers::fs( void ) const
    {
        return this->impl->_fs;
    }
    
    uint16_t Registers::gs( void ) const
    {
        return this->impl->_gs;
    }
    
    uint16_t Registers::ip( void ) const
    {
        return this->impl->_ip;
    }
    
    uint32_t Registers::eax( void ) const
    {
        return this->impl->_eax;
    }
    
    uint32_t Registers::ebx( void ) const
    {
        return this->impl->_ebx;
    }
    
    uint32_t Registers::ecx( void ) const
    {
        return this->impl->_ecx;
    }
    
    uint32_t Registers::edx( void ) const
    {
        return this->impl->_edx;
    }
    
    uint32_t Registers::esi( void ) const
    {
        return this->impl->_esi;
    }
    
    uint32_t Registers::edi( void ) const
    {
        return this->impl->_edi;
    }
    
    uint32_t Registers::esp( void ) const
    {
        return this->impl->_esp;
    }
    
    uint32_t Registers::ebp( void ) const
    {
        return this->impl->_ebp;
    }
    
    uint32_t Registers::eip( void ) const
    {
        return this->impl->_eip;
    }
    
    uint32_t Registers::eflags( void ) const
    {
        return this->impl->_eflags;
    }
    
    void swap( Registers & o1, Registers & o2 )
    {
        using std::swap;
        
        swap( o1.impl, o2.impl );
    }

    Registers::IMPL::IMPL( void ):
        _cf(     false ),
        _ah(     0 ),
        _al(     0 ),
        _bh(     0 ),
        _bl(     0 ),
        _ch(     0 ),
        _cl(     0 ),
        _dh(     0 ),
        _dl(     0 ),
        _ax(     0 ),
        _bx(     0 ),
        _cx(     0 ),
        _dx(     0 ),
        _si(     0 ),
        _di(     0 ),
        _sp(     0 ),
        _bp(     0 ),
        _cs(     0 ),
        _ds(     0 ),
        _ss(     0 ),
        _es(     0 ),
        _fs(     0 ),
        _gs(     0 ),
        _ip(     0 ),
        _eax(    0 ),
        _ebx(    0 ),
        _ecx(    0 ),
        _edx(    0 ),
        _esi(    0 ),
        _edi(    0 ),
        _esp(    0 ),
        _ebp(    0 ),
        _eip(    0 ),
        _eflags( 0 )
    {}
    
    Registers::IMPL::IMPL( const Engine & engine ):
        _cf(     engine.cf() ),
        _ah(     engine.ah() ),
        _al(     engine.al() ),
        _bh(     engine.bh() ),
        _bl(     engine.bl() ),
        _ch(     engine.ch() ),
        _cl(     engine.cl() ),
        _dh(     engine.dh() ),
        _dl(     engine.dl() ),
        _ax(     engine.ax() ),
        _bx(     engine.bx() ),
        _cx(     engine.cx() ),
        _dx(     engine.dx() ),
        _si(     engine.si() ),
        _di(     engine.di() ),
        _sp(     engine.sp() ),
        _bp(     engine.bp() ),
        _cs(     engine.cs() ),
        _ds(     engine.ds() ),
        _ss(     engine.ss() ),
        _es(     engine.es() ),
        _fs(     engine.fs() ),
        _gs(     engine.gs() ),
        _ip(     engine.ip() ),
        _eax(    engine.eax() ),
        _ebx(    engine.ebx() ),
        _ecx(    engine.ecx() ),
        _edx(    engine.edx() ),
        _esi(    engine.esi() ),
        _edi(    engine.edi() ),
        _esp(    engine.esp() ),
        _ebp(    engine.ebp() ),
        _eip(    engine.eip() ),
        _eflags( engine.eflags() )
    {}

    Registers::IMPL::IMPL( const IMPL & o ):
        _cf(     o._cf ),
        _ah(     o._ah ),
        _al(     o._al ),
        _bh(     o._bh ),
        _bl(     o._bl ),
        _ch(     o._ch ),
        _cl(     o._cl ),
        _dh(     o._dh ),
        _dl(     o._dl ),
        _ax(     o._ax ),
        _bx(     o._bx ),
        _cx(     o._cx ),
        _dx(     o._dx ),
        _si(     o._si ),
        _di(     o._di ),
        _sp(     o._sp ),
        _bp(     o._bp ),
        _cs(     o._cs ),
        _ds(     o._ds ),
        _ss(     o._ss ),
        _es(     o._es ),
        _fs(     o._fs ),
        _gs(     o._gs ),
        _ip(     o._ip ),
        _eax(    o._eax ),
        _ebx(    o._ebx ),
        _ecx(    o._ecx ),
        _edx(    o._edx ),
        _esi(    o._esi ),
        _edi(    o._edi ),
        _esp(    o._esp ),
        _ebp(    o._ebp ),
        _eip(    o._eip ),
        _eflags( o._eflags )
    {}

    Registers::IMPL::~IMPL( void )
    {}
}
