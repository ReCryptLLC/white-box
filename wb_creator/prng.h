//***************************************************************************************
// prng.h
// Pseudo random numbers generator (Windows specific)
// Dmitry Schelkunov, 2016
//
//***************************************************************************************

#ifndef PRNG_H
#define PRNG_H

#include "stdtypes.h"
#include "poly.h"
#include <algorithm>

namespace NPrng
{

class CWinSpecificPrngCtx;

void get_rnd_128( void* buf, uint32_t size );
NGFPoly::CPoly get_rnd_128();
uint32_t get_rnd_32();
uint8_t get_rnd_8();


template <class T>
void shuffle( std::vector<T>& v )
{
    class Fn_rnd
    {
    public:
        uint32_t operator()( uint32_t index ) const
        {
            NGFPoly::CPoly p( get_rnd_128() );
            uint32_t ret = p[0];

            for( int i = 1; i < sizeof( uint32_t ); ++i )
            {
                ret = ( ret << 1 ) ^ p[i];
            }

            if( !index )
                return 0;
            return ret % index;
        }
    } f;

    std::random_shuffle( v.begin(), v.end(), f );
}

}

#endif // PRNG_H