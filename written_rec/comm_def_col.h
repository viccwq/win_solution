#ifndef _COMM_DEF_H
#define _COMM_DEF_H

#ifdef __cplusplus
#include <iostream>
#include <string>
#include <fstream>
#include <ctime>
#include <vector>
using namespace std;
#else
//#include <Windows.h>  
#include <intrin.h>
#include <basetsd.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdio.h>
#include <process.h>
#include <cstringt.h>
//#include <afx.h>
#include <sys/types.h>
#include <atlimage.h>
#endif

#include "../vicImgPro/vicImgPro.h"
#include "../vicImgPro/cv_lib.h"
#ifdef _DEBUG
#pragma comment(lib, "../Debug/vicImgPro.lib") //���lib�ļ����� 
#else
#pragma comment(lib, "../Release/vicImgPro.lib") //���lib�ļ�����
#endif

#ifdef _DEBUG
#define SAVE_FLAG				(1)
#else
#define SAVE_FLAG				(0)
#endif

#endif