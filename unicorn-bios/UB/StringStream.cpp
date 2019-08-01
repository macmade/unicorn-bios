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

#include "StringStream.hpp"
#include <mutex>
#include <sstream>
#include <vector>
#include <functional>

namespace UB
{
    class StringStream::IMPL
    {
        public:
            
            IMPL( void );
            IMPL( const std::string & s );
            IMPL( const IMPL & o );
            IMPL( const IMPL & o, const std::lock_guard< std::recursive_mutex > & l );
            ~IMPL( void );
            
            mutable std::recursive_mutex                          _rmtx;
            std::stringstream                                     _ss;
            std::vector< std::reference_wrapper< std::ostream > > _redirects;
    };

    StringStream::StringStream( void ):
        impl( std::make_unique< IMPL >() )
    {}

    StringStream::StringStream( const std::string & s ):
        impl( std::make_unique< IMPL >( s ) )
    {}

    StringStream::StringStream( const StringStream & o ):
        impl( std::make_unique< IMPL >( *( o.impl ) ) )
    {}

    StringStream::StringStream( StringStream && o ) noexcept:
        impl( std::move( o.impl ) )
    {}

    StringStream::~StringStream( void )
    {}

    StringStream & StringStream::operator =( StringStream o )
    {
        swap( *( this ), o );
        
        return *( this );
    }

    StringStream::operator std::string() const
    {
        return this->string();
    }

    std::string StringStream::string( void ) const
    {
        std::lock_guard< std::recursive_mutex > l( this->impl->_rmtx );
        
        return this->impl->_ss.str();
    }
            
    void StringStream::redirect( std::ostream & os )
    {
        std::lock_guard< std::recursive_mutex > l( this->impl->_rmtx );
        
        return this->impl->_redirects.push_back( os );
    }

    StringStream & StringStream::operator <<( const std::string & s )
    {
        std::lock_guard< std::recursive_mutex > l( this->impl->_rmtx );
        
        this->impl->_ss << s;
        
        for( const auto & os: this->impl->_redirects )
        {
            os.get() << s;
        }
        
        return *( this );
    }

    StringStream & StringStream::operator <<( short v )
    {
        std::lock_guard< std::recursive_mutex > l( this->impl->_rmtx );
        
        this->impl->_ss << v;
        
        for( const auto & os: this->impl->_redirects )
        {
            os.get() << v;
        }
        
        return *( this );
    }

    StringStream & StringStream::operator <<( unsigned short v )
    {
        std::lock_guard< std::recursive_mutex > l( this->impl->_rmtx );
        
        this->impl->_ss << v;
        
        for( const auto & os: this->impl->_redirects )
        {
            os.get() << v;
        }
        
        return *( this );
    }

    StringStream & StringStream::operator <<( int v )
    {
        std::lock_guard< std::recursive_mutex > l( this->impl->_rmtx );
        
        this->impl->_ss << v;
        
        for( const auto & os: this->impl->_redirects )
        {
            os.get() << v;
        }
        
        return *( this );
    }

    StringStream & StringStream::operator <<( unsigned int v )
    {
        std::lock_guard< std::recursive_mutex > l( this->impl->_rmtx );
        
        this->impl->_ss << v;
        
        for( const auto & os: this->impl->_redirects )
        {
            os.get() << v;
        }
        
        return *( this );
    }

    StringStream & StringStream::operator <<( long v )
    {
        std::lock_guard< std::recursive_mutex > l( this->impl->_rmtx );
        
        this->impl->_ss << v;
        
        for( const auto & os: this->impl->_redirects )
        {
            os.get() << v;
        }
        
        return *( this );
    }

    StringStream & StringStream::operator <<( unsigned long v )
    {
        std::lock_guard< std::recursive_mutex > l( this->impl->_rmtx );
        
        this->impl->_ss << v;
        
        for( const auto & os: this->impl->_redirects )
        {
            os.get() << v;
        }
        
        return *( this );
    }

    StringStream & StringStream::operator <<( long long v )
    {
        std::lock_guard< std::recursive_mutex > l( this->impl->_rmtx );
        
        this->impl->_ss << v;
        
        for( const auto & os: this->impl->_redirects )
        {
            os.get() << v;
        }
        
        return *( this );
    }

    StringStream & StringStream::operator <<( unsigned long long v )
    {
        std::lock_guard< std::recursive_mutex > l( this->impl->_rmtx );
        
        this->impl->_ss << v;
        
        for( const auto & os: this->impl->_redirects )
        {
            os.get() << v;
        }
        
        return *( this );
    }

    StringStream & StringStream::operator <<( float v )
    {
        std::lock_guard< std::recursive_mutex > l( this->impl->_rmtx );
        
        this->impl->_ss << v;
        
        for( const auto & os: this->impl->_redirects )
        {
            os.get() << v;
        }
        
        return *( this );
    }

    StringStream & StringStream::operator <<( double v )
    {
        std::lock_guard< std::recursive_mutex > l( this->impl->_rmtx );
        
        this->impl->_ss << v;
        
        for( const auto & os: this->impl->_redirects )
        {
            os.get() << v;
        }
        
        return *( this );
    }

    StringStream & StringStream::operator <<( long double v )
    {
        std::lock_guard< std::recursive_mutex > l( this->impl->_rmtx );
        
        this->impl->_ss << v;
        
        for( const auto & os: this->impl->_redirects )
        {
            os.get() << v;
        }
        
        return *( this );
    }
    
    StringStream & StringStream::operator <<( std::ostream & ( * f )( std::ostream & ) )
    {
        std::lock_guard< std::recursive_mutex > l( this->impl->_rmtx );
        
        f( this->impl->_ss );
        
        for( const auto & os: this->impl->_redirects )
        {
            f( os.get() );
        }
        
        return *( this );
    }
    
    StringStream & StringStream::operator <<( std::ios_base & ( * f )( std::ios_base & ) )
    {
        std::lock_guard< std::recursive_mutex > l( this->impl->_rmtx );
        
        f( this->impl->_ss );
        
        for( const auto & os: this->impl->_redirects )
        {
            f( os.get() );
        }
        
        return *( this );
    }
    
    void swap( StringStream & o1, StringStream & o2 )
    {
        using std::swap;
        
        swap( o1.impl, o2.impl );
    }

    StringStream::IMPL::IMPL()
    {}

    StringStream::IMPL::IMPL( const std::string & s ):
        _ss( s )
    {}

    StringStream::IMPL::IMPL( const IMPL & o ):
        IMPL( o, std::lock_guard< std::recursive_mutex >( o._rmtx ) )
    {}

    StringStream::IMPL::IMPL( const IMPL & o, const std::lock_guard< std::recursive_mutex > & l ):
        _ss( o._ss.str() )
    {
        ( void )l;
    }

    StringStream::IMPL::~IMPL( void )
    {}
}
