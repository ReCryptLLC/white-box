//***************************************************************************************
// round.h
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

#ifndef ROUND_H
#define ROUND_H

#include "poly.h"
#include <vector>

namespace NWhiteBox
{

class CRound
{
public:
    struct tuple // x * a. Multiplication in GF(2^8). p - irreducible.
    {
        uint8_t a;
        uint8_t p;
    };

    struct mix_t
    {
        tuple const& operator[]( size_t index ) const
        {
            return m_mix[index];
        }

        tuple& operator[]( size_t index )
        {
            return m_mix[index];
        }

        tuple m_mix[16];
    };

    typedef std::vector<uint8_t>    s_boxes_t[16];

public:
    CRound( uint32_t min_mixes_count, uint32_t max_mixes_count, bool is_decr = false, bool is_last = false );
    CRound( uint32_t min_mixes_count, uint32_t max_mixes_count, std::vector<mix_t> const& prev_mixes, bool is_decr = false,
        bool is_last = false );
    CRound( CRound const& r );
    virtual ~CRound();

public:
    void Init();
    void Init( std::vector<mix_t> const& prev_mixes );
    void Init( s_boxes_t const& s_boxes_clear, NGFPoly::CPoly const& poly, NGFPoly::CPoly const& anti_poly, 
        uint8_t irr_p, std::vector<mix_t> const& prev_mixes );  
    void Init( s_boxes_t const& s_boxes_clear, NGFPoly::CPoly const& poly, NGFPoly::CPoly const& anti_poly, 
        uint8_t irr_p ); 
    void Clear();

private:
    void CreateMixes();
    void CreatePoly();
    void CreateSboxes( std::vector<mix_t> const& prev_mixes );
    void ApplyPrevMixes( std::vector<mix_t> const& prev_mixes );

public:
    CRound const& operator =( CRound const& r );

public:
    s_boxes_t const& GetSboxes() const
    {
        return m_s_boxes;
    }

    s_boxes_t const& GetClearSboxes() const
    {
        return m_s_boxes_clear;
    }


    NGFPoly::CPoly const& GetPoly() const
    {
        return m_poly;
    }

    NGFPoly::CPoly const& GetAntiPoly() const
    {
        return m_anti_poly;
    }

    uint8_t GetIrreduciblePoly() const
    {
        return m_irr_p;
    }       

    std::vector<mix_t> const& GetMixes() const
    {
        return m_mixes;
    }

    bool IsLast() const
    {
        return m_is_last;
    }

    uint32_t GetMinMixesCount() const
    {
        return m_min_mixes_count;
    }

    uint32_t GetMaxMixesCount() const
    {
        return m_max_mixes_count;
    }

    void SetMinMixesCount( uint32_t min_mixes_count )
    {
        m_min_mixes_count = min_mixes_count;
    }

    void SetMaxMixesCount( uint32_t max_mixes_count )
    {
        m_max_mixes_count = max_mixes_count;
    }

    bool IsDecyption() const
    {
        return m_is_decr;
    }

    void SetDecryption( bool is_decr )
    {
        m_is_decr = is_decr;
    }

private:
    s_boxes_t               m_s_boxes;
    s_boxes_t               m_s_boxes_clear;
    NGFPoly::CPoly          m_poly;
    NGFPoly::CPoly          m_anti_poly;
    uint8_t                 m_irr_p;
    std::vector<mix_t>      m_mixes;
    bool                    m_is_last; 
    uint32_t                m_min_mixes_count;
    uint32_t                m_max_mixes_count;
    bool                    m_is_decr;
};

void InverseSboxes( CRound::s_boxes_t const& s_boxes_in, CRound::s_boxes_t& s_boxes_out );

}

#endif // ROUND_H