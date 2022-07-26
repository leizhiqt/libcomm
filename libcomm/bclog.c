#ifdef __ANDROID__

#elif __linux__
#include "stdio.h"
#include "unistd.h"
#include "string.h"
#include "stdarg.h"
#include "fcntl.h"
//#include "sys/types.h"

#elif _WIN32
#define WIN32_LEAN_AND_MEAN
#include "stdio.h"
#include "windows.h"
#include "io.h"

HANDLE g_hSemaphore = NULL;

#endif

#include <fstream>
#include "bctime.h"
#include "bclog.h"

int _sk = 1024;

int _level = 0;

static char _log_file[128];

FILE *log_fp = NULL;

void nop() {}

void log_sk(const int sk)
{
	_sk = sk;
}

int log_initialize(const char *log_file, int level)
{
	int ret = 0;

	snprintf(_log_file, sizeof(_log_file), "%s", log_file);
	//memset(_log_file, '\0', sizeof(_log_file));

	_level = level;

#ifdef __linux__
	if (log_fp != NULL && (ret = access(_log_file, 2)) != -1) {
		printf("File ACCESS.C has write permission\n");
		return 1;
	}

	if ((log_fp = fopen(_log_file, "w+")) != 0)
	{
		printf("The file %s can not be opened.\n", "student.txt");
		return -1;
	}
#endif

#ifdef _WIN32
	if(g_hSemaphore==NULL)
	g_hSemaphore = CreateSemaphore(NULL, 1, 1, NULL);
	
	if (log_file != NULL)
	{
		printf("log_fp save:%s\n", log_file);
		snprintf(_log_file, sizeof(_log_file), "%s", log_file);
		ofstream file_writer(_log_file, ios_base::out);
		
		if (log_fp == NULL)
		{
			log_fp = fopen(_log_file, "a+");
			//log_fp = fopen(log_file, "w+");
		}
		return;
	}

	if (log_file == NULL)
	{
		string running_path = _pgmptr;
		string current_path;
		size_t found = running_path.find_last_of('.exe');
		if (found != std::string::npos)
		{
			current_path = running_path.substr(0, found - 3);
		}
		string save_log_path = current_path + ".txt";
		
		snprintf(_log_file, sizeof(_log_file), "%s", save_log_path.c_str());
		ofstream file_writer(_log_file, ios_base::out);
		printf("log_fp save:%s\n", _log_file);
		if (log_fp == NULL)
		{
			log_fp = fopen(_log_file, "a+");
		}
		return;
	}
#endif

	if (setvbuf(log_fp, NULL, _IOLBF, 1024) != 0) /*为流指定特殊的缓冲区*/
	{
		printf("failed to set up buffer for output file\n");
	}

	if (setvbuf(stdout, NULL, _IOLBF, 1024) != 0) /*为流指定特殊的缓冲区*/
	{
		printf("failed to set up buffer for output file\n");
	}

	return ret;
}

int log_release()
{
	int ret = 0;
#ifdef __linux__
	if (log_fp != NULL && (ret = access(_log_file, 2)) != -1) {
		LOG_INFO("%s", "关闭成功");
		fclose(log_fp);
	}
#endif

#ifdef _WIN32
	if (log_fp != NULL && (ret = _access(_log_file, 2)) != -1) {
		LOG_INFO("关闭成功");
		fclose(log_fp);
	}
#endif

	return ret;
}

void log_printfs(char const *files, int line, char const *fmt, ...)
{
#ifdef _WIN32
	if (g_hSemaphore == NULL)
		g_hSemaphore = CreateSemaphore(NULL, 1, 1, NULL);

	WaitForSingleObject(g_hSemaphore, INFINITE);      //信号量值-1
#endif
	int RET = -1;

	char buf[4096] = "\0";
	char logbuf[4096] = "\0";
	char times[32] = "\0";

	va_list va_alist;

	va_start(va_alist, fmt);

	vsnprintf(buf, sizeof(buf), fmt, va_alist);

	va_end(va_alist);

	//printf("logfile save:%s\n",logfile);
	getNowS(times, sizeof(times));

	snprintf(logbuf, sizeof(logbuf), "[%s %s %d]%s\n", times, files, line, buf);
	printf(logbuf);
	if (log_fp != NULL)
	{
		//printf("log_fp is Not NULL\n");
		fprintf(log_fp, "%s", logbuf);
		fflush(log_fp);
	}
#ifdef _WIN32
	ReleaseSemaphore(g_hSemaphore, 1, NULL);          //信号量值+1
#endif
}

void printf_hex(char const *p, int size)
{
	for (int i = 0; i < size; i++) {
		printf("[0x%02x %c] ", *(p + i), *(p + i));
	}
	printf("\n");
}

void printbytes(const char * bytes, const int n)
{
	for (int i = 0; i < n; i++)
	{
		printf("bytes[%d]:%02X\n", i, bytes[i]);
	}
}
