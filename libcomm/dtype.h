#ifndef _DTYPE_H_
#define _DTYPE_H_

#define boolean unsigned int
#define true	1
#define false	0

#ifdef __cplusplus
extern "C"
{
#endif
	//#define uchar unsigned char
	//#define ushort unsigned short
	//#define uint unsigned int

	typedef unsigned char uchar;
	typedef unsigned short ushort;
	typedef unsigned int uint;

	typedef unsigned long ulong;

	typedef unsigned long long ulongl;
#ifdef _cplusplus
}
#endif

#endif