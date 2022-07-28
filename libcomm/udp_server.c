#include <Winsock2.h>
#include <stdio.h>

//服务器端口号为5050
#define DEFAULT_PORT 22004

//缓冲区长度
#define BUFFER_LENGTH  1500

int udp_server() {

	WSADATA wsaData;
	SOCKET sSocket;

	//客户地址长度
	int iLen;
	//发送的数据
	//int iSend;
	//接收的数据
	int iRecv;
	//要发送给客户的信息
	//char    send_buf[] = "Hello!I am a server.";
	//接收数据的缓冲区
	char    recv_buf[BUFFER_LENGTH];
	//本地地址和客户地址
	struct sockaddr_in  ser, cli;
	printf("------------------------------\n");
	printf("Server waiting\n");
	printf("------------------------------\n");
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		printf("Failed to load Winsock\n");
        return -1;
	}

	//产生服务器端套接口
	sSocket = socket(AF_INET, SOCK_DGRAM, 0);
	if (sSocket == INVALID_SOCKET)
	{
		printf("socket()Failed：%d\n", WSAGetLastError());
        return -2;
	}

	//以下建立服务器端地址
	ser.sin_family = AF_INET;
	//htons()函数把一个双字节主机字节顺序的数转换为网络字节顺序的数

    ser.sin_port = htons(DEFAULT_PORT);
	//htonl()函数把一个主机字节顺序的数转换为网络字节顺序的数
	ser.sin_addr.s_addr = htonl(INADDR_ANY);
	if (bind(sSocket, (LPSOCKADDR)&ser, sizeof(ser)) == SOCKET_ERROR)
	{
		printf("bind()Failed：%d\n", WSAGetLastError());
        return -3;
	}

    // 接收缓冲区
    int nRecvBuf=1024*1024*200;//设置为32K
    setsockopt(sSocket,SOL_SOCKET,SO_RCVBUF,(const char*)&nRecvBuf,sizeof(int));

	iLen = sizeof(cli);
	//初始化接收缓冲区
	memset(recv_buf, 0, sizeof(recv_buf));
	//进入一个无限循环,进行数据接收和发送
	while (1)
	{
		//从客户端接收数据
		iRecv = recvfrom(sSocket, recv_buf, BUFFER_LENGTH, 0, (SOCKADDR *)&cli, &iLen);
		if (iRecv == SOCKET_ERROR || iRecv == 0)
		{
			printf("recvfrom()Failed.：%d\n", WSAGetLastError());
			break;
		}

		////输出接收到的数据
		//printf("recvfrom()：%s\n", recv_buf);
		////输出客户IP地址和端口号
		//printf("Accepted client IP:[%s],port:[%d]\n", inet_ntoa(cli.sin_addr), ntohs(cli.sin_port));

		//给客户发送信息
	}

	closesocket(sSocket);
	WSACleanup();
}

void udp_server_th()
{
    HANDLE thread = _beginthreadex(NULL, 0, (void *)udp_server, (void*)NULL, 0, 0);
    Sleep(3);
}
