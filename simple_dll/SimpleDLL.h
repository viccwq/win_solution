//------------------ SimpleDLL.h ----------------  
//http://blog.csdn.net/testcs_dn/article/details/27237509
#pragma once;  

//�ú������dll��Ŀ�ڲ�ʹ��__declspec(dllexport)����  
//��dll��Ŀ�ⲿʹ��ʱ����__declspec(dllimport)����  
//��DLL_IMPLEMENT��SimpleDLL.cpp�ж���  
#ifdef DLL_IMPLEMENT  
#define DLL_API __declspec(dllexport)  
#else  
#define DLL_API __declspec(dllimport)  
#endif  
DLL_API int add(int x, int y); //�򵥷���  
DLL_API const wchar_t* getPlayUrl(const wchar_t* mgrIp, long mgrPort, long materialId);  
DLL_API const char* getUrl(const char* mgrIp, long mgrPort, long materialId);  