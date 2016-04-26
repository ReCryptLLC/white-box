//***************************************************************************************
// cipher.cpp
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

#include "cipher.h"
#include <stdio.h>
#include <stdlib.h>
#include "prng.h"


namespace NWhiteBox
{

std::string val_to_str( int val )
{
    char buf[10];
    if( _itoa_s( val, buf, sizeof( buf ), 10 ) )
        throw std::runtime_error( "Can\'t convert value to string!!!" );
    return buf;
}

std::string tbox_to_str( tbox_t const& item )
{
    std::string s( "{ " );
    for( int i = 0; i < sizeof( tbox_t ); ++i )
    {     
        s += val_to_str( item[i] );
        if( i != sizeof( tbox_t ) )
            s+= ", ";
        else
            s+= " ";
    }
    s += "}";
    return s;
}
 
//
// CCipherCreator
// 

CCipherCreator::CCipherCreator( uint32_t rnum, uint32_t min_mix_count, uint32_t max_mix_count ) : m_rnum( rnum ), 
m_min_mix_count( min_mix_count ), m_max_mix_count( max_mix_count )
{

}

CCipherCreator::~CCipherCreator()
{

}

void CCipherCreator::Init()
{
    if( m_rnum < 2 )
        throw std::runtime_error( "ERROR: Rounds number must be equal or greater than 2!!!" );

    // Create white-box lookup tables for every round
    CRound rnd( m_min_mix_count, m_max_mix_count );
    rnd.Init();
    m_rounds.push_back( rnd );

    for( uint32_t i = 1; i < m_rnum - 1; ++i )
    {
        rnd.Clear();
        rnd.Init( m_rounds[i - 1].GetMixes() );
        m_rounds.push_back( rnd );
    }

    CRound last( m_min_mix_count, m_max_mix_count, false, true );
    last.Init( m_rounds[m_rnum - 2].GetMixes() );
    m_rounds.push_back( last );

	// Create white-box inverse lookup tables (decryption lookup tables) for every round
    rnd.Clear();
    //rnd.SetMinMixesCount( 0 );	// uncomment fot debugging
    //rnd.SetMaxMixesCount( 0 );	// uncomment fot debugging
    rnd.SetDecryption( true );
    CRound::s_boxes_t   s_boxes_inv;
    InverseSboxes( last.GetClearSboxes(), s_boxes_inv );
    rnd.Init( s_boxes_inv, m_rounds[m_rnum - 2].GetAntiPoly(), m_rounds[m_rnum - 2].GetPoly(), 
        m_rounds[m_rnum - 2].GetIrreduciblePoly() );
    m_anti_rounds.push_back( rnd );

    for( uint32_t i = m_rnum - 2; i >= 1; --i )
    {
        rnd.Clear();     
        InverseSboxes( m_rounds[i].GetClearSboxes(), s_boxes_inv );
        rnd.Init( s_boxes_inv, m_rounds[i - 1].GetAntiPoly(), m_rounds[i - 1].GetPoly(), 
            m_rounds[i - 1].GetIrreduciblePoly(), m_anti_rounds[m_anti_rounds.size() - 1].GetMixes() );
        m_anti_rounds.push_back( rnd );
    }

    last.Clear();
    last.SetDecryption( true );
    NGFPoly::CPoly p1( 1, 0 ), p2( 1, 0 );
    InverseSboxes( m_rounds[0].GetClearSboxes(), s_boxes_inv );
    last.Init( s_boxes_inv, p1, p2, m_rounds[0].GetIrreduciblePoly(), m_anti_rounds[m_anti_rounds.size() - 1].GetMixes() );
    m_anti_rounds.push_back( last );

    SelfTest();
}

void CCipherCreator::SelfTest()
{
    //
    // TODO:
    //
}   

void CCipherCreator::Flash( std::string const& fname_encr, std::string const& fname_decr )
{
    FlashOneFile( fname_encr, "wb_encr_tbl", m_rounds );
    FlashOneFile( fname_decr, "wb_decr_tbl", m_anti_rounds );
}

void CCipherCreator::FlashOneFile( std::string const& fname, std::string const& tbl_name, std::vector<CRound> const& rounds )
{
    FILE* f;
    errno_t err = fopen_s( &f, fname.c_str(), "w" );  
    if( err != 0 )
        throw std::runtime_error( std::string( "ERROR: Can\'t open \'" ) + fname + "\' file!!!\n" );

    std::string::size_type pos = fname.find_first_of( '.' );
    std::string str_name;
    for( std::string::size_type i = 0; i < pos; ++i )
        str_name += fname[i];

    std::string str_wb( "/*****************************************************************************************\n" );
    str_wb += "Automatically generated white-box tables\n";
    str_wb += "Dmitry Schelkunov, 2009\n";
    str_wb += "*****************************************************************************************/\n\n\n";
    str_wb += "#include \"stdtypes.h\"\n#ifndef " + str_name + "_H\n#define " + str_name + "_H\n\n";

    // Convert rounds into T-boxes
    for( uint32_t i = 0; i < m_rnum; ++i )
    {
        str_wb += "const tbox_t " + tbl_name + "_" + val_to_str( i ) + "[16][256] = { \n";

        for( uint32_t j = 0; j < 16; ++j )
        {   
            str_wb += "{ ";

            tbox_t tbox_clear;
            
            if( !rounds[i].IsLast() )
            {
                for( uint32_t cnt = 0; cnt < sizeof( tbox_t ); ++cnt  )
                    tbox_clear[cnt] = rounds[i].GetPoly()[( cnt - j ) % 16];
            }
            else
            {
                for( uint32_t cnt = 0; cnt < sizeof( tbox_t ); ++cnt )
                    tbox_clear[cnt] = ( cnt == j ) ? 1 : 0;
            }

            for( uint32_t k = 0; k < 256; ++k )
            {
                tbox_t tbox;
                
                for( uint32_t cnt = 0; cnt < sizeof( tbox_t ); ++cnt )
                {
                    tbox[cnt] = NGF2exp8::gmul_tab( rounds[i].GetSboxes()[j].at( k ), 
                        tbox_clear[cnt], rounds[i].GetIrreduciblePoly() );
                    for( std::vector<CRound::mix_t>::size_type mix_cnt = 0; mix_cnt < rounds[i].GetMixes().size(); ++mix_cnt )
                    {
                        tbox[cnt] =  NGF2exp8::gmul_tab( ( rounds[i].GetMixes()[mix_cnt] )[cnt].a, tbox[cnt],
                            ( rounds[i].GetMixes()[mix_cnt] )[cnt].p );
                    }
                }
                
                str_wb += tbox_to_str( tbox );
                if( k != 255 )
                    str_wb += ",\n";
                else
                    str_wb += "\n";
            }

            if( j != 15 )
                str_wb += "},\n";
            else
                str_wb += "}\n";  
        }
        
        str_wb += "};\n";       
    }

    str_wb += "\n#endif // " + str_name + "_H\n";
    fwrite( str_wb.c_str(), sizeof( char ), str_wb.size(), f );

    fclose( f );
}

}
