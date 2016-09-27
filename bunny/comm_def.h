#ifndef _COMM_H
#define _COMM_H

#if		0
#define OPENCV1_0
#else
#define OPENCV_OTHER
#endif

#include <basetsd.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdio.h>
#include <iostream>
#include <vector>
#include <process.h>
#include <cstring>
#include <cstringt.h>
#include <afx.h>
#include <sys/types.h>
#include <atlimage.h>
using namespace std;

#ifndef OPENCV1_0
#include <opencv\cv.h>
#include <opencv\cxcore.h>
#include <opencv\highgui.h>
#include <opencv\ml.h>
#include <opencv2\opencv.hpp>
using namespace cv;
#else
#include <cv.h>
#include <cxcore.h>
#include <cxerror.h>
#include <highgui.h>
#include <ml.h>
#include <cxmisc.h>
#endif


#ifndef BOOL
#define BOOL int
#endif

#endif