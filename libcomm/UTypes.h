#ifndef UTYPES_H
#define UTYPES_H

//#pragma pack(4)

typedef unsigned char  uchar_8;
typedef unsigned int   uint32_t;
typedef unsigned short uint16_t;
typedef float          float32_t;

union UStuff32_t
{
  float    va_float;
  int      va_int;
  uchar_8  ascii[4];
};

#define hl_to_int16(l,h) ((uchar_8)h << 8 | (uchar_8)l)

#define hl_to_int32(l0,l1,h0,h1) ((uchar_8)h1 << 24 |(uchar_8)h0 << 16|(uchar_8)l1 << 8| (uchar_8)l0)

#define htol_int32(b0,b1,b2,b3) ((uchar_8)b0 << 24 |(uchar_8)b0 << 16|(uchar_8)b2 << 8| (uchar_8)b3)

//#define bx_to_int32t(bx) ((uchar_8)(bx)[0] << 24 |(uchar_8)(bx)[1] << 16|(uchar_8)(bx)[2] << 8| (uchar_8)(bx)[3])

#endif // UTYPES_H
