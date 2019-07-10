// MultiThreadClient.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include <iostream>
#include "message11.pb.h"
#include <WinSock2.h>
#include <Ws2tcpip.h>
#include <string>


using namespace UService;

#pragma comment(lib, "ws2_32.lib")

using std::cin;
using std::cerr;
using std::cout;
using std::endl;
using std::flush;
using namespace std;

const char DEFAULT_PORT[] = "5000";
const int SEND_BUF_SIZE = 256;
const int PACKET_HEAD_SIZE = 4;  //报文的包头长度，4字节，表示包体的字节数

//客户端
int main() {
	WSADATA wsa_data; //WSADATA变量,包含windows socket执行的信息
	int i_result = 0; //接收返回值
	SOCKET sock_client = INVALID_SOCKET;
	addrinfo *result = nullptr, hints;
	//初始化winsock动态库(ws2_32.dll),MAKEWORD(2, 2)用于请求使用winsock2.2版本
	i_result = WSAStartup(MAKEWORD(2, 2), &wsa_data);
	if (i_result != 0) {
		cerr << "WSAStartup() function failed: " << i_result << "\n";
		system("pause");
		return 1;
	}
	SecureZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	//
	i_result = getaddrinfo("127.0.0.1", DEFAULT_PORT, &hints, &result);
	if (i_result != 0) {
		cerr << "getaddrinfo() function failed with error: " << WSAGetLastError() << "\n";
		WSACleanup();
		system("pause");
		return 1;
	}
	//创建套接字
	sock_client = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if (sock_client == INVALID_SOCKET) {
		cerr << "socket() function failed with error: " << WSAGetLastError() << "\n";
		WSACleanup();
		system("pause");
		return 1;
	}
	//连接服务器
	i_result = connect(sock_client, result->ai_addr, result->ai_addrlen);
	if (i_result == SOCKET_ERROR) {
		cerr << "connect() function failed with error: " << WSAGetLastError() << "\n";
		WSACleanup();
		system("pause");
		return 1;
	}
	cout << "connect server successfully..." << endl;
	//
	freeaddrinfo(result);
	//
//	char send_buf[SEND_BUF_SIZE];
	int send_count = 5;
	//SecureZeroMemory(send_buf, SEND_BUF_SIZE);
	do {
		/*
		cout << "enter the message that you want to send: " << flush;
		cin.getline(send_buf, SEND_BUF_SIZE);
		i_result = send(sock_client, send_buf, static_cast<int>(strlen(send_buf)), 0);
		if (i_result == SOCKET_ERROR) {
			cerr << "send() function failed with error: " << WSAGetLastError() << "\n";
			closesocket(sock_client);
			WSACleanup();
			system("pause");
			return 1;
		}
		cout << "Bytes sent: " << i_result << endl;
		//接收服务器返回的数据
		recv_result = recv(sock_client, send_buf, SEND_BUF_SIZE, 0);
		cout << "Bytes recv: " << recv_result << endl;
		if (recv_result > 0) {
			cout << "feedback from server: " << send_buf << endl;
		}
		else if (recv_result == 0) {
			cout << "connection closed..." << endl;
		}
		else {
			cerr << "recv() function failed with error: " << WSAGetLastError() << "\n";
			closesocket(sock_client);
			WSACleanup();
			system("pause");
			return 1;
		}*/
		MYMessage myMessage;
		myMessage.set_type(UService::MSG::KeepAlive_Request);
		myMessage.set_sequence(3);

		Request * request = myMessage.mutable_request();
		LoginRequest * loginRequest = request->mutable_loginrequest();

		static int userId = 0;
		string username("userId"+userId++) ;
		std::cout << "username is: " << username << std::endl;

		static int passwordId = 0;
		string password("password" + passwordId++);
		std::cout << "password is: " << password << std::endl;


		loginRequest->set_username(username.c_str());
		loginRequest->set_password(password.c_str());


		int body_size = myMessage.ByteSize();
		std::cout << "myMessage body_size is: " << body_size << std::endl;
		char * buf = new char[ PACKET_HEAD_SIZE+ body_size];             //报文的整体长度
		myMessage.SerializeToArray((void *)(buf + PACKET_HEAD_SIZE), body_size);   //设置包体,Protobuf格式
	

		/*设置包头，为4字节，内容为包体长度*/
		*((int *)buf) = htonl(body_size);       

		i_result = send(sock_client, buf, 4+ body_size, 0);
		if (i_result == SOCKET_ERROR) {
			cerr << "send() function failed with error: " << WSAGetLastError() << "\n";
			closesocket(sock_client);
			WSACleanup();
			system("pause");
			return 1;
		}

	//	_itoa_s(size2, buf2,10,10);

	} while (--send_count>0);
	//
	i_result = shutdown(sock_client, SD_SEND);
	if (i_result == SOCKET_ERROR) {
		cerr << "shutdown() function failed with error: " << WSAGetLastError() << "\n";
		closesocket(sock_client);
		WSACleanup();
		system("pause");
		return 1;
	}
	closesocket(sock_client);
	WSACleanup();
	cout << "socket closed..." << endl;
	system("pause");
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
