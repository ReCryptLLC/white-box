//***************************************************************************************
// main.cpp
// Entry point
//
// Copyright (c) 2016,  Dmitry Schelkunov
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
#include "prng.h"
#include "cipher.h"


static char* const hello = { 
    "White-Box lookup tables generator\n\n"
    "Copyright (c) 2016,  Dmitry Schelkunov\n\n"
	
	"Permission is hereby granted, free of charge, to any person obtaining\n" 
    "a copy of this software and associated documentation files\n" 
	"(the \"Software\"), to deal in the Software without restriction,\n" 
	"including without limitation the rights to use, copy, modify, merge,\n" 
	"publish, distribute, sublicense, and/or sell copies of the Software,\n" 
	"and to permit persons to whom the Software is furnished to do so,\n" 
	"subject to the following conditions:\n"
	"\n"
	"The above copyright notice and this permission notice\n" 
	"shall be included in all copies or substantial portions\n"
    "of the Software.\n"
	"\n"
	"THE SOFTWARE IS PROVIDED \"AS IS\", WITHOUT WARRANTY OF ANY KIND,\n"
    "EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES\n" 
	"OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND\n"
    "NONINFRINGEMENT.\n"
	"IN NO EVENT SHALL THE AUTHORS OR\n" 
	"COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,\n" 
    "WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,\n" 
	"ARISING FROM, OUT OF OR IN CONNECTION WITH\n" 
	"THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.\n\n\n" 
    "USAGE: wb_creator.exe number_of_rounds min_number_of_mixes max_number_of_mixes\n\n"
};

int main( int argc, char* argv[] )
{
    
    printf_s( "%s", hello );
    
    if( argc != 4 )
    {
        printf_s( "%s", "Use wb_creator.exe number_of_rounds min_number_of_mixes max_number_of_mixes!\n" );
        return 1;
    }

    uint32_t rounds_num = (uint32_t)atol( argv[1] );
    uint32_t min_mixes_num = (uint32_t)atol( argv[2] );
    uint32_t max_mixes_num = (uint32_t)atol( argv[3] );

    try
    {
        if( min_mixes_num > max_mixes_num )
            throw std::runtime_error( "ERROR: min_mixes_number must be less or equal to max_mixes_number!!!\n" );
        
        NWhiteBox::CCipherCreator c( rounds_num, min_mixes_num, max_mixes_num );
        c.Init();
        c.Flash( "wb_encr_tbl.h", "wb_decr_tbl.h" );
    }
    catch( std::runtime_error& e )
    {
        printf_s( "%s", e.what() );
        getchar();
        return 2;
    }
    catch( ... )
    {
        printf_s( "%s", "Unknown internal error!\n" );
        getchar();
        return 3;
    }
        
    return 0;
}

