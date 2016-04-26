//***************************************************************************************
// main.cpp
// White-Box implementation sample
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

#include <stdio.h>
#include "stdtypes.h"
#include "wb_decr_tbl.h"
#include "wb_encr_tbl.h"


#define encr_r( bo, bi, r ) do{                                                     \
    ((uint32_t*)bo)[0] = ((uint32_t*)wb_encr_tbl_##r[0][((uint8_t*)bi)[0]])[0] ^    \
                        ((uint32_t*)wb_encr_tbl_##r[1][((uint8_t*)bi)[5]])[0] ^     \
                        ((uint32_t*)wb_encr_tbl_##r[2][((uint8_t*)bi)[10]])[0] ^    \
                        ((uint32_t*)wb_encr_tbl_##r[3][((uint8_t*)bi)[15]])[0] ^    \
                        ((uint32_t*)wb_encr_tbl_##r[4][((uint8_t*)bi)[4]])[0] ^     \
                        ((uint32_t*)wb_encr_tbl_##r[5][((uint8_t*)bi)[9]])[0] ^     \
                        ((uint32_t*)wb_encr_tbl_##r[6][((uint8_t*)bi)[14]])[0] ^    \
                        ((uint32_t*)wb_encr_tbl_##r[7][((uint8_t*)bi)[3]])[0] ^     \
                        ((uint32_t*)wb_encr_tbl_##r[8][((uint8_t*)bi)[8]])[0] ^     \
                        ((uint32_t*)wb_encr_tbl_##r[9][((uint8_t*)bi)[13]])[0] ^    \
                        ((uint32_t*)wb_encr_tbl_##r[10][((uint8_t*)bi)[2]])[0] ^    \
                        ((uint32_t*)wb_encr_tbl_##r[11][((uint8_t*)bi)[7]])[0] ^    \
                        ((uint32_t*)wb_encr_tbl_##r[12][((uint8_t*)bi)[12]])[0] ^   \
                        ((uint32_t*)wb_encr_tbl_##r[13][((uint8_t*)bi)[1]])[0] ^    \
                        ((uint32_t*)wb_encr_tbl_##r[14][((uint8_t*)bi)[6]])[0] ^    \
                        ((uint32_t*)wb_encr_tbl_##r[15][((uint8_t*)bi)[11]])[0];    \
    ((uint32_t*)bo)[1] = ((uint32_t*)wb_encr_tbl_##r[0][((uint8_t*)bi)[0]])[1] ^    \
                        ((uint32_t*)wb_encr_tbl_##r[1][((uint8_t*)bi)[5]])[1] ^     \
                        ((uint32_t*)wb_encr_tbl_##r[2][((uint8_t*)bi)[10]])[1] ^    \
                        ((uint32_t*)wb_encr_tbl_##r[3][((uint8_t*)bi)[15]])[1] ^    \
                        ((uint32_t*)wb_encr_tbl_##r[4][((uint8_t*)bi)[4]])[1] ^     \
                        ((uint32_t*)wb_encr_tbl_##r[5][((uint8_t*)bi)[9]])[1] ^     \
                        ((uint32_t*)wb_encr_tbl_##r[6][((uint8_t*)bi)[14]])[1] ^    \
                        ((uint32_t*)wb_encr_tbl_##r[7][((uint8_t*)bi)[3]])[1] ^     \
                        ((uint32_t*)wb_encr_tbl_##r[8][((uint8_t*)bi)[8]])[1] ^     \
                        ((uint32_t*)wb_encr_tbl_##r[9][((uint8_t*)bi)[13]])[1] ^    \
                        ((uint32_t*)wb_encr_tbl_##r[10][((uint8_t*)bi)[2]])[1] ^    \
                        ((uint32_t*)wb_encr_tbl_##r[11][((uint8_t*)bi)[7]])[1] ^    \
                        ((uint32_t*)wb_encr_tbl_##r[12][((uint8_t*)bi)[12]])[1] ^   \
                        ((uint32_t*)wb_encr_tbl_##r[13][((uint8_t*)bi)[1]])[1] ^    \
                        ((uint32_t*)wb_encr_tbl_##r[14][((uint8_t*)bi)[6]])[1] ^    \
                        ((uint32_t*)wb_encr_tbl_##r[15][((uint8_t*)bi)[11]])[1];    \
    ((uint32_t*)bo)[2] = ((uint32_t*)wb_encr_tbl_##r[0][((uint8_t*)bi)[0]])[2] ^    \
                        ((uint32_t*)wb_encr_tbl_##r[1][((uint8_t*)bi)[5]])[2] ^     \
                        ((uint32_t*)wb_encr_tbl_##r[2][((uint8_t*)bi)[10]])[2] ^    \
                        ((uint32_t*)wb_encr_tbl_##r[3][((uint8_t*)bi)[15]])[2] ^    \
                        ((uint32_t*)wb_encr_tbl_##r[4][((uint8_t*)bi)[4]])[2] ^     \
                        ((uint32_t*)wb_encr_tbl_##r[5][((uint8_t*)bi)[9]])[2] ^     \
                        ((uint32_t*)wb_encr_tbl_##r[6][((uint8_t*)bi)[14]])[2] ^    \
                        ((uint32_t*)wb_encr_tbl_##r[7][((uint8_t*)bi)[3]])[2] ^     \
                        ((uint32_t*)wb_encr_tbl_##r[8][((uint8_t*)bi)[8]])[2] ^     \
                        ((uint32_t*)wb_encr_tbl_##r[9][((uint8_t*)bi)[13]])[2] ^    \
                        ((uint32_t*)wb_encr_tbl_##r[10][((uint8_t*)bi)[2]])[2] ^    \
                        ((uint32_t*)wb_encr_tbl_##r[11][((uint8_t*)bi)[7]])[2] ^    \
                        ((uint32_t*)wb_encr_tbl_##r[12][((uint8_t*)bi)[12]])[2] ^   \
                        ((uint32_t*)wb_encr_tbl_##r[13][((uint8_t*)bi)[1]])[2] ^    \
                        ((uint32_t*)wb_encr_tbl_##r[14][((uint8_t*)bi)[6]])[2] ^    \
                        ((uint32_t*)wb_encr_tbl_##r[15][((uint8_t*)bi)[11]])[2];    \
    ((uint32_t*)bo)[3] = ((uint32_t*)wb_encr_tbl_##r[0][((uint8_t*)bi)[0]])[3] ^    \
                        ((uint32_t*)wb_encr_tbl_##r[1][((uint8_t*)bi)[5]])[3] ^     \
                        ((uint32_t*)wb_encr_tbl_##r[2][((uint8_t*)bi)[10]])[3] ^    \
                        ((uint32_t*)wb_encr_tbl_##r[3][((uint8_t*)bi)[15]])[3] ^    \
                        ((uint32_t*)wb_encr_tbl_##r[4][((uint8_t*)bi)[4]])[3] ^     \
                        ((uint32_t*)wb_encr_tbl_##r[5][((uint8_t*)bi)[9]])[3] ^     \
                        ((uint32_t*)wb_encr_tbl_##r[6][((uint8_t*)bi)[14]])[3] ^    \
                        ((uint32_t*)wb_encr_tbl_##r[7][((uint8_t*)bi)[3]])[3] ^     \
                        ((uint32_t*)wb_encr_tbl_##r[8][((uint8_t*)bi)[8]])[3] ^     \
                        ((uint32_t*)wb_encr_tbl_##r[9][((uint8_t*)bi)[13]])[3] ^    \
                        ((uint32_t*)wb_encr_tbl_##r[10][((uint8_t*)bi)[2]])[3] ^    \
                        ((uint32_t*)wb_encr_tbl_##r[11][((uint8_t*)bi)[7]])[3] ^    \
                        ((uint32_t*)wb_encr_tbl_##r[12][((uint8_t*)bi)[12]])[3] ^   \
                        ((uint32_t*)wb_encr_tbl_##r[13][((uint8_t*)bi)[1]])[3] ^    \
                        ((uint32_t*)wb_encr_tbl_##r[14][((uint8_t*)bi)[6]])[3] ^    \
                        ((uint32_t*)wb_encr_tbl_##r[15][((uint8_t*)bi)[11]])[3]; } while( 0 );  
                                                                                    
#define decr_r( bo, bi, r )  do {                                                   \
    ((uint32_t*)bo)[0] = ((uint32_t*)wb_decr_tbl_##r[0][((uint8_t*)bi)[0]])[0] ^    \
                        ((uint32_t*)wb_decr_tbl_##r[1][((uint8_t*)bi)[13]])[0] ^    \
                        ((uint32_t*)wb_decr_tbl_##r[2][((uint8_t*)bi)[10]])[0] ^    \
                        ((uint32_t*)wb_decr_tbl_##r[3][((uint8_t*)bi)[7]])[0] ^     \
                        ((uint32_t*)wb_decr_tbl_##r[4][((uint8_t*)bi)[4]])[0] ^     \
                        ((uint32_t*)wb_decr_tbl_##r[5][((uint8_t*)bi)[1]])[0] ^     \
                        ((uint32_t*)wb_decr_tbl_##r[6][((uint8_t*)bi)[14]])[0] ^    \
                        ((uint32_t*)wb_decr_tbl_##r[7][((uint8_t*)bi)[11]])[0] ^    \
                        ((uint32_t*)wb_decr_tbl_##r[8][((uint8_t*)bi)[8]])[0] ^     \
                        ((uint32_t*)wb_decr_tbl_##r[9][((uint8_t*)bi)[5]])[0] ^     \
                        ((uint32_t*)wb_decr_tbl_##r[10][((uint8_t*)bi)[2]])[0] ^    \
                        ((uint32_t*)wb_decr_tbl_##r[11][((uint8_t*)bi)[15]])[0] ^   \
                        ((uint32_t*)wb_decr_tbl_##r[12][((uint8_t*)bi)[12]])[0] ^   \
                        ((uint32_t*)wb_decr_tbl_##r[13][((uint8_t*)bi)[9]])[0] ^    \
                        ((uint32_t*)wb_decr_tbl_##r[14][((uint8_t*)bi)[6]])[0] ^    \
                        ((uint32_t*)wb_decr_tbl_##r[15][((uint8_t*)bi)[3]])[0];     \
    ((uint32_t*)bo)[1] = ((uint32_t*)wb_decr_tbl_##r[0][((uint8_t*)bi)[0]])[1] ^    \
                        ((uint32_t*)wb_decr_tbl_##r[1][((uint8_t*)bi)[13]])[1] ^    \
                        ((uint32_t*)wb_decr_tbl_##r[2][((uint8_t*)bi)[10]])[1] ^    \
                        ((uint32_t*)wb_decr_tbl_##r[3][((uint8_t*)bi)[7]])[1] ^     \
                        ((uint32_t*)wb_decr_tbl_##r[4][((uint8_t*)bi)[4]])[1] ^     \
                        ((uint32_t*)wb_decr_tbl_##r[5][((uint8_t*)bi)[1]])[1] ^     \
                        ((uint32_t*)wb_decr_tbl_##r[6][((uint8_t*)bi)[14]])[1] ^    \
                        ((uint32_t*)wb_decr_tbl_##r[7][((uint8_t*)bi)[11]])[1] ^    \
                        ((uint32_t*)wb_decr_tbl_##r[8][((uint8_t*)bi)[8]])[1] ^     \
                        ((uint32_t*)wb_decr_tbl_##r[9][((uint8_t*)bi)[5]])[1] ^     \
                        ((uint32_t*)wb_decr_tbl_##r[10][((uint8_t*)bi)[2]])[1] ^    \
                        ((uint32_t*)wb_decr_tbl_##r[11][((uint8_t*)bi)[15]])[1] ^   \
                        ((uint32_t*)wb_decr_tbl_##r[12][((uint8_t*)bi)[12]])[1] ^   \
                        ((uint32_t*)wb_decr_tbl_##r[13][((uint8_t*)bi)[9]])[1] ^    \
                        ((uint32_t*)wb_decr_tbl_##r[14][((uint8_t*)bi)[6]])[1] ^    \
                        ((uint32_t*)wb_decr_tbl_##r[15][((uint8_t*)bi)[3]])[1];     \
    ((uint32_t*)bo)[2] = ((uint32_t*)wb_decr_tbl_##r[0][((uint8_t*)bi)[0]])[2] ^    \
                        ((uint32_t*)wb_decr_tbl_##r[1][((uint8_t*)bi)[13]])[2] ^    \
                        ((uint32_t*)wb_decr_tbl_##r[2][((uint8_t*)bi)[10]])[2] ^    \
                        ((uint32_t*)wb_decr_tbl_##r[3][((uint8_t*)bi)[7]])[2] ^     \
                        ((uint32_t*)wb_decr_tbl_##r[4][((uint8_t*)bi)[4]])[2] ^     \
                        ((uint32_t*)wb_decr_tbl_##r[5][((uint8_t*)bi)[1]])[2] ^     \
                        ((uint32_t*)wb_decr_tbl_##r[6][((uint8_t*)bi)[14]])[2] ^    \
                        ((uint32_t*)wb_decr_tbl_##r[7][((uint8_t*)bi)[11]])[2] ^    \
                        ((uint32_t*)wb_decr_tbl_##r[8][((uint8_t*)bi)[8]])[2] ^     \
                        ((uint32_t*)wb_decr_tbl_##r[9][((uint8_t*)bi)[5]])[2] ^     \
                        ((uint32_t*)wb_decr_tbl_##r[10][((uint8_t*)bi)[2]])[2] ^    \
                        ((uint32_t*)wb_decr_tbl_##r[11][((uint8_t*)bi)[15]])[2] ^   \
                        ((uint32_t*)wb_decr_tbl_##r[12][((uint8_t*)bi)[12]])[2] ^   \
                        ((uint32_t*)wb_decr_tbl_##r[13][((uint8_t*)bi)[9]])[2] ^    \
                        ((uint32_t*)wb_decr_tbl_##r[14][((uint8_t*)bi)[6]])[2] ^    \
                        ((uint32_t*)wb_decr_tbl_##r[15][((uint8_t*)bi)[3]])[2];     \
    ((uint32_t*)bo)[3] = ((uint32_t*)wb_decr_tbl_##r[0][((uint8_t*)bi)[0]])[3] ^    \
                        ((uint32_t*)wb_decr_tbl_##r[1][((uint8_t*)bi)[13]])[3] ^    \
                        ((uint32_t*)wb_decr_tbl_##r[2][((uint8_t*)bi)[10]])[3] ^    \
                        ((uint32_t*)wb_decr_tbl_##r[3][((uint8_t*)bi)[7]])[3] ^     \
                        ((uint32_t*)wb_decr_tbl_##r[4][((uint8_t*)bi)[4]])[3] ^     \
                        ((uint32_t*)wb_decr_tbl_##r[5][((uint8_t*)bi)[1]])[3] ^     \
                        ((uint32_t*)wb_decr_tbl_##r[6][((uint8_t*)bi)[14]])[3] ^    \
                        ((uint32_t*)wb_decr_tbl_##r[7][((uint8_t*)bi)[11]])[3] ^    \
                        ((uint32_t*)wb_decr_tbl_##r[8][((uint8_t*)bi)[8]])[3] ^     \
                        ((uint32_t*)wb_decr_tbl_##r[9][((uint8_t*)bi)[5]])[3] ^     \
                        ((uint32_t*)wb_decr_tbl_##r[10][((uint8_t*)bi)[2]])[3] ^    \
                        ((uint32_t*)wb_decr_tbl_##r[11][((uint8_t*)bi)[15]])[3] ^   \
                        ((uint32_t*)wb_decr_tbl_##r[12][((uint8_t*)bi)[12]])[3] ^   \
                        ((uint32_t*)wb_decr_tbl_##r[13][((uint8_t*)bi)[9]])[3] ^    \
                        ((uint32_t*)wb_decr_tbl_##r[14][((uint8_t*)bi)[6]])[3] ^    \
                        ((uint32_t*)wb_decr_tbl_##r[15][((uint8_t*)bi)[3]])[3]; } while( 0 );

int main()
{
    char bi[] = { 'W', 'h', 'i', 't', 'e', '-', 'B', 'o', 'x', ' ', 's', 'a', 'm', 'p', 'l', 'e', 0 };
    char bo[16];

    printf_s( "Before: %s\n", bi );

    encr_r( bo, bi, 0 );
    encr_r( bi, bo, 1 );
    encr_r( bo, bi, 2 );
    encr_r( bi, bo, 3 );
    encr_r( bo, bi, 4 );
    encr_r( bi, bo, 5 );
    encr_r( bo, bi, 6 );
    encr_r( bi, bo, 7 );
    encr_r( bo, bi, 8 );
    encr_r( bi, bo, 9 );

    decr_r( bo, bi, 0 );
    decr_r( bi, bo, 1 );
    decr_r( bo, bi, 2 );
    decr_r( bi, bo, 3 );
    decr_r( bo, bi, 4 );
    decr_r( bi, bo, 5 );
    decr_r( bo, bi, 6 );
    decr_r( bi, bo, 7 );
    decr_r( bo, bi, 8 );
    decr_r( bi, bo, 9 ); 
    
    printf_s( "After: %s\n", bi );

    return 0;
}