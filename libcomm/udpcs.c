#include <Winsock2.h>

#include <stdio.h>

//服务器端口号为5050

#define DEFAULT_PORT 22004

//缓冲区长度

#define DATA_BUFFER  1024

int main(int argc,char *argv[])

{

WSADATA wsaData;

SOCKET sClient;

int iPort=22004;

//服务器地址长度

int iLen;

//接收数据的缓冲

int iSend;

int   iRecv;

//要发送给服务器的信息

char send_buf[]="Hello!I am a client.";

//接收数据的缓冲区

char recv_buf[DATA_BUFFER];

//服务器端地址

struct sockaddr_in ser;

//处理命令行中

//if(argc<2)

//{

//       //提示在命令行中输入服务器IP地址

//       printf("Usage：client [server IP address]\n");

//       return -3;

//}

//接收数据的缓冲区初始化

memset(recv_buf,0,sizeof(recv_buf));

if(WSAStartup(MAKEWORD(2,2),&wsaData)!=0)

{

  printf("Failed to load Winsock.\n");

  return -1;

}

//建立服务器端地址

ser.sin_family=AF_INET;

ser.sin_port=htons(iPort);

ser.sin_addr.s_addr=inet_addr("127.0.0.1");

//建立客户端数据报套接口

sClient=socket(AF_INET,SOCK_DGRAM,0);

if(sClient==INVALID_SOCKET)

{

  printf("socket()Failed：%d\n",WSAGetLastError());

  return -2;

}

iLen=sizeof(ser);



//从服务器接收数据
while(1)
{
    //向服务器发送数据
    iSend=sendto(sClient,send_buf,sizeof(send_buf),0,(struct sockaddr*)&ser,iLen);
    if(iSend==SOCKET_ERROR)
    {
          printf("sendto()Failed：%d\n",WSAGetLastError());
           return -3;
    }else if(iSend==0)
          return -4;
     else
           printf("sendto()succeeded.\n");

    iRecv=recvfrom(sClient,recv_buf,sizeof(recv_buf),0,(struct sockaddr*)&ser,&iLen);
    if(iRecv==SOCKET_ERROR)
    {
           printf("recvfrom()Failed.%d\n",WSAGetLastError());
           return -5;
    }else if(iRecv==0){

       return -6;

    }

    //显示从服务器收到的信息
    fprintf(stdout,"recvfrom():%s\n",recv_buf);
    fprintf(stdout,"---------------------------\n");

    Sleep(5);
}

closesocket(sClient);
WSACleanup();

}


#include<Winsock2.h>

#include<stdio.h>

#include<stdlib.h>

//服务器端口号为5050

#define DEFAULT_PORT  5050

//接收数据缓冲区长度

#define BUFFER_LENGTH  1024

int main_udp_server(){

int   iPort = DEFAULT_PORT;

WSADATA wsaData;

SOCKET sSocket;

//客户地址长度

int iLen;

//发送的数据

int iSend;

//接收的数据

int iRecv;

//要发送给客户的信息

char    send_buf[]="Hello!I am a server.";

//接收数据的缓冲区

char    recv_buf[BUFFER_LENGTH];

//本地地址和客户地址

struct sockaddr_in  ser, cli;

printf("------------------------------\n");

printf("Server waiting\n");

printf("------------------------------\n");

if(WSAStartup(MAKEWORD(2,2),&wsaData)!=0)

{

    printf("Failed to load Winsock\n");

    return;

}

//产生服务器端套接口

sSocket=socket(AF_INET,SOCK_DGRAM,0);

if(sSocket==INVALID_SOCKET)

{

  printf("socket()Failed：%d\n",WSAGetLastError());

  return;

}

//以下建立服务器端地址

ser.sin_family=AF_INET;

//htons()函数把一个双字节主机字节顺序的数转换为网络字节顺序的数

ser.sin_port=htons(iPort);

//htonl()函数把一个主机字节顺序的数转换为网络字节顺序的数

ser.sin_addr.s_addr=htonl(INADDR_ANY);

if(bind(sSocket,(LPSOCKADDR)&ser,sizeof(ser))==SOCKET_ERROR)

{

  printf("bind()Failed：%d\n",WSAGetLastError());

  return;

}

iLen=sizeof(cli);

//初始化接收缓冲区

memset(recv_buf,0,sizeof(recv_buf));

//进入一个无限循环,进行数据接收和发送

while(1)

{

//从客户端接收数据

       iRecv=recvfrom(sSocket,recv_buf,BUFFER_LENGTH,0,(SOCKADDR *)&cli,&iLen);

if(iRecv==SOCKET_ERROR)

{

       printf("recvfrom()Failed.：%d\n",WSAGetLastError());

    break;

}

else if(iRecv==0)

    break;

    else

    {

    //输出接收到的数据

    printf("recvfrom()：%s\n",recv_buf);

    //输出客户IP地址和端口号

    printf("Accepted client IP:[%s],port:[%d]\n",inet_ntoa(cli.sin_addr),ntohs(cli.sin_port));

    }

//给客户发送信息

iSend=sendto(sSocket,send_buf,sizeof(send_buf),0,(SOCKADDR *)&cli,sizeof(cli));

if(iSend==SOCKET_ERROR)

{

    printf("sendto()Failed.：%d\n",WSAGetLastError());

    printf("------------------------------\n");

    break;

}

else if(iSend==0)

    break;

    else{

           printf("sendto() succeeded!\n");

           printf("------------------------------\n");

    }

}

closesocket(sSocket);

WSACleanup();

}
