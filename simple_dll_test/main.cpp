// SimpleDLLTest.cpp : �������̨Ӧ�ó������ڵ㡣  
//http://blog.csdn.net/testcs_dn/article/details/27237509
//#include "stdafx.h"  
#include "../simple_dll/SimpleDLL.h" //���ͷ�ļ�����  
#ifdef _DEBUG
#pragma comment(lib, "../Debug/simple_dll.lib") //���lib�ļ����� 
#else
#pragma comment(lib, "../Release/simple_dll.lib") //���lib�ļ�����
#endif
#include <process.h>  
#include <locale.h> 
#include <stdio.h>

int main(int argc, char* argv[])  
{  
	setlocale(LC_ALL, "chs"); //���õ�����ϢΪ�������ģ������ӡ����������������  
	wprintf(L"getPlayUrl: %s\r\n", getPlayUrl(L"127.0.0.1", 10087, 1));  

	printf("getUrl: %s\r\n", getUrl("127.0.0.1", 10087, 1)); 

	printf("60 + 34 = %d\n", add(60, 34));
	system("pause");  
	return 0;  
}  