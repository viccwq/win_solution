#ifndef _CV_LIB_H
#define _CV_LIB_H

#if		0
#define OPENCV1_0
#else
#define OPENCV_OTHER
#endif

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

#ifndef OPENCV1_0

#ifdef _DEBUG
//official
//#define LIB_PATH "D:/Program Files (x86)/opencv2.4.9/opencv/build/x86/vc10/lib/"
//custom compiled
#define LIB_PATH "D:/Program Files (x86)/opencv2.4.9/opencv/build_vs10/lib/Debug/"

#pragma comment(lib, LIB_PATH"opencv_ml249d.lib")
#pragma comment(lib, LIB_PATH"opencv_calib3d249d.lib")
#pragma comment(lib, LIB_PATH"opencv_contrib249d.lib")
#pragma comment(lib, LIB_PATH"opencv_core249d.lib")
#pragma comment(lib, LIB_PATH"opencv_features2d249d.lib")
#pragma comment(lib, LIB_PATH"opencv_flann249d.lib")
#pragma comment(lib, LIB_PATH"opencv_gpu249d.lib")
#pragma comment(lib, LIB_PATH"opencv_highgui249d.lib")
#pragma comment(lib, LIB_PATH"opencv_imgproc249d.lib")
#pragma comment(lib, LIB_PATH"opencv_legacy249d.lib")
#pragma comment(lib, LIB_PATH"opencv_objdetect249d.lib")
#pragma comment(lib, LIB_PATH"opencv_ts249d.lib")
#pragma comment(lib, LIB_PATH"opencv_video249d.lib")
#pragma comment(lib, LIB_PATH"opencv_nonfree249d.lib")
#pragma comment(lib, LIB_PATH"opencv_ocl249d.lib")
#pragma comment(lib, LIB_PATH"opencv_photo249d.lib")
#pragma comment(lib, LIB_PATH"opencv_stitching249d.lib")
#pragma comment(lib, LIB_PATH"opencv_superres249d.lib")
#pragma comment(lib, LIB_PATH"opencv_videostab249d.lib")
#else
//official
//#define LIB_PATH "D:/Program Files (x86)/opencv2.4.9/opencv/build/x86/vc10/lib/"
//custom compiled
#define LIB_PATH "D:/Program Files (x86)/opencv2.4.9/opencv/build_vs10/lib/Release/"

#pragma comment(lib, LIB_PATH"opencv_ml249.lib")
#pragma comment(lib, LIB_PATH"opencv_calib3d249.lib")
#pragma comment(lib, LIB_PATH"opencv_contrib249.lib")
#pragma comment(lib, LIB_PATH"opencv_core249.lib")
#pragma comment(lib, LIB_PATH"opencv_features2d249.lib")
#pragma comment(lib, LIB_PATH"opencv_flann249.lib")
#pragma comment(lib, LIB_PATH"opencv_gpu249.lib")
#pragma comment(lib, LIB_PATH"opencv_highgui249.lib")
#pragma comment(lib, LIB_PATH"opencv_imgproc249.lib")
#pragma comment(lib, LIB_PATH"opencv_legacy249.lib")
#pragma comment(lib, LIB_PATH"opencv_objdetect249.lib")
#pragma comment(lib, LIB_PATH"opencv_ts249.lib")
#pragma comment(lib, LIB_PATH"opencv_video249.lib")
#pragma comment(lib, LIB_PATH"opencv_nonfree249.lib")
#pragma comment(lib, LIB_PATH"opencv_ocl249.lib")
#pragma comment(lib, LIB_PATH"opencv_photo249.lib")
#pragma comment(lib, LIB_PATH"opencv_stitching249.lib")
#pragma comment(lib, LIB_PATH"opencv_superres249.lib")
#pragma comment(lib, LIB_PATH"opencv_videostab249.lib")
#endif  
#endif


#endif