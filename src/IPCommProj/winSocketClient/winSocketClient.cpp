// winSocketClient.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include <iostream>

// client.cpp : 定义控制台应用程序的入口点。
//
//#include"stdafx.h"
#include <winsock2.h>  

#include <string.h>  
using namespace std;

#pragma comment(lib, "ws2_32.lib")          


const int DEFAULT_PORT = 8000;
int main(int argc, char* argv[])
{

	WORD    wVersionRequested;
	WSADATA wsaData;
	int     err, iLen;
	wVersionRequested = MAKEWORD(2, 2);

	err = WSAStartup(wVersionRequested, &wsaData); //load win socket  
	if (err != 0)
	{
		printf("WSAStartup failed with error: %d\n", err);
		return -1;
	}

	SOCKET sockClt = socket(AF_INET, SOCK_STREAM, 0);
	if (sockClt == INVALID_SOCKET)
	{
		printf("socket() fail:%d\n", WSAGetLastError());
		return -2;
	}

	SOCKADDR_IN addrSrv;
	memset(&addrSrv, 0, sizeof(addrSrv));
	addrSrv.sin_family = AF_INET;
	addrSrv.sin_addr.s_addr = inet_addr("127.0.0.1");
	addrSrv.sin_port = htons(DEFAULT_PORT);

	err = connect(sockClt, (SOCKADDR*)&addrSrv, sizeof(SOCKADDR));

	if (err == INVALID_SOCKET)
	{
		printf("connect()fail:%d\n", WSAGetLastError());
		return -1;
	}

	char sendBuf[1024], hostname[100];
	if (gethostname(hostname, 100) != 0) //如果没有错误的时候gethostname会返回0;
	{
		strcpy_s(hostname, strlen("None") + 1, "None");
	}
	strcpy_s(sendBuf, strlen(hostname) + 1, hostname);
	strcat_s(sendBuf, sizeof(sendBuf), " have connected to you!");
	err = send(sockClt, sendBuf, strlen(sendBuf) + 1, 0);

	char recvBuf[1024] = "\0";
	iLen = recv(sockClt, recvBuf, 1024, 0);

	if (iLen == 0)
	{
		return -3;
	}
	else if (iLen == SOCKET_ERROR)
	{
		printf("recv() fail:%d\n", WSAGetLastError());
		return -4;
	}
	else
	{
		recvBuf[iLen] = '\0';
		printf(recvBuf);
		printf("\n");
	}
	closesocket(sockClt);

	WSACleanup();
	system("PAUSE");
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
