/**********************************
 * @author     wallwind@yeah.net
 * @date        2012/06/13
 * Last update: 2012/06/13
 * License:     LGPL
 *
 **********************************/
#include "thpool.h"

#include "stdlib.h"
#include "errno.h"
#include "signal.h"
#include "bclog.h"


#ifdef __linux__
static	sigset_t signal_mask;
#endif


static void th_nosing()
{
#ifdef __linux__
	sigemptyset(&signal_mask);
	sigaddset(&signal_mask, SIGPIPE);//send errno
	//sigaddset(&signal_mask,SIGQUIT);//ctr+c errno

	int rc = pthread_sigmask(SIG_BLOCK, &signal_mask, NULL);//NULL屏蔽信号处理函数
	if (rc != 0) {
		printfs("%s\t%d\t[DEBUG]:thpool_add_work new:block sigpipe error", __FILE__, __LINE__);
	}
#endif
}

//h1 = (HANDLE)_beginthread(Thread1, 0, NULL);//线程1

//开辟的线程--一直运行
static void thpool_thread_do(void* v)
{
	th_node_t *th_node = (th_node_t *)v;

	LOG_INFO("thpool_thread_do begin pthread_t:%ld lock:%d", th_node->thread, th_node->lock);
	//GetCurrentThreadId()
	while (1)
	{
		//printfs("%s\t%d\t[DEBUG]:thpool_thread_do loop pthread_t:%ld lock:%d",__FILE__,__LINE__,th_node->thread,th_node->lock);
#ifdef __ANDROID__
#elif __linux__
		pthread_mutex_lock(&(th_node->mutex));
#elif _WIN32
		EnterCriticalSection(&(th_node->mutex));
#endif
		//检测队列就绪--执行
		//printfs("%s\t%d\t[DEBUG]:thpool_thread_do loop pthread_t:%ld lock:%d",__FILE__,__LINE__,th_node->thread,th_node->lock);
		if (th_node->lock && th_node->function != NULL)
		{
			th_node->function(th_node->arg);
			th_node->lock = 0;//清空队列
			th_node->function = NULL;
			th_node->arg = NULL;
		}

		//exit
		if (th_node->lock == -1)
		{
#ifdef __ANDROID__
#elif __linux__
			pthread_mutex_unlock(&(th_node->mutex));
#elif _WIN32
			LeaveCriticalSection(&(th_node->mutex));
#endif
			break;
		}
#ifdef __ANDROID__
#elif __linux__
		pthread_mutex_unlock(&(th_node->mutex));
#elif _WIN32
		LeaveCriticalSection(&(th_node->mutex));
#endif

#ifdef __ANDROID__
#elif __linux__
		//usleep(30*1000);
		usleep(50);
#elif _WIN32
		Sleep(50);
#endif
	}
	LOG_INFO("thpool_thread_do exit pthread_t:%ld lock:%d", th_node->thread, th_node->lock);
}

//将消息加入线程池
#ifdef __ANDROID__
#elif __linux__
pthread_t thpool_add_work(thpool_t* thpool, void* (*function)(void*), void* arg)


#elif _WIN32
HANDLE thpool_add_work(thpool_t* thpool, void* (*function)(void*), void* arg)

#endif
{
	th_node_t *p = thpool->head;
	int busy = 0;
	int k = 0;

again:
	k = 0;
	do
	{
		if (!p->lock)
		{
#ifdef __ANDROID__
#elif __linux__
			pthread_mutex_lock(&(p->mutex));
#elif _WIN32
			EnterCriticalSection(&(p->mutex));
#endif
			p->function = function;
			p->arg = arg;
			p->lock = 1;

#ifdef __ANDROID__
#elif __linux__
			pthread_mutex_unlock(&(p->mutex));
#elif _WIN32
			LeaveCriticalSection(&(p->mutex));
#endif
			//LOG_INFO("thpool_add_work exits:%ld (0x%x) k=%d lock:%d",p->thread,p->thread,k,p->lock);
			return p->thread;
		}
		//LOG_INFO("k=%d lock:%d",k,!(p->lock));
		k++;
	} while ((p = p->next) != NULL);

	//printfs("%s\t%d\t[DEBUG]:thpool_add_work for k=%d max_threads=%d",__FILE__,__LINE__,k,thpool->max_threads);
	if (k == thpool->max_threads)
	{
		if (busy == 10)
		{
			LOG_INFO("thpool_add_work is full max_threads:%ld k=%d", thpool->max_threads, k);
			return -1;
		}
		LOG_INFO("thpool_add_work loop add:busy=%d", busy);

		busy++;
		p = thpool->head;

#ifdef __ANDROID__
#elif __linux__
		sleep(1);
#elif _WIN32
		Sleep(1000);
#endif
		goto again;
	}

	//新分配空间
	//LOG_INFO("thpool_add_work for NEW");
	p = (th_node_t*)malloc(sizeof(th_node_t));
	p->function = function;
	p->arg = arg;
	p->next = NULL;
	p->prev = NULL;

#ifdef __ANDROID__
#elif __linux__
	pthread_mutex_init(&(p->mutex), NULL);
#elif _WIN32
	InitializeCriticalSection(&(p->mutex));
#endif
	//add to last node
	thpool->last->next = p;
	p->prev = thpool->last;
	thpool->last = p;
	p->lock = 1;

	th_nosing();

#ifdef __ANDROID__
#elif __linux__
	pthread_create(&(p->thread), NULL, (void *)thpool_thread_do, (void*)p);
	//usleep(50);
#elif _WIN32
	p->thread = _beginthreadex(NULL, 0, (void *)thpool_thread_do, (void*)p, 0, 0);
#endif
	LOG_INFO("thpool_add_work new:%u (0x%x)", p->thread, p->thread);
	return p->thread;
}

int thpool_init(thpool_t* thpool, const int max_threads, const int min_threads)
{
	//最大线程数
	if (!max_threads || max_threads < 1)
		thpool->max_threads = 1;
	else
		thpool->max_threads = max_threads;

	//最小线程数
	if (!min_threads || min_threads >= max_threads)
		thpool->min_threads = max_threads;
	else
		thpool->min_threads = min_threads;

	thpool->head = NULL;
	thpool->last = NULL;

	for (int i = 0; i < thpool->min_threads; i++)
	{
		th_node_t *p = (th_node_t*)malloc(sizeof(th_node_t));
		p->function = NULL;
		p->arg = NULL;
		p->next = NULL;
		p->prev = NULL;
		p->lock = 0;

#ifdef __ANDROID__
#elif __linux__
		pthread_mutex_init(&(p->mutex), NULL);
#elif _WIN32
		InitializeCriticalSection(&(p->mutex));
#endif
		if (i == 0)
		{
			thpool->head = p;
			thpool->last = p;
		}
		else
		{
			//add to last node
			thpool->last->next = p;
			p->prev = thpool->last;
			thpool->last = p;
		}

		th_nosing();

		//LOG_INFO("thpool_init:%ld"p->thread);
#ifdef __ANDROID__
#elif __linux__
		pthread_create(&(p->thread), NULL, (void *)thpool_thread_do, (void*)p);
#elif _WIN32
		p->thread = _beginthreadex(NULL, 0, (void *)thpool_thread_do, (void*)p, 0, 0);
#endif
		LOG_INFO("thpool_add_work new:%u (0x%x)", p->thread, p->thread);
	}
	return 0;
}

void thpool_destroy(thpool_t* thpool)
{
	//LOG_INFO("thpool_destroy begin");
	th_node_t *p = thpool->head;
	th_node_t *p1 = NULL;
	do
	{
		unsigned long td = p->thread;

#ifdef __ANDROID__
#elif __linux__
		pthread_mutex_lock(&(p->mutex));
#elif _WIN32
		EnterCriticalSection(&(p->mutex));
#endif
		p->lock = -1;
#ifdef __ANDROID__
#elif __linux__
		pthread_mutex_unlock(&(p->mutex));
#elif _WIN32
		LeaveCriticalSection(&(p->mutex));
#endif

#ifdef __ANDROID__
#elif __linux__
		//usleep(500);
		pthread_cancel(p->thread);
		//LOG_INFO("wait thpool_destroy pthread_t:%ld stop....",td);
		pthread_join(p->thread, NULL);
		pthread_mutex_destroy(&(p->mutex));
#elif _WIN32
		DWORD dwExitCode1;
		GetExitCodeThread(p->thread, &dwExitCode1);//获取线程是否运行完毕，若运行完毕返回 dwExitCode=0，否则为259
		if (dwExitCode1 != 0)//有其一未运行完毕，等待100ms
		{
			Sleep(100);
		}

		DeleteCriticalSection(&(p->mutex));
#endif
		p1 = p->next;
		free(p);
		//LOG_INFO("thpool_destroy already pthread_t:%ld",td);
	} while ((p = p1) != NULL);

	thpool->head = NULL;
	thpool->last = NULL;
}
