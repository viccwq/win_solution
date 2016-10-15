#ifndef _COMM_FUN_CPLUS
#define _COMM_FUN_CPLUS
#include "comm_def.h"
#include <vector>

#define SHOW_IMAGE			1
#define SAVE_IMAGE			1

void showImage(cv::Mat &image, int time);

void saveImage(cv::Mat &image, const char *file_name);
void saveImage(cv::Mat &image, CString file_name);
void saveImage(cv::Mat &image, const string file_name);

int writeDataBin(const char *file_name, const void *data, size_t data_len);
//should free *data
int readDataBin(const char *file_name, void **data, size_t data_len);

void binReverse(cv::Mat &src);

void erosion(cv::Mat &src, cv::Mat &dst, int type, int size);

void dilation(cv::Mat &src, cv::Mat &dst, int type, int size);

void openClose(cv::Mat &src, cv::Mat &dst, int type, int size, int open_close_flag);

void lawsTexture(cv::Mat &img_src, cv::Mat &img_dst);


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

int getSizeContours(cv::Mat &img_src, cv::Mat &img_dst);

int test( Mat &img_src);

#endif