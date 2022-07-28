#define WIN32_LEAN_AND_MEAN // 排除一些不太常用的 API, 加速生成过程
#include "Windows.h"
#include "process.h"
#include <Winsock2.h>
#include <stdio.h>

//服务器端口号为5050
#define DEFAULT_PORT 22004
#define DEFAULT_IP "127.0.0.1"

//缓冲区长度
#define BUFFER_LENGTH  1500

int udp_client()
{
    WSADATA wsaData;
    SOCKET sClient;

//    int iPort = 22004;
    //服务器地址长度

    int iLen;
    //接收数据的缓冲

    int iSend;

    int   iRecv;
    //要发送给服务器的信息

    char send_buf[BUFFER_LENGTH];
    //接收数据的缓冲区
    char recv_buf[BUFFER_LENGTH];

    //服务器端地址
    struct sockaddr_in ser;

    memset(recv_buf, 0, sizeof(recv_buf));

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        printf("Failed to load Winsock.\n");
        return -1;
    }

    //建立服务器端地址
    ser.sin_family = AF_INET;
    ser.sin_port = htons(DEFAULT_PORT);
    ser.sin_addr.s_addr = inet_addr(DEFAULT_IP);

    //建立客户端数据报套接口
    sClient = socket(AF_INET, SOCK_DGRAM, 0);
    if (sClient == INVALID_SOCKET)
    {
        printf("socket()Failed：%d\n", WSAGetLastError());
        return -2;
    }

    // 接收缓冲区
//    int nRecvBuf=1024*1024*200;//设置为32K
//    setsockopt(sClient,SOL_SOCKET,SO_RCVBUF,(const char*)&nRecvBuf,sizeof(int));

    //发送缓冲区
    int nSendBuf=1024*1024*200;//设置为32K
    setsockopt(sClient,SOL_SOCKET,SO_SNDBUF,(const char*)&nSendBuf,sizeof(int));

    iLen = sizeof(ser);
    //从服务器接收数据
    while (1)
    {
        //向服务器发送数据
        iSend = sendto(sClient, send_buf, BUFFER_LENGTH, 0, (struct sockaddr*)&ser, iLen);
        if (iSend == SOCKET_ERROR || iSend == 0)
        {
            printf("sendto()Failed：%d\n", WSAGetLastError());
            return -3;
        }
//        printf("sendto()succeeded.\n");
//        Sleep(5);
    }
    closesocket(sClient);
    WSACleanup();
}

void udp_client_th()
{
    HANDLE thread = _beginthreadex(NULL, 0, (void *)udp_client, (void*)NULL, 0, 0);
    Sleep(3);
}
