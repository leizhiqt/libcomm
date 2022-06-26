/*
	Client API
*/

#ifndef _CSOCKET_H_
#define _CSOCKET_H_

#ifdef __ANDROID__
#elif __linux__
#include "stdio.h"
#include "stdlib.h"
#include "pthread.h"
#include "string.h"
#include "unistd.h"


#include "netdb.h"
#include "sys/socket.h"
#include "netinet/in.h"
#include "arpa/inet.h"
#include "netinet/in.h"


#elif _WIN32
#define WIN32_LEAN_AND_MEAN
#include <stdio.h>
#include <windows.h>
#include <WinSock2.h>
#endif

//#include "log.h"
//#include "sfline.h"

#define MAXDATASIZE 2048 /*每次最大数据传输量 */



//客户端
typedef struct _client client_t;
typedef struct _client {
	int sockfd;
	int port;
	struct hostent *host;
	struct sockaddr_in serv_addr;
#ifdef __linux__
	pthread_mutex_t lock;
#endif

#ifdef _WIN32

#endif

	int exit_socket;
	int sn;//send Number of times

	char buf[MAXDATASIZE];
	int buf_size;
	char flags;
}client_t;

#ifdef __cplusplus
extern "C"
{
#endif

	int client_connect(client_t* client);

	int client_send(client_t* client);

	int client_recv(client_t* client);

	void client_close(client_t* client);

	//
	int short_send(client_t* client);
#ifdef _cplusplus
}
#endif

#endif
