//***************************************************************************************
// prng.cpp
// Pseudo random numbers generator (Windows specific for now)
//
// Dmitry Schelkunov, 2016
//***************************************************************************************

#include "prng.h"
#include <iostream>

#ifdef WIN32
#pragma comment(lib, "crypt32.lib")
#include <Windows.h>
#include <Wincrypt.h>
#endif // WIN32

namespace NPrng
{

#ifdef WIN32
class CWinSpecificPrngCtx
{
public:
	CWinSpecificPrngCtx()
	{
		if ( !::CryptAcquireContext(
			&m_hCryptProv,
			NULL,
			NULL,
			PROV_RSA_FULL,
			0 ) )
		{
			printf( "ERROR: CryptAcquireContext!!!\n" );
			exit( -1 );
		}

	}

	~CWinSpecificPrngCtx()
	{
		if (m_hCryptProv)
			::CryptReleaseContext(m_hCryptProv, 0);
	}

	void Get( void* buf, uint32_t size )
	{
		if( !CryptGenRandom(
			m_hCryptProv,
			size,
			(PBYTE)buf ) )
		{
			throw std::runtime_error("ERROR: CryptGenRandom!!!\n");
		}
	}

private:
	HCRYPTPROV   m_hCryptProv;
};

CWinSpecificPrngCtx g_win_prng_ctx;

#endif // WIN32


void get_rnd_128( void* buf, uint32_t size )
{
	if (size != 16)
		throw std::runtime_error("ERROR: Illegal size!!!\n");
	g_win_prng_ctx.Get( buf, size );
}

uint32_t get_rnd_32()
{
    uint32_t buf[4];
    get_rnd_128( buf, sizeof( buf ) );
    return buf[0];
}

uint8_t get_rnd_8()
{
    uint8_t buf[16];
    get_rnd_128( buf, sizeof( buf ) );
    return buf[0];
}

NGFPoly::CPoly get_rnd_128()
{   
    uint8_t rnd_val[16];

    NPrng::get_rnd_128( rnd_val, sizeof( rnd_val ) );

    NGFPoly::CPoly r;
    for( int i = 0; i < sizeof( rnd_val ); ++i )
    {
        if( rnd_val[i] )
            r.push( rnd_val[i] );
        else
            r.push( 1 );
    }

    return r;

}

}