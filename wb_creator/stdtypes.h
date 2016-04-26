//***************************************************************************************
// stdtypes.h
// Standart types
// Dmitry Schelkunov, 2016
//***************************************************************************************

#ifndef STDTYPES_H
#define STDTYPES_H

#ifdef WIN32

typedef unsigned char	    uint8_t;
typedef unsigned __int32	uint32_t;
typedef unsigned __int64    uint64_t;

#endif // WIN32

typedef uint8_t             tbox_t[16];

#endif // STDTYPES_H