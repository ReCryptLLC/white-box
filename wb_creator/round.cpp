//***************************************************************************************
// round.cpp
// A creator of the white-box round
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

#include "round.h"
#include "prng.h"

namespace NWhiteBox
{

void InverseSboxes( CRound::s_boxes_t const& s_boxes_in, CRound::s_boxes_t& s_boxes_out )
{
    for( int i = 0; i < 16; ++i )
    {
        s_boxes_out[( i + ( i % 4 ) * 4 ) % 16].resize( 256, 0 );  // !!!!!!!!!!!!!!!!!!!!!!!!!!
        for( int j = 0; j < 256; ++j )
            s_boxes_out[( i + ( i % 4 ) * 4 ) % 16].at( s_boxes_in[i].at( j ) ) = (uint8_t)j; // !!!!!!!!!!!!!!!!!!!!!!
    }
}

//
// CRound
//

CRound::CRound( uint32_t min_mixes_count, uint32_t max_mixes_count, bool is_decr, bool is_last ) : m_is_last( is_last ),
m_min_mixes_count( min_mixes_count ), m_max_mixes_count( max_mixes_count ), m_is_decr( is_decr )
{
}

CRound::CRound( uint32_t min_mixes_count, uint32_t max_mixes_count, std::vector<mix_t> const& prev_mixes, bool is_decr, bool is_last ) : m_is_last( is_last ),
m_min_mixes_count( min_mixes_count ), m_max_mixes_count( max_mixes_count ), m_is_decr( is_decr )
{
    Init( prev_mixes );
}

CRound::CRound( CRound const& r ) : m_poly( r.m_poly ), m_anti_poly( r.m_anti_poly ), m_irr_p( r.m_irr_p ),
m_is_last( r.m_is_last ), m_mixes( r.m_mixes ), m_min_mixes_count( r.m_min_mixes_count ), m_max_mixes_count( r.m_max_mixes_count ), 
m_is_decr( r.m_is_decr )
{
    for( int i = 0; i < 16; ++i )
    {
        m_s_boxes[i] = r.m_s_boxes[i];
        m_s_boxes_clear[i] = r.m_s_boxes_clear[i];
    }
}

CRound::~CRound()
{
}

void CRound::Init()
{
    std::vector<CRound::mix_t> v;
    Init( v );
}

void CRound::Init( s_boxes_t const& s_boxes_clear, NGFPoly::CPoly const& poly, NGFPoly::CPoly const& anti_poly, 
        uint8_t irr_p )
{
    std::vector<CRound::mix_t> v; 
    Init( s_boxes_clear, poly, anti_poly, irr_p, v );
}

void CRound::Init( s_boxes_t const& s_boxes_clear, NGFPoly::CPoly const& poly, NGFPoly::CPoly const& anti_poly, 
        uint8_t irr_p, std::vector<mix_t> const& prev_mixes )
{
    // Fill S-boxes
    for( int i = 0; i < 16; ++i )
    {
        m_s_boxes[i] = m_s_boxes_clear[i] = s_boxes_clear[i];
    }

    ApplyPrevMixes( prev_mixes );
    
    // Init polynomials
    m_poly = poly;
    m_anti_poly = anti_poly;
    
    m_irr_p = irr_p;

    if( m_is_last )
        return;

    CreateMixes();
}

void CRound::Clear()
{
    m_poly.clear();
    m_anti_poly.clear();
    m_mixes.clear();
    m_irr_p = 0;
}

void CRound::Init( std::vector<CRound::mix_t> const& prev_mixes )
{
    Clear();
    CreateSboxes( prev_mixes );
    CreatePoly();
    if( m_is_last )
        return;
    CreateMixes();
}

void CRound::ApplyPrevMixes( std::vector<mix_t> const& prev_mixes )
{
    class calc_index
    {
    public:
        calc_index( bool decr ) : m_decr( decr ){}
        ~calc_index(){}
        
        int operator()( int i ) const
        {
            i =  ( !m_decr ) ? ( ( i + ( i % 4 ) * 4 ) % 16 ) : ( ((unsigned int)( i - ( i % 4 ) * 4 )) % 16 ); 
            return i;
        }

    private:
        bool m_decr;
    };
    
    
    if( prev_mixes.empty() )
    {
        for( int i = 0; i < 16; ++i )
        {
            m_s_boxes[i] = m_s_boxes_clear[i];
        }
        return;
    }

    calc_index ci( m_is_decr );

    for( int i = 0; i < 16; ++i )
    {     
        for( std::vector<uint8_t>::size_type j = 0; j < 256; ++j )
        {
            uint8_t index = (uint8_t)j;
            for( std::vector<CRound::mix_t>::size_type k = 0; k < prev_mixes.size(); ++k )
            {    
                index = NGF2exp8::gmul_tab( index, prev_mixes.at( k )[ci( i )].a, prev_mixes.at( k )[ci( i )].p );
            }
            m_s_boxes[i].at( index ) = m_s_boxes_clear[i].at( j );
        }
    }
}

void CRound::CreateSboxes( std::vector<CRound::mix_t> const& prev_mixes )
{
    // Create S-boxes randomly
    for( int i = 0; i < 16; ++i )
    {
        m_s_boxes[i].resize( 256 );
        m_s_boxes_clear[i].clear();
        m_s_boxes_clear[i].push_back( 0 );
        for( uint8_t j = 1; j != 0; ++j )
            m_s_boxes_clear[i].push_back( j );
        NPrng::shuffle( m_s_boxes_clear[i] );
    }

    ApplyPrevMixes( prev_mixes );
}

void CRound::CreatePoly()
{
    // Create polynomial
    NGFPoly::CPoly m( 1, 16 );
    m[0] = 1;
    m_irr_p = NGF2exp8::get_poly_by_index( NPrng::get_rnd_32() % 30 );
    
    if( m_is_last )
    {
        m_anti_poly = m_poly = NGFPoly::CPoly( 1, 0 );
        return;
    }   

    for( ; ; )
    {
        m_poly = NPrng::get_rnd_128();
        m_anti_poly = NGFPoly::euclid_algo( m, m_poly, m_irr_p ); 
        
        if( !m_anti_poly || m_anti_poly.size() != 16 )
            continue;

        bool is_gen_new_poly = false;
        for( NGFPoly::CPoly::size_type i  = 0; i < m_anti_poly.size(); ++i )
        {
            if( !m_anti_poly[i] )
            {
                is_gen_new_poly = true;
            }
        }
        if( is_gen_new_poly )
            continue;

        NGFPoly::CPoly p( NGFPoly::mul( m_poly, m_anti_poly, m_irr_p ) ), q;
        p = NGFPoly::div( p, m, m_irr_p, q );
        if( p.size() != 1 || p[0] != 1 )
            throw std::runtime_error( "ERROR: Illegal polynomial creation!!!\n" );
        break;    
    }
}

void CRound::CreateMixes()
{
    if( !m_max_mixes_count )
        return;

    // Create mixes
    uint32_t mixes_num = m_min_mixes_count + 
        ( ( m_min_mixes_count != m_max_mixes_count ) ? ( NPrng::get_rnd_32() %( m_max_mixes_count - m_min_mixes_count ) ) : 0 );
    
    std::vector<uint8_t> v_irr_p;

    for( uint32_t i = 0; i < 30; ++i )
        v_irr_p.push_back( NGF2exp8::get_poly_by_index( i ) );

    for( uint32_t i = 0; i < mixes_num; ++i )
    {
        CRound::mix_t mix_array;
        NPrng::shuffle( v_irr_p );                           
        for( int j = 0; j < 16; ++j )
        {   
            for( ; ; )
            {
                mix_array[j].a = NPrng::get_rnd_8();
                if( mix_array[j].a )
                    break;
            }
            mix_array[j].p = v_irr_p[j];
        }
        m_mixes.push_back( mix_array );
    }
}

CRound const& CRound::operator =( CRound const& r )
{
    for( int i = 0; i < 16; ++i )
    {
        m_s_boxes[i] = r.m_s_boxes[i];
        m_s_boxes_clear[i] = r.m_s_boxes_clear[i];
    }

    m_poly = r.m_poly;
    m_anti_poly = r.m_anti_poly;
    m_irr_p = r.m_irr_p;
    m_is_last = r.m_is_last;
    m_mixes = r.m_mixes; 
    m_min_mixes_count = r.m_min_mixes_count;
    m_max_mixes_count = r.m_max_mixes_count;

    return *this;
}

}