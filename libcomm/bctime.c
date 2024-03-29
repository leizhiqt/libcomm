﻿#include "bctime.h"
#include <stdio.h>

//时间

//秒
struct tm *tm_now;

//毫秒 微秒
struct timeval tv;
struct timeval tv_prv;

time_t now;
char datetime[20];

//按照不同的方式格式化当前times
void t_formats(char *nowS, size_t s_len, const char*fmats)
{

#ifdef __ANDROID__
#elif __linux__
	time_t now;
	time(&now);
	tm_now = localtime(&now);
	strftime(nowS, 20, fmats, tm_now);

#elif _WIN32
	time_t timep;
	time(&timep);
	struct tm nowTime;
	localtime_s(&nowTime, &timep);
	strftime(nowS, s_len, fmats, &nowTime);

#endif

}

void getNowS(char *nowS, size_t size) {
#ifdef __ANDROID__
#elif __linux__
	time_t now;
	time(&now);
	struct tm *nowTime;
	nowTime=localtime(&now);
	strftime(nowS, size, "%Y-%m-%d %H:%M:%S", nowTime);

#elif _WIN32
	time_t timep;
	time(&timep);
	struct tm nowTime;
	localtime_s(&nowTime, &timep);
	strftime(nowS, size, "%Y-%m-%d %H:%M:%S", &nowTime);

#endif

}

//获取当前时间
void t_ftime(char *tsname)
{
	t_formats(tsname, 20, "%Y%m%d%H%M%S");
}
//获取当前日期
void t_fdate(char *s)
{
	t_formats(s, 20, "%Y%m%d");
}

//获取当前时间
void t_stime(char *times)
{
	t_formats(times, 20, "%Y-%m-%d %H:%M:%S");
}

//执行时间
int disTime()
{
#ifdef __linux__
	//执行一次处理的时间
	gettimeofday(&tv, NULL);
	if (tv_prv.tv_sec == 0)
	{
		tv_prv = tv;
		return 1;
	}
	//秒 微秒
	double second = (tv.tv_sec - tv_prv.tv_sec) + (double)(tv.tv_usec - tv_prv.tv_usec) / 1000000;//秒

	printf("Seconds:%ld Microseconds:%ld Seconds:%ld Microseconds:%ld  耗时秒:%lf",tv.tv_sec,tv.tv_usec,tv_prv.tv_sec,tv_prv.tv_usec,second);

	tv_prv = tv;
#endif

#ifdef _WIN32

#endif

	return 0;
}

//192.168.1.70
int ntpdate(const char *f_ip)
{
	char ntpd[64];
	snprintf(ntpd, sizeof(ntpd), "sudo /usr/sbin/ntpdate %s &", f_ip);
	system(ntpd);
	return 0;
}

//秒:纳秒
void sys_ms(char *ms)
{
#ifdef __linux__
	struct timeval tv;
	gettimeofday(&tv, NULL);
	snprintf(ms, 30, "%ld%ld", tv.tv_sec, tv.tv_usec);
#endif

#ifdef _WIN32

#endif


}
