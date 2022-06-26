/**********************************
 * @author	leizhifesker@gmail.com
 * @date	2017/02/16
 * Last update:	2017/02/16
 * License:	LGPL
 *
**********************************/

#ifndef _THPOOL_
#define _THPOOL_

#ifdef __ANDROID__

#elif __linux__
#include "unistd.h"
#include "pthread.h"

#elif _WIN32
#define WIN32_LEAN_AND_MEAN // 排除一些不太常用的 API, 加速生成过程
#include "Windows.h"
#include "process.h"

#endif

/*
*开辟一个线程池thpool
*每一个线程对应一个任务队列
*一个任务队列对应多个任务  任务调度 根据算法处理
*/

/*
*线程池thpool
*线程池加入任务队列
*/

/**
	线程节点
**/
typedef struct _th_node th_node_t;
typedef struct _th_node {

	unsigned int lock;		//是否可用

#ifdef __ANDROID__
#elif __linux__
	pthread_t thread;		//线程ID
	pthread_mutex_t mutex; //临界区 锁

#elif _WIN32
	CRITICAL_SECTION mutex;//临界区 锁
	HANDLE thread;//线程句柄
	//unsigned long threadId;//线程ID
#endif


	void* (*function)(void* arg);	//函数指针
	void*	arg;			//函数参数
	th_node_t* prev;		//指向上一个节点
	th_node_t* next;		//指向下一个节点
}th_node_t;

/**
	线程池
**/
typedef struct _thpool {
	int 		max_threads;	//线程数
	int 		min_threads;	//线程数
	th_node_t*	head;		//头指针
	th_node_t*	last;		//末尾指针

#ifdef __ANDROID__
#elif __linux__
	pthread_t thread;		//线程ID
	pthread_mutex_t mutex;		//访问锁

#elif _WIN32
	CRITICAL_SECTION mutex;//临界区 锁

#endif
}thpool_t;

#ifdef __cplusplus
extern "C"
{
#endif

	int thpool_init(thpool_t* thpool, const int max_threads, const int min_threads);


#ifdef __ANDROID__
#elif __linux__
	pthread_t thpool_add_work(thpool_t* thpool, void* (*function)(void*), void* arg);


#elif _WIN32
	HANDLE thpool_add_work(thpool_t* thpool, void* (*function)(void*), void* arg);

#endif

	void thpool_destroy(thpool_t* thpool);
#ifdef _cplusplus
}
#endif

#endif
