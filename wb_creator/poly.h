//***************************************************************************************
// poly.h
// CPoly class
// CPoly is a class for working with polynomials under GF(2^N)
//
// Copyright (c) 2016, Dmitry Schelkunov
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of 
// this software and associated documentation files (the "Software"), to deal in the 
// Software without restriction, including without limitation the rights to use, copy, 
// modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, 
// and to permit persons to whom the Software is furnished to do so, subject to the 
// following conditions:
//
// The above copyright notice and this permission notice shall be included in all copies 
// or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, 
// INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS 
// FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR 
// COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN 
// ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH 
// THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//
//***************************************************************************************

#ifndef POLY_H
#define POLY_H

#include "gf2exp8.h"
#include <vector>

namespace NGFPoly
{

class CPoly
{
public:
    typedef std::vector<uint8_t>::size_type     size_type;

public:
    CPoly()
    {}

    CPoly( std::vector<uint8_t> const& v ) : m_v( v )
    {}

    CPoly( CPoly const& p ) : m_v( p.m_v )
    {}

    CPoly( uint8_t val, size_type pos )
    {
        m_v.resize( pos + 1 );
        m_v[pos] = val;
    }
   
    virtual ~CPoly()
    {}

public:
    uint8_t operator[]( size_type index ) const
    {
        return m_v[index];
    }

    uint8_t& operator[]( size_type index )
    {
        return m_v[index];
    }

    size_type size() const
    {   
        return m_v.size();
    }

    void clear()
    {
        m_v.clear();
    }

    operator bool() const
    {
        return !m_v.empty();
    }

    void reserve( size_type count )
    {
        m_v.reserve( count );
    }

    void push( uint8_t val )
    {
        m_v.push_back( val );
    }

    void increase( std::vector<uint8_t> const& v )
    {
        m_v.insert( m_v.end(), v.begin(), v.end() );
    }

public:
    CPoly const& operator *() const
    {
        return *this;
    }

    CPoly& operator *()
    {
        return *this;
    }

    CPoly operator =( CPoly const& p )
    {
        m_v = p.m_v;
        return *this;
    }

    bool operator ==( CPoly const& p )
    {
        return m_v == p.m_v;
    }

    bool operator !=( CPoly const& p )
    {
        return !( *this == p );
    }

    bool operator <( CPoly const& p ) const
    {
        if( !(*this) && !p )
            return false;

        if( size() < p.size() )
            return true;

        if( size() > p.size() )
            return false;

        for( size_type i = m_v.size() - 1; i; --i )
        {
            if( m_v[i] == p.m_v[i] )
                continue;
            return m_v[i] < p.m_v[i]; 
        }

        return m_v[0] < p.m_v[0]; 
    }

    bool operator <=( CPoly const& p ) const
    {
        return ( (*this) < p || *(this) == p );
    }

    bool operator >( CPoly const& p ) const
    {
        return !( (*this) <= p );
    }

    bool operator >=( CPoly const& p ) const
    {
        return !( (*this) < p );
    }

    void validate_size()
    {
        if( !(*this) )
            return;

        size_type count = m_v.size() - 1;

        for( ; count != 0 && !m_v[count]; --count )
        {
        }

        if( count == 0 && !m_v[count] )
        {
            m_v.clear();
        }
        else
        {
            m_v.resize( count + 1 );
        }
    }

private:
    std::vector<uint8_t> m_v;


};

CPoly operator ^( CPoly const& p1, CPoly const& p2 );
CPoly operator +( CPoly const& p1, CPoly const& p2 );
CPoly operator -( CPoly const& p1, CPoly const& p2 );
CPoly operator >>( CPoly const& p, CPoly::size_type k );
CPoly operator <<( CPoly const& p, CPoly::size_type k );
CPoly mul( CPoly const& p1, CPoly const& p2, uint8_t irr_p );
CPoly div( CPoly const& p1, CPoly const& p2, uint8_t irr_p, CPoly& q ); // returns remainder
CPoly euclid_algo( CPoly const& p1, CPoly const& p2, uint8_t irr_p );
CPoly normalize( CPoly const& p, uint8_t irr_p );


}

#endif // POLY_H