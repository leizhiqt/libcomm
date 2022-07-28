#ifndef _BC_LOG_

#define _BC_LOG_

#ifdef __cplusplus
extern "C"
{
#endif

	int log_initialize(const char *log_file, int level);
	void log_printfs(char const *files, int line, char const *fmt, ...);
	int log_release();

	void printf_hex(char const *p, int size);
	void printbytes(const char * bytes, const int n);
	
	void nop();

#ifdef __cplusplus
}
#endif

#define LOG_INFO(format,...) log_printfs(__FILE__,__LINE__,format,##__VA_ARGS__) 
#define LOG_WARN(format,...) log_printfs(__FILE__,__LINE__,format,##__VA_ARGS__) 
#define LOG_EROR(format,...) log_printfs(__FILE__,__LINE__,format,##__VA_ARGS__) 

#endif
