//------------------ SimpleDLL.h ----------------  
//http://blog.csdn.net/testcs_dn/article/details/27237509
#pragma once;  

//该宏完成在dll项目内部使用__declspec(dllexport)导出  
//在dll项目外部使用时，用__declspec(dllimport)导入  
//宏DLL_IMPLEMENT在SimpleDLL.cpp中定义  
#ifdef DLL_IMPLEMENT  
#define DLL_API __declspec(dllexport)  
#else  
#define DLL_API __declspec(dllimport)  
#endif  
DLL_API int add(int x, int y); //简单方法  
DLL_API const wchar_t* getPlayUrl(const wchar_t* mgrIp, long mgrPort, long materialId);  
DLL_API const char* getUrl(const char* mgrIp, long mgrPort, long materialId);  