//------------------ vicImgPro.h ----------------  
#pragma once;  

//�ú������dll��Ŀ�ڲ�ʹ��__declspec(dllexport)����  
//��dll��Ŀ�ⲿʹ��ʱ����__declspec(dllimport)����  
//��DLL_IMPLEMENT��SimpleDLL.cpp�ж���  
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

//ԭʼ��LBP�㷨��ʹ��ģ�����
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

//����ͼ����Ϣ�ؼ���
DLL_API double entropy(const Mat &img_src);

DLL_API double comEntropy(const Mat &img_src1, const Mat &img_src2);

//����ͼ��Ļ���Ϣ
DLL_API double mutualInfo(const Mat &img_src1, const Mat &img_src2);


/****************************************
 * Function:DFT( )
 * Desc    :
 *          
 * Input   :
 *          const T *r_in, ʱ��ʵ��
 *          const T *i_in, ʱ���鲿
 *          const int N, ����
 * Output  :
 *          double *r_out, Ƶ��ʵ��
 *          double *i_out, Ƶ���鲿
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
 *          const T *r_in, Ƶ��ʵ��
 *          const T *i_in, Ƶ���鲿
 *          const int N, ����
 * Output  :
 *          double *r_out, ʱ��ʵ��
 *          double *i_out, ʱ���鲿
 * Return  :
 * NOTICE  :
****************************************/
template<typename T>
DLL_API int IDFT(const T *r_in, const T *i_in, double *r_out, double *i_out, const int N);



/****************************************
 * Function:dftLowPass( )
 * Desc    :DFT��ͨ
 *          
 * Input   :
 *          const vector<Point> &point_in, ʱ������
 *          const int N, ����
 *          const int n, ��ͨ����ʵ����ĿΪ��2*n��,n>=1
 * Output  :
 *          vector<Point> &point_out, ʱ�����
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
 *          const int n, ��ͨ����ʵ����ĿΪ��2*n��,n>=1
 * Output  :
 *          vector<double> &Mag, Ƶ���ֵ
 * Return  :
 * NOTICE  :
****************************************/
DLL_API int dftFreqMagnitude(const vector<Point> &point_in, const int N, const int n, vector<double> &Mag);
