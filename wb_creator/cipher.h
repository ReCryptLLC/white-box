//***************************************************************************************
// cipher.h
// A creator of the white-box cipher
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

#ifndef CIPHER_H
#define CIPHER_H

#include "round.h"
#include <string>

namespace NWhiteBox
{

class CCipherCreator
{
public:
    CCipherCreator( uint32_t rnum, uint32_t min_mix_count, uint32_t max_mix_count );
    virtual ~CCipherCreator();

public:
    void Flash( std::string const& fname_encr, std::string const& fname_decr );
    void FlashOneFile( std::string const& fname, std::string const& tbl_name, std::vector<CRound> const& rounds );
    void Init();

private:
    void SelfTest();

public:
    uint32_t GetRoundsNum() const
    {
        return m_rnum;
    }

    std::vector<CRound> const& GetRounds() const
    {
        return m_rounds;
    }

    std::vector<CRound> const& GetAntiRounds() const
    {
        return m_anti_rounds;
    }

    uint32_t GetMinMixCount() const
    {
        return m_min_mix_count;
    }

    uint32_t GetMaxMixCount() const
    {
        return m_max_mix_count;
    }

private:
    uint32_t                m_rnum;
    uint32_t                m_min_mix_count;
    uint32_t                m_max_mix_count;
    std::vector<CRound>     m_rounds;
    std::vector<CRound>     m_anti_rounds;
    
};

}

#endif // CIPHER_H