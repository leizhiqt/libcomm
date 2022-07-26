#ifndef CONVERTUTIL_H
#define CONVERTUTIL_H

#include "dtype.h"

#define GET_BIT(x,bit) ((x&(1<<bit))>>bit) //获取bit位

#ifdef __cplusplus /* 如果采用了C++，如下代码使用C编译器 */
     extern  "C"  {  /* 如果没有采用C++，顺序预编译 */
#endif

void byte1_to_char(char *hexs,unsigned char va);
float32_t byte4_to_float32(uchar_8 *va);

void swap_byte32(uchar_8 *htol,const uchar_8 *ltoh);
void copy_byte32(uchar_8 *htol,const uchar_8 *ltoh);

void ntoh_32(char *bytes);
void ntoh_16(char *bytes);

void sprintf_hex(char *hexs,unsigned char const *p,int size);
//void printf_hex(unsigned char *hexs,int len);

int hexs_to_binary(const char *in,int len, unsigned char *out);
int binary_to_hexs(unsigned char *in, int len, char *out);

void utf8ToGbk(char *utf8String, char *gbkString);

/* 采用C编译器编译的C语言代码段 */
#ifdef __cplusplus /* 结束使用C编译器 */
 }
#endif
#endif // CONVERTUTIL_H
