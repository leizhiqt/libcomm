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
#endif

#include "bctime.h"
#include "bclog.h"

int _sk = 1024;

int _level = 0;

static char _log_file[128];

FILE *log_fp = NULL;

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
	if (log_fp != NULL && (ret = _access(_log_file, 2)) != -1) {
		printf("File ACCESS.C has write permission\n");
		return 1;
	}

	if ((ret = fopen_s(&log_fp, _log_file, "w+")) != 0)
	{
		printf("The file %s can not be opened.\n", "student.txt");
		return -1;
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
