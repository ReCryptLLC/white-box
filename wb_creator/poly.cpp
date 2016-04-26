//***************************************************************************************
// poly.cpp
// Working with polynomials under GF(2^8)
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

#include "poly.h"

namespace NGFPoly
{

CPoly operator ^( CPoly const& p1, CPoly const& p2 )
{   
    CPoly const* min_poly ;
    CPoly const* max_poly;

    if( p1.size() > p2.size() )
    {
        min_poly = &p2;
        max_poly = &p1;
    }
    else
    {
        min_poly = &p1;
        max_poly = &p2;
    }

    std::vector<uint8_t> vret;
    vret.reserve( max_poly->size() );
    CPoly::size_type i;
    
    for( i = 0; i < min_poly->size(); ++i )
        vret.push_back( (*min_poly)[i] ^ (*max_poly)[i] );
        
    for( ; i < max_poly->size(); ++i )
        vret.push_back( (*max_poly)[i] );

    CPoly ret( vret );

    ret.validate_size();
    return ret;
}

CPoly operator +( CPoly const& p1, CPoly const& p2 )
{
    return p1 ^ p2;
}

CPoly operator -( CPoly const& p1, CPoly const& p2 )
{
    return p1 ^ p2;
}

CPoly operator >>( CPoly const& p, CPoly::size_type k )
{
    if( !p )
        return p;

    CPoly ret;
    ret.reserve( p.size() );
    
    for( CPoly::size_type j = 0; j < k; ++j )
    {
        for( CPoly::size_type i = 0; i < p.size() - 1; ++i  )
            ret[i] = p[i + 1];

        ret[p.size() - 1] = 0;
    }

    ret.validate_size();
    return ret;
}

CPoly operator <<( CPoly const& p, CPoly::size_type k )
{
    if( !p )
        return p;

    CPoly ret;
    ret.reserve( p.size() );
    
    for( CPoly::size_type j = 0; j < k; ++j )
    {
        for( CPoly::size_type i = p.size() - 1; i > 0 ; --i  )
            ret[i] = p[i - 1];

        ret[0] = 0;
    }
    
    ret.validate_size();
    return ret;
}

CPoly mul( CPoly const& p1, CPoly const& p2, uint8_t irr_p )
{
    CPoly ret;

    if( !p1 || !p2 )
        return ret;

    for( CPoly::size_type i = 0; i < p1.size(); ++i )
    {
        std::vector<uint8_t> v;

        for( CPoly::size_type j = 0; j < p2.size(); ++j )
        {
            v.push_back( NGF2exp8::gmul_tab( p1[i], p2[j], irr_p ) );
        }
        if( i != 0 )
        {
            ret.push( v[v.size() - 1] );

            for( CPoly::size_type j = 0; j < ( v.size() - 1 ); ++j )
            {
                ret[j + i] ^= v[j];
            }
        }
        else
        {
            ret = v;
        }
    }
    
    ret.validate_size();
    return ret;
}

CPoly div( CPoly const& p1, CPoly const& p2, uint8_t irr_p, CPoly& q )
{
    if( !p1 )
    {
        q = p1;
        return p1;
    }

    if( !p2 )
        std::runtime_error( "Error: Division by zero!!!\n" );

    if( p2.size() > p1.size() )
    {
        q = CPoly();
        return p1;
    }

    CPoly d( p1 );
    std::vector<uint8_t> v;
    v.resize( p1.size() - p2.size() + 1 );

    for( ; d.size() >= p2.size(); )
    {
        uint8_t t = v[d.size() - p2.size()] = NGF2exp8::gdiv_tab( d[d.size() - 1], p2[p2.size() - 1], irr_p );
        CPoly p( t, d.size() - p2.size() );
        p = mul( p, p2, irr_p );
        d = d - p;
    }

    q = v;
    d.validate_size();
    q.validate_size();
    return d;
}

CPoly normalize( CPoly const& p, uint8_t irr_p )
{
    if( !p )
        return p;

    CPoly ret( p );
    ret.validate_size();

    uint8_t val = NGF2exp8::inv_tab( ret[ret.size() - 1], irr_p );

    for( CPoly::size_type i = 0; i < ret.size(); ++i )
        ret[i] = NGF2exp8::gmul_tab( ret[i], val, irr_p );

    return ret;
}

CPoly euclid_algo( CPoly const& p1, CPoly const& p2, uint8_t irr_p )
{  
    CPoly p1_norm( p1 );
    CPoly p2_norm( p2 );

    CPoly u( p1_norm );
    CPoly v( p2_norm );

    CPoly d1( 1, 0 ), d2, d, q, /*s,*/ r/*, t, x1, x2( 1, 0 ), x*/;

    u.validate_size();
    v.validate_size();

    for( ; ; )
    {        
        if( u < v )
        {
            std::swap( u, v );
            std::swap( d1, d2 );
        }

        r = div( u, v, irr_p, q );

        if( !r )
            break;

        d = mul( q, d1, irr_p );
        d = div( d, p1_norm, irr_p, /*t*/q );
        d = d2 - d;

        d2 = d1; d1 = d; u = v; v = r;

    }

    if( v.size() != 1 )
        return CPoly();

    uint8_t val = NGF2exp8::inv_tab( v[0], irr_p );
    for( CPoly::size_type i = 0; i < d.size(); ++i )
        d[i] = NGF2exp8::gmul_tab( d[i], val, irr_p ); 

    return d;
}


}