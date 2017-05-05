//------------------ vicImgPro.h ----------------  
#pragma once;  

//该宏完成在dll项目内部使用__declspec(dllexport)导出  
//在dll项目外部使用时，用__declspec(dllimport)导入  
//宏DLL_IMPLEMENT在SimpleDLL.cpp中定义  
#ifdef DLL_IMPLEMENT  
#define DLL_API __declspec(dllexport)  
#else  
#define DLL_API __declspec(dllimport)  
#endif
#include "cv_lib.h"

#ifndef complex
typedef struct my_complex 
{
    double x, y;
} COMPLEX;
#define complex COMPLEX
#endif

#ifndef PI
#define PI 3.1415926
#endif

DLL_API int my_add(int x, int y);

DLL_API void showImage(cv::Mat &image, int time, int flag = 0);

DLL_API void saveImage(cv::Mat &image, const char *file_name, int flag = 0);
//DLL_API void saveImage(cv::Mat &image, CString file_name, int flag = 0);
DLL_API void saveImage(cv::Mat &image, const string file_name, int flag = 0);

DLL_API int writeDataBin(const char *file_name, const void *data, size_t data_len);
//should free *data
DLL_API int readDataBin(const char *file_name, void **data, size_t data_len);

DLL_API void binReverse(cv::Mat &src);

DLL_API void erosion(cv::Mat &src, cv::Mat &dst, int type, int size, cv::Mat el = cv::Mat(3, 3, CV_8U, cv::Scalar(1)));

DLL_API void dilation(cv::Mat &src, cv::Mat &dst, int type, int size, cv::Mat el = cv::Mat(3, 3, CV_8U, cv::Scalar(1)));

DLL_API void openClose(cv::Mat &src, cv::Mat &dst, int type, int size, int op, cv::Mat el = cv::Mat(3, 3, CV_8U, cv::Scalar(1)));

DLL_API void lawsTexture(cv::Mat &img_src, cv::Mat &img_dst);

//原始的LBP算法，使用模板参数
//http://blog.csdn.net/dujian996099665/article/details/8886576
template <typename T> 
void lbp(cv::Mat &src, cv::Mat &dst) 
{
	dst.create(src.size(), CV_8UC1);
	// calculate patterns
	for(int i = 1; i < src.rows - 1; i++) 
	{
		for(int j = 1; j < src.cols - 1; j++) 
		{
			T center = src.at<T>(i,j);
			unsigned char code = 0;
			code |= (src.at<T>(i-1,j-1) >= center) << 7;
			code |= (src.at<T>(i-1,j  ) >= center) << 6;
			code |= (src.at<T>(i-1,j+1) >= center) << 5;
			code |= (src.at<T>(i  ,j+1) >= center) << 4;
			code |= (src.at<T>(i+1,j+1) >= center) << 3;
			code |= (src.at<T>(i+1,j  ) >= center) << 2;
			code |= (src.at<T>(i+1,j-1) >= center) << 1;
			code |= (src.at<T>(i  ,j-1) >= center) << 0;
			dst.at<unsigned char>(i-1,j-1) = code;
		}
	}
	dst.row(0).setTo(cv::Scalar(255));
	dst.row(dst.rows - 1).setTo(cv::Scalar(255));
	dst.col(0).setTo(cv::Scalar(255));
	dst.col(dst.cols - 1 ).setTo(cv::Scalar(255));
}

DLL_API int getSizeContours(cv::Mat &img_src, cv::Mat &img_dst);

DLL_API int getSizeContoursGray(cv::Mat &img_src, cv::Mat &img_dst, int area = 4, int y_min = 0, int y_max = 4000);

DLL_API void fillHole(const cv::Mat &src, cv::Mat &dst);

//单幅图像信息熵计算
DLL_API double entropy(const Mat &img_src);

DLL_API double comEntropy(const Mat &img_src1, const Mat &img_src2);

//计算图像的互信息
DLL_API double mutualInfo(const Mat &img_src1, const Mat &img_src2);


/****************************************
 * Function:DFT( )
 * Desc    :
 *          
 * Input   :
 *          const T *r_in, 时域实部
 *          const T *i_in, 时域虚部
 *          const int N, 周期
 * Output  :
 *          double *r_out, 频域实部
 *          double *i_out, 频域虚部
 * Return  :
 * NOTICE  :
****************************************/
template<typename T> 
DLL_API int DFT(const T *r_in, const T *i_in, double *r_out, double *i_out, const int N);

 
/****************************************
 * Function:IDFT( )
 * Desc    :
 *          
 * Input   :
 *          const T *r_in, 频域实部
 *          const T *i_in, 频域虚部
 *          const int N, 周期
 * Output  :
 *          double *r_out, 时域实部
 *          double *i_out, 时域虚部
 * Return  :
 * NOTICE  :
****************************************/
template<typename T>
DLL_API int IDFT(const T *r_in, const T *i_in, double *r_out, double *i_out, const int N);



/****************************************
 * Function:dftLowPass( )
 * Desc    :DFT低通
 *          
 * Input   :
 *          const vector<Point> &point_in, 时域输入
 *          const int N, 周期
 *          const int n, 低通分量实际数目为（2*n）,n>=1
 * Output  :
 *          vector<Point> &point_out, 时域输出
 * Return  :
 * NOTICE  :
****************************************/
DLL_API int dftLowPass(const vector<Point> &point_in, const int N, const int n, vector<Point> &point_out);


/****************************************
 * Function:dftFreqMagnitude( )
 * Desc    :
 *          
 * Input   :
 *          const vector<Point> &point_in, 
 *          const int N, 
 *          const int n, 低通分量实际数目为（2*n）,n>=1
 * Output  :
 *          vector<double> &Mag, 频域幅值
 * Return  :
 * NOTICE  :
****************************************/
DLL_API int dftFreqMagnitude(const vector<Point> &point_in, const int N, const int n, vector<double> &Mag);
