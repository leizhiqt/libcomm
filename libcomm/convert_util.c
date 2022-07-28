#include "convert_util.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bclog.h"
#include "dtype.h"

void byte1_to_char(char *hexs,unsigned char va)
{
    memset(hexs,'\0',21);
    sprintf(hexs,"%02x",va);
}

float32_t byte4_to_float32(uchar_8 *va)
{
    UStuff32_t value;
    value.ascii[3]=*va;
    value.ascii[2]=*(va+1);
    value.ascii[1]=*(va+2);
    value.ascii[0]=*(va+3);

//    value.va_float=va;
    /*
    // 求符号位
    float sig = 1.;

    if (*va >=128.)
        sig = -1.;
//求阶码
    float jie = 0.;
    if (*va >=128.)
    {
        jie = *va-128.  ;
    }
    else
    {
        jie = *va;
    }
    jie = jie * 2.;
    if (*(va+1) >=128.)
        jie += 1.;
    jie -= 127.;
//求尾码
    float tail = 0.;
    if (*(va+1) >=128.)
        *(va+1) -= 128.;
    tail =  *(va+3) + (*(va+2) + *(va+1) * 256.) * 256.;
    tail  = (tail)/8388608;   //   8388608 = 2^23
    float f = sig * pow(2., jie) * (1+tail);
*/
    return value.va_float;
}


void ntoh_32(char *net_bytes)
{
    unsigned char host_bytes[4]={0};

    host_bytes[0]=net_bytes[3];
    host_bytes[1]=net_bytes[2];
    host_bytes[2]=net_bytes[1];
    host_bytes[3]=net_bytes[0];

    net_bytes[0]=host_bytes[0];
    net_bytes[1]=host_bytes[1];
    net_bytes[2]=host_bytes[2];
    net_bytes[3]=host_bytes[3];
}

void swap_byte32(uchar_8 *htol,const uchar_8 *ltoh)
{
    *(htol+0)=*(ltoh+3);
    *(htol+1)=*(ltoh+2);
    *(htol+2)=*(ltoh+1);
    *(htol+3)=*(ltoh+0);
}


void copy_byte32(uchar_8 *htol,const uchar_8 *ltoh)
{
    *(htol+0)=*(ltoh+0);
    *(htol+1)=*(ltoh+1);
    *(htol+2)=*(ltoh+2);
    *(htol+3)=*(ltoh+3);
}

void ntoh_16(char *net_bytes)
{
    unsigned char host_bytes[2]={0};

    host_bytes[0]=net_bytes[1];
    host_bytes[1]=net_bytes[0];

    net_bytes[0]=host_bytes[0];
    net_bytes[1]=host_bytes[1];
}

void sprintf_hex(char *hexs,unsigned char const *p,int size)
{
    unsigned char bufs[1024];
    memset(bufs,'\0',sizeof(bufs));
    char buf[20];
    for (int i = 0; i < size;i++) {
        memset(buf,'\0',sizeof(buf));
        sprintf(buf,"%02x ",(unsigned char)*(p+i));
        strcat((char *)bufs,buf);
    }
    strcpy((char *)hexs,(char *)bufs);
}

int hexs_to_binary(const char *in,int len, unsigned char *out) {
    char *str = (char *)malloc(len);
    memset(str, 0, len);
    memcpy(str, in, len);
    for (int i = 0; i < len; i+=2) {
        //小写转大写
        if(str[i] >= 'a' && str[i] <= 'f') str[i] = str[i] & ~0x20;
        if(str[i+1] >= 'a' && str[i] <= 'f') str[i+1] = str[i+1] & ~0x20;
        //处理第前4位
        if(str[i] >= 'A' && str[i] <= 'F')
            out[i/2] = (str[i]-'A'+10)<<4;
        else
            out[i/2] = (str[i] & ~0x30)<<4;
        //处理后4位, 并组合起来
        if(str[i+1] >= 'A' && str[i+1] <= 'F')
            out[i/2] |= (str[i+1]-'A'+10);
        else
            out[i/2] |= (str[i+1] & ~0x30);
    }
    free(str);
    return 0;
}

int binary_to_hexs(unsigned char *in, int len, char *out) {
    for (int i = 0; i < len; i++) {
        if ((in[i] >> 4) >= 10 && (in[i] >> 4) <= 15)
            out[2*i] = (in[i] >> 4) + 'A' - 10;
        else
            out[2*i] = (in[i] >> 4) | 0x30;

        if ((in[i] & 0x0f) >= 10 && (in[i] & 0x0f) <= 15)
            out[2*i+1] = (in[i] & 0x0f) + 'A' - 10;
        else
            out[2*i+1] = (in[i] & 0x0f) | 0x30;
    }
    return 0;
}

void utf8ToGbk(char *utf8String, char *gbkString)
{
#ifdef _WIN32
   /*
    wchar_t *unicodeStr = NULL;
    int nRetLen = 0;

    nRetLen = MultiByteToWideChar(CP_UTF8, 0, utf8String, -1, NULL, 0);

    //求需求的宽字符数大小
    unicodeStr = (wchar_t *)malloc(nRetLen * sizeof(wchar_t));
    nRetLen = MultiByteToWideChar(CP_UTF8, 0, utf8String, -1, unicodeStr, nRetLen);

    //将utf-8编码转换成unicode编码
    nRetLen = WideCharToMultiByte(CP_ACP, 0, unicodeStr, -1, NULL, 0, NULL, 0);

    //求转换所需字节数
    nRetLen = WideCharToMultiByte(CP_ACP, 0, unicodeStr, -1, gbkString, nRetLen, NULL, 0);

    //unicode编码转换成gbk编码
    free(unicodeStr);
    */
#endif
}


