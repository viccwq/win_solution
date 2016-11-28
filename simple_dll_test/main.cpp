// SimpleDLLTest.cpp : 定义控制台应用程序的入口点。  
//http://blog.csdn.net/testcs_dn/article/details/27237509
//#include "stdafx.h"  
#include "../simple_dll/SimpleDLL.h" //添加头文件引用  
#ifdef _DEBUG
#pragma comment(lib, "../Debug/simple_dll.lib") //添加lib文件引用 
#else
#pragma comment(lib, "../Release/simple_dll.lib") //添加lib文件引用
#endif
#include <process.h>  
#include <locale.h> 
#include <stdio.h>

int main(int argc, char* argv[])  
{  
	setlocale(LC_ALL, "chs"); //配置地域化信息为简体中文，否则打印出来的中文是乱码  
	wprintf(L"getPlayUrl: %s\r\n", getPlayUrl(L"127.0.0.1", 10087, 1));  

	printf("getUrl: %s\r\n", getUrl("127.0.0.1", 10087, 1)); 

	printf("60 + 34 = %d\n", add(60, 34));
	system("pause");  
	return 0;  
}  