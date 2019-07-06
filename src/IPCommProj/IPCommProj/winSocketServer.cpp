// IPCommProj.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include <iostream>
//#include"stdafx.h"
#include <winsock2.h>  

#include <string.h>  
using namespace std;

#pragma comment(lib, "ws2_32.lib")          //add ws2_32.lib  


const int DEFAULT_PORT = 8000;
int main(int argc, char* argv[])
{

	WORD    wVersionRequested;
	WSADATA wsaData;
	int     err, iLen;
	wVersionRequested = MAKEWORD(2, 2);

	err = WSAStartup(wVersionRequested, &wsaData);
	if (err != 0)
	{
		printf("WSAStartup failed with error: %d\n", err);
		return -1;
	}
	if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2)
	{
		printf("Could not find a usable version of Winsock.dll\n");
		WSACleanup();
		return 1;
	}
	else
	{
		printf("The Winsock 2.2 dll was found okay\n");
	}

	SOCKET sockSrv = socket(AF_INET, SOCK_STREAM, 0);
	if (sockSrv == INVALID_SOCKET)
	{
		printf("socket() fail:%d\n", WSAGetLastError());
		return -2;
	}

	SOCKADDR_IN addrSrv;
	memset(&addrSrv, 0, sizeof(addrSrv));
	addrSrv.sin_family = AF_INET;
	addrSrv.sin_addr.s_addr = htonl(INADDR_ANY);
	addrSrv.sin_port = htons(DEFAULT_PORT);

	err = bind(sockSrv, (SOCKADDR*)&addrSrv, sizeof(SOCKADDR));
	if (err != 0)
	{
		printf("bind()fail:%d\n", WSAGetLastError());
		return -3;
	}

	err = listen(sockSrv, 5);//listen函数的第一个参数即为要监听的socket描述字，第二个参数为相应socket可以排队的最大连接个数
	if (err != 0)
	{
		printf("listen()fail:%d\n", WSAGetLastError());
		return -4;
	}
	printf("Server waitting...!!!!!\n");
	SOCKADDR_IN addrClt;
	int len = sizeof(SOCKADDR);
	memset(&addrClt, 0, len);

	while (1)
	{
		SOCKET sockConn = accept(sockSrv, (SOCKADDR*)&addrClt, &len);
		char sendBuf[1024], hostname[100];
		if (gethostname(hostname, 100) != 0)
		{
			strcpy_s(hostname, strlen("None") + 1, "None");
		}
		sprintf_s(sendBuf, sizeof(sendBuf), "Welecome %s connected to %s!", inet_ntoa(addrClt.sin_addr), hostname);

		err = send(sockConn, sendBuf, strlen(sendBuf) + 1, 0);

		char recvBuf[1024] = "\0";
		iLen = recv(sockConn, recvBuf, 1024, 0);

		recvBuf[iLen] = '\0';
		printf(recvBuf);

		closesocket(sockConn);
	}

	closesocket(sockSrv);

	WSACleanup();
	return 0;
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门提示: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
