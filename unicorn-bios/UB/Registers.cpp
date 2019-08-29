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
            
            uint8_t  _ah;
            uint8_t  _al;
            uint16_t _ax;
            uint32_t _eax;
            uint64_t _rax;
            uint8_t  _bh;
            uint8_t  _bl;
            uint16_t _bx;
            uint32_t _ebx;
            uint64_t _rbx;
            uint8_t  _ch;
            uint8_t  _cl;
            uint16_t _cx;
            uint32_t _ecx;
            uint64_t _rcx;
            uint8_t  _dh;
            uint8_t  _dl;
            uint16_t _dx;
            uint32_t _edx;
            uint64_t _rdx;
            uint8_t  _sil;
            uint16_t _si;
            uint32_t _esi;
            uint64_t _rsi;
            uint8_t  _dil;
            uint16_t _di;
            uint32_t _edi;
            uint64_t _rdi;
            uint8_t  _bpl;
            uint16_t _bp;
            uint32_t _ebp;
            uint64_t _rbp;
            uint8_t  _spl;
            uint16_t _sp;
            uint32_t _esp;
            uint64_t _rsp;
            uint16_t _ip;
            uint32_t _eip;
            uint64_t _rip;
            uint16_t _cs;
            uint16_t _ds;
            uint16_t _es;
            uint16_t _fs;
            uint16_t _gs;
            uint16_t _ss;
            uint32_t _eflags;
            uint8_t  _r8b;
            uint16_t _r8w;
            uint32_t _r8d;
            uint64_t _r8;
            uint8_t  _r9b;
            uint16_t _r9w;
            uint32_t _r9d;
            uint64_t _r9;
            uint8_t  _r10b;
            uint16_t _r10w;
            uint32_t _r10d;
            uint64_t _r10;
            uint8_t  _r11b;
            uint16_t _r11w;
            uint32_t _r11d;
            uint64_t _r11;
            uint8_t  _r12b;
            uint16_t _r12w;
            uint32_t _r12d;
            uint64_t _r12;
            uint8_t  _r13b;
            uint16_t _r13w;
            uint32_t _r13d;
            uint64_t _r13;
            uint8_t  _r14b;
            uint16_t _r14w;
            uint32_t _r14d;
            uint64_t _r14;
            uint8_t  _r15b;
            uint16_t _r15w;
            uint32_t _r15d;
            uint64_t _r15;
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
    
    uint8_t Registers::ah( void ) const
    {
        return this->impl->_ah;
    }

    uint8_t Registers::al( void ) const
    {
        return this->impl->_al;
    }

    uint16_t Registers::ax( void ) const
    {
        return this->impl->_ax;
    }

    uint32_t Registers::eax( void ) const
    {
        return this->impl->_eax;
    }

    uint64_t Registers::rax( void ) const
    {
        return this->impl->_rax;
    }

    uint8_t Registers::bh( void ) const
    {
        return this->impl->_bh;
    }

    uint8_t Registers::bl( void ) const
    {
        return this->impl->_bl;
    }

    uint16_t Registers::bx( void ) const
    {
        return this->impl->_bx;
    }

    uint32_t Registers::ebx( void ) const
    {
        return this->impl->_ebx;
    }

    uint64_t Registers::rbx( void ) const
    {
        return this->impl->_rbx;
    }

    uint8_t Registers::ch( void ) const
    {
        return this->impl->_ch;
    }

    uint8_t Registers::cl( void ) const
    {
        return this->impl->_cl;
    }

    uint16_t Registers::cx( void ) const
    {
        return this->impl->_cx;
    }

    uint32_t Registers::ecx( void ) const
    {
        return this->impl->_ecx;
    }

    uint64_t Registers::rcx( void ) const
    {
        return this->impl->_rcx;
    }

    uint8_t Registers::dh( void ) const
    {
        return this->impl->_dh;
    }

    uint8_t Registers::dl( void ) const
    {
        return this->impl->_dl;
    }

    uint16_t Registers::dx( void ) const
    {
        return this->impl->_dx;
    }

    uint32_t Registers::edx( void ) const
    {
        return this->impl->_edx;
    }

    uint64_t Registers::rdx( void ) const
    {
        return this->impl->_rdx;
    }

    uint8_t Registers::sil( void ) const
    {
        return this->impl->_sil;
    }

    uint16_t Registers::si( void ) const
    {
        return this->impl->_si;
    }

    uint32_t Registers::esi( void ) const
    {
        return this->impl->_esi;
    }

    uint64_t Registers::rsi( void ) const
    {
        return this->impl->_rsi;
    }

    uint8_t Registers::dil( void ) const
    {
        return this->impl->_dil;
    }

    uint16_t Registers::di( void ) const
    {
        return this->impl->_di;
    }

    uint32_t Registers::edi( void ) const
    {
        return this->impl->_edi;
    }

    uint64_t Registers::rdi( void ) const
    {
        return this->impl->_rdi;
    }

    uint8_t Registers::bpl( void ) const
    {
        return this->impl->_bpl;
    }

    uint16_t Registers::bp( void ) const
    {
        return this->impl->_bp;
    }

    uint32_t Registers::ebp( void ) const
    {
        return this->impl->_ebp;
    }

    uint64_t Registers::rbp( void ) const
    {
        return this->impl->_rbp;
    }

    uint8_t Registers::spl( void ) const
    {
        return this->impl->_spl;
    }

    uint16_t Registers::sp( void ) const
    {
        return this->impl->_sp;
    }

    uint32_t Registers::esp( void ) const
    {
        return this->impl->_esp;
    }

    uint64_t Registers::rsp( void ) const
    {
        return this->impl->_rsp;
    }

    uint16_t Registers::ip( void ) const
    {
        return this->impl->_ip;
    }

    uint32_t Registers::eip( void ) const
    {
        return this->impl->_eip;
    }

    uint64_t Registers::rip( void ) const
    {
        return this->impl->_rip;
    }

    uint16_t Registers::cs( void ) const
    {
        return this->impl->_cs;
    }

    uint16_t Registers::ds( void ) const
    {
        return this->impl->_ds;
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

    uint16_t Registers::ss( void ) const
    {
        return this->impl->_ss;
    }

    uint32_t Registers::eflags( void ) const
    {
        return this->impl->_eflags;
    }

    uint8_t Registers::r8b( void ) const
    {
        return this->impl->_r8b;
    }

    uint16_t Registers::r8w( void ) const
    {
        return this->impl->_r8w;
    }

    uint32_t Registers::r8d( void ) const
    {
        return this->impl->_r8d;
    }

    uint64_t Registers::r8( void ) const
    {
        return this->impl->_r8;
    }

    uint8_t Registers::r9b( void ) const
    {
        return this->impl->_r9b;
    }

    uint16_t Registers::r9w( void ) const
    {
        return this->impl->_r9w;
    }

    uint32_t Registers::r9d( void ) const
    {
        return this->impl->_r9d;
    }

    uint64_t Registers::r9( void ) const
    {
        return this->impl->_r9;
    }

    uint8_t Registers::r10b( void ) const
    {
        return this->impl->_r10b;
    }

    uint16_t Registers::r10w( void ) const
    {
        return this->impl->_r10w;
    }

    uint32_t Registers::r10d( void ) const
    {
        return this->impl->_r10d;
    }

    uint64_t Registers::r10( void ) const
    {
        return this->impl->_r10;
    }

    uint8_t Registers::r11b( void ) const
    {
        return this->impl->_r11b;
    }

    uint16_t Registers::r11w( void ) const
    {
        return this->impl->_r11w;
    }

    uint32_t Registers::r11d( void ) const
    {
        return this->impl->_r11d;
    }

    uint64_t Registers::r11( void ) const
    {
        return this->impl->_r11;
    }

    uint8_t Registers::r12b( void ) const
    {
        return this->impl->_r12b;
    }

    uint16_t Registers::r12w( void ) const
    {
        return this->impl->_r12w;
    }

    uint32_t Registers::r12d( void ) const
    {
        return this->impl->_r12d;
    }

    uint64_t Registers::r12( void ) const
    {
        return this->impl->_r12;
    }

    uint8_t Registers::r13b( void ) const
    {
        return this->impl->_r13b;
    }

    uint16_t Registers::r13w( void ) const
    {
        return this->impl->_r13w;
    }

    uint32_t Registers::r13d( void ) const
    {
        return this->impl->_r13d;
    }

    uint64_t Registers::r13( void ) const
    {
        return this->impl->_r13;
    }

    uint8_t Registers::r14b( void ) const
    {
        return this->impl->_r14b;
    }

    uint16_t Registers::r14w( void ) const
    {
        return this->impl->_r14w;
    }

    uint32_t Registers::r14d( void ) const
    {
        return this->impl->_r14d;
    }

    uint64_t Registers::r14( void ) const
    {
        return this->impl->_r14;
    }

    uint8_t Registers::r15b( void ) const
    {
        return this->impl->_r15b;
    }

    uint16_t Registers::r15w( void ) const
    {
        return this->impl->_r15w;
    }

    uint32_t Registers::r15d( void ) const
    {
        return this->impl->_r15d;
    }

    uint64_t Registers::r15( void ) const
    {
        return this->impl->_r15;
    }

    void swap( Registers & o1, Registers & o2 )
    {
        using std::swap;
        
        swap( o1.impl, o2.impl );
    }

    Registers::IMPL::IMPL( void ):
        _ah(     0 ),
        _al(     0 ),
        _ax(     0 ),
        _eax(    0 ),
        _rax(    0 ),
        _bh(     0 ),
        _bl(     0 ),
        _bx(     0 ),
        _ebx(    0 ),
        _rbx(    0 ),
        _ch(     0 ),
        _cl(     0 ),
        _cx(     0 ),
        _ecx(    0 ),
        _rcx(    0 ),
        _dh(     0 ),
        _dl(     0 ),
        _dx(     0 ),
        _edx(    0 ),
        _rdx(    0 ),
        _sil(    0 ),
        _si(     0 ),
        _esi(    0 ),
        _rsi(    0 ),
        _dil(    0 ),
        _di(     0 ),
        _edi(    0 ),
        _rdi(    0 ),
        _bpl(    0 ),
        _bp(     0 ),
        _ebp(    0 ),
        _rbp(    0 ),
        _spl(    0 ),
        _sp(     0 ),
        _esp(    0 ),
        _rsp(    0 ),
        _ip(     0 ),
        _eip(    0 ),
        _rip(    0 ),
        _cs(     0 ),
        _ds(     0 ),
        _es(     0 ),
        _fs(     0 ),
        _gs(     0 ),
        _ss(     0 ),
        _eflags( 0 ),
        _r8b(    0 ),
        _r8w(    0 ),
        _r8d(    0 ),
        _r8(     0 ),
        _r9b(    0 ),
        _r9w(    0 ),
        _r9d(    0 ),
        _r9(     0 ),
        _r10b(   0 ),
        _r10w(   0 ),
        _r10d(   0 ),
        _r10(    0 ),
        _r11b(   0 ),
        _r11w(   0 ),
        _r11d(   0 ),
        _r11(    0 ),
        _r12b(   0 ),
        _r12w(   0 ),
        _r12d(   0 ),
        _r12(    0 ),
        _r13b(   0 ),
        _r13w(   0 ),
        _r13d(   0 ),
        _r13(    0 ),
        _r14b(   0 ),
        _r14w(   0 ),
        _r14d(   0 ),
        _r14(    0 ),
        _r15b(   0 ),
        _r15w(   0 ),
        _r15d(   0 ),
        _r15(    0 )
    {}
    
    Registers::IMPL::IMPL( const Engine & engine ):
        _ah(     engine.ah() ),
        _al(     engine.al() ),
        _ax(     engine.ax() ),
        _eax(    engine.eax() ),
        _rax(    engine.rax() ),
        _bh(     engine.bh() ),
        _bl(     engine.bl() ),
        _bx(     engine.bx() ),
        _ebx(    engine.ebx() ),
        _rbx(    engine.rbx() ),
        _ch(     engine.ch() ),
        _cl(     engine.cl() ),
        _cx(     engine.cx() ),
        _ecx(    engine.ecx() ),
        _rcx(    engine.rcx() ),
        _dh(     engine.dh() ),
        _dl(     engine.dl() ),
        _dx(     engine.dx() ),
        _edx(    engine.edx() ),
        _rdx(    engine.rdx() ),
        _sil(    engine.sil() ),
        _si(     engine.si() ),
        _esi(    engine.esi() ),
        _rsi(    engine.rsi() ),
        _dil(    engine.dil() ),
        _di(     engine.di() ),
        _edi(    engine.edi() ),
        _rdi(    engine.rdi() ),
        _bpl(    engine.bpl() ),
        _bp(     engine.bp() ),
        _ebp(    engine.ebp() ),
        _rbp(    engine.rbp() ),
        _spl(    engine.spl() ),
        _sp(     engine.sp() ),
        _esp(    engine.esp() ),
        _rsp(    engine.rsp() ),
        _ip(     engine.ip() ),
        _eip(    engine.eip() ),
        _rip(    engine.rip() ),
        _cs(     engine.cs() ),
        _ds(     engine.ds() ),
        _es(     engine.es() ),
        _fs(     engine.fs() ),
        _gs(     engine.gs() ),
        _ss(     engine.ss() ),
        _eflags( engine.eflags() ),
        _r8b(    engine.r8b() ),
        _r8w(    engine.r8w() ),
        _r8d(    engine.r8d() ),
        _r8(     engine.r8() ),
        _r9b(    engine.r9b() ),
        _r9w(    engine.r9w() ),
        _r9d(    engine.r9d() ),
        _r9(     engine.r9() ),
        _r10b(   engine.r10b() ),
        _r10w(   engine.r10w() ),
        _r10d(   engine.r10d() ),
        _r10(    engine.r10() ),
        _r11b(   engine.r11b() ),
        _r11w(   engine.r11w() ),
        _r11d(   engine.r11d() ),
        _r11(    engine.r11() ),
        _r12b(   engine.r12b() ),
        _r12w(   engine.r12w() ),
        _r12d(   engine.r12d() ),
        _r12(    engine.r12() ),
        _r13b(   engine.r13b() ),
        _r13w(   engine.r13w() ),
        _r13d(   engine.r13d() ),
        _r13(    engine.r13() ),
        _r14b(   engine.r14b() ),
        _r14w(   engine.r14w() ),
        _r14d(   engine.r14d() ),
        _r14(    engine.r14() ),
        _r15b(   engine.r15b() ),
        _r15w(   engine.r15w() ),
        _r15d(   engine.r15d() ),
        _r15(    engine.r15() )
    {}

    Registers::IMPL::IMPL( const IMPL & o ):
        _ah(     o._ah ),
        _al(     o._al ),
        _ax(     o._ax ),
        _eax(    o._eax ),
        _rax(    o._rax ),
        _bh(     o._bh ),
        _bl(     o._bl ),
        _bx(     o._bx ),
        _ebx(    o._ebx ),
        _rbx(    o._rbx ),
        _ch(     o._ch ),
        _cl(     o._cl ),
        _cx(     o._cx ),
        _ecx(    o._ecx ),
        _rcx(    o._rcx ),
        _dh(     o._dh ),
        _dl(     o._dl ),
        _dx(     o._dx ),
        _edx(    o._edx ),
        _rdx(    o._rdx ),
        _sil(    o._sil ),
        _si(     o._si ),
        _esi(    o._esi ),
        _rsi(    o._rsi ),
        _dil(    o._dil ),
        _di(     o._di ),
        _edi(    o._edi ),
        _rdi(    o._rdi ),
        _bpl(    o._bpl ),
        _bp(     o._bp ),
        _ebp(    o._ebp ),
        _rbp(    o._rbp ),
        _spl(    o._spl ),
        _sp(     o._sp ),
        _esp(    o._esp ),
        _rsp(    o._rsp ),
        _ip(     o._ip ),
        _eip(    o._eip ),
        _rip(    o._rip ),
        _cs(     o._cs ),
        _ds(     o._ds ),
        _es(     o._es ),
        _fs(     o._fs ),
        _gs(     o._gs ),
        _ss(     o._ss ),
        _eflags( o._eflags ),
        _r8b(    o._r8b ),
        _r8w(    o._r8w ),
        _r8d(    o._r8d ),
        _r8(     o._r8 ),
        _r9b(    o._r9b ),
        _r9w(    o._r9w ),
        _r9d(    o._r9d ),
        _r9(     o._r9 ),
        _r10b(   o._r10b ),
        _r10w(   o._r10w ),
        _r10d(   o._r10d ),
        _r10(    o._r10 ),
        _r11b(   o._r11b ),
        _r11w(   o._r11w ),
        _r11d(   o._r11d ),
        _r11(    o._r11 ),
        _r12b(   o._r12b ),
        _r12w(   o._r12w ),
        _r12d(   o._r12d ),
        _r12(    o._r12 ),
        _r13b(   o._r13b ),
        _r13w(   o._r13w ),
        _r13d(   o._r13d ),
        _r13(    o._r13 ),
        _r14b(   o._r14b ),
        _r14w(   o._r14w ),
        _r14d(   o._r14d ),
        _r14(    o._r14 ),
        _r15b(   o._r15b ),
        _r15w(   o._r15w ),
        _r15d(   o._r15d ),
        _r15(    o._r15 )
    {}

    Registers::IMPL::~IMPL( void )
    {}
}
