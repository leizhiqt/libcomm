#ifndef _BC_TIMES_H_
#define _BC_TIMES_H_

#ifdef __ANDROID__
#elif __linux__
#include "stdlib.h"
#include "sys/time.h"
#include "time.h"

#elif _WIN32
//#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <time.h>
#endif

#ifdef __cplusplus
extern "C"
{
#endif

	//按照不同的方式格式化当前times
	void t_formats(char *nowS, size_t s_len, const char*fmats);

	//获取当前时间
	void t_ftime(char *tsname);
	//获取当前日期
	void t_fdate(char *s);
	//获取当前时间
	void t_stime(char *tsname);

	//执行时间
	int disTime();

	int ntpdate(const char *f_ip);

	void sys_ms(char *ms);

	void getNowS(char *nowS, size_t size);



#ifdef __linux__
#define CPUTIME_TEST_BEGIN {}
#define CPUTIME_TEST_PRINT {}

#endif

#ifdef _WIN32

#ifdef __CPUTIME_TEST__
	clock_t start, stop;
	//clock_t是clock()函数返回的变量类型 
	double duration;
	//记录被测函数运行时间，以秒为单位
#define CPUTIME_TEST_BEGIN start = clock();
//开始计时 
//把被测函数加在这里
#define CPUTIME_TEST_PRINT \
	stop = clock(); \
duration = ((double)(stop - start)) / CLK_TCK; \
LOG_INFO("消耗CPU:%d", duration);
#endif

#endif

#ifdef _cplusplus
}
#endif

#endif
