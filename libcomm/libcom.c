// libcom.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "stdio.h"
#include "bclog.h"
#include "thpool.h"
#include "bctime.h"
#include "stdlib.h"

#ifdef __ANDROID__
#elif __linux__
#include "sys/socket.h"
#include "unistd.h"
#include "stdlib.h"
#include "errno.h"
#include <sys/types.h>
#include "signal.h"
#include "bclog.h"
#include "unistd.h"

#elif _WIN32
#define WIN32_LEAN_AND_MEAN // 排除一些不太常用的 API, 加速生成过程
#include "Windows.h"
//#include "process.h"

#endif

//static uint forever;



void *console()
{
	char sendBuf[1024];

	fprintf(stdout, "====Welcome lepoll console=====\n");
	while (1) {
		LOG_INFO("console$>");
		/*fgets(sendBuf, MAXDATASIZE, stdin);

		if (strcmp(sendBuf, "exit\n") == 0) {
			//forever = 0;
			break;
		}

		if (strcmp(sendBuf, "w\n") == 0) {
			save_append("warn.txt", bbuf);
			printf("cache save to warn.txt done\n");
		}

		if (strcmp(sendBuf, "show\n") == 0) {
			printf("console$>%ld bytes\n", (wp - bbuf));
		}
		*/
		//Sleep(50);
	}
	LOG_INFO("console$>bye!\n");
	//Sleep(2);
	//pthread_exit(NULL);
}

void th_task(void *argc)
{
	//CPUTIME_TEST_BEGIN

	int task_id = *(int *)argc;
	LOG_INFO("th_task begin");
	for (int i = 0; i < 15; i++) {
		LOG_INFO("th_task %d", task_id);
		//
#ifdef __linux__
		usleep(50);
#endif

#ifdef _WIN32
		Sleep(1000);
#endif
	}
	LOG_INFO("th_task end");
	//CPUTIME_TEST_PRINT
	//pthread_exit(NULL);
}

int main()
{
	log_initialize("logcomm.txt", 1);
	LOG_INFO("abcd");
	LOG_INFO("abcd");
	LOG_INFO("abcd");
	LOG_INFO("abcd");


	//线程池
	thpool_t thpool;
	thpool_init(&thpool, 5, 2);
	//Sleep(50);

	//启动测试用例
	//test_start(&client);
	//if (thpool_add_work(&thpool, th_task, NULL) < 0)
	//{
	//	LOG_INFO("thpool full");
	//}
	//Sleep(1);

	//epoll_process();

	for (int i = 0; i < 20; i++) {
		if (thpool_add_work(&thpool, th_task, &i) < 0)
		{
			LOG_INFO("thpool full");
		}
#ifdef __linux__
		usleep(50);
#endif

#ifdef _WIN32
		Sleep(1000);
#endif
	}

	LOG_INFO("start finsh\n");

	thpool_destroy(&thpool);
	LOG_INFO("thpool_destroy\n");

	//log_release();
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
