// prototest.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include <iostream>
#include "message11.pb.h"

int main()
{
	static int seq = 0;
	MYMessage myMessage;
	myMessage.set_type(MSG::KeepAlive_Request);
	myMessage.set_sequence(3);

	Request * request = myMessage.mutable_request();
	LoginRequest * loginRequest = request->mutable_loginrequest();
	loginRequest->set_username("user1");
	loginRequest->set_password("mypasswd");

	myMessage.PrintDebugString();

	int size = myMessage.ByteSize();

	std::cout << "myMessage size is: " << size << std::endl;

	char * buf = new char[size + 4];
	

	myMessage.SerializeToArray((void *)(buf + 4), size);

	MYMessage myMessage2;

	myMessage2.ParseFromArray((void *)(buf + 4), size);

	myMessage2.set_sequence(1);
	myMessage2.mutable_request()->mutable_loginrequest()->set_password("quniafafabi");


	std::cout << "myMessage2 size is: " << myMessage2.ByteSize() << std::endl;


	myMessage2.PrintDebugString();





   // std::cout << "Hello World!\n"; 
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
