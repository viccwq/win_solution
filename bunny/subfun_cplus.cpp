#include "stdafx.h"
#include "subfun_cplus.h"

void vic::salt(cv::Mat &image, int n)
{
	for (int k = 0; k < n; k++)
	{
		int i = rand() % image.cols;
		int j = rand() % image.rows;
		if (1 == image.channels())
			image.at<UINT8>(j, i) = 255;
		else
		{
			image.at<cv::Vec3b>(j, i)[0] = 255;
			image.at<cv::Vec3b>(j, i)[1] = 255;
			image.at<cv::Vec3b>(j, i)[2] = 255;
		}
	}
}

void vic::colorReduce(const cv::Mat &image_in, cv::Mat &image_out, int div/* = 64*/)
{
	int y = image_in.rows;
	int x = image_in.cols * image_in.channels();
	//验证矩阵大小是否一致,如有必要则分配图像
	image_out.create(image_in.rows, image_in.cols, image_in.type());
	//mask预算
	int n = static_cast<int>(log(static_cast<double>(div)) / log(2.0));
	UINT8 mask = 0xff << n;
	for (int j = 0; j < y; j++)
	{
		//每行首地址
		const UINT8 *data = image_in.ptr<UINT8>(j);
		UINT8 *data1 = image_out.ptr<UINT8>(j);
		for (int i = 0; i < x; i++)
		{
			data1[i] = data[i] & mask + div / 2;
		}
	}
}

void vic::sharpen(const cv::Mat &image_in, cv::Mat &image_out)
{
	//验证矩阵大小是否一致,如有必要则分配图像
	image_out.create(image_in.size(), image_in.type());
	for (int j = 1; j < image_in.rows - 1; j++)
	{
		const UINT8 *previous = image_in.ptr<UINT8>(j - 1);
		const UINT8 *current = image_in.ptr<UINT8>(j);
		const UINT8 *next = image_in.ptr<UINT8>(j + 1);

		UINT8 *output = image_out.ptr<UINT8>(j);
		for (int i = 1; i < image_in.cols; i++)
		{
			//cv::saturate_cast截断数值0~255
			*output ++= cv::saturate_cast<UINT8>(
				5 * current[i] - 
				current[i - 1] - current [i + 1] - 
				previous[i] - next [i]);	
		}
	}
	//方法row（）col（）没有数据拷贝
	image_out.row(0).setTo(cv::Scalar(0));
	image_out.row(image_out.rows - 1).setTo(cv::Scalar(0));
	image_out.col(0).setTo(cv::Scalar(0));
	image_out.col(image_out.cols - 1).setTo(cv::Scalar(0));
}

void vic::sharpen2D(const cv::Mat &image_in, cv::Mat &image_out)
{
	cv::Mat kernel(3, 3, CV_32F, cv::Scalar(0));
	kernel.at<float>(1, 1) = 5.0;
	kernel.at<float>(0, 1) = -1.0;
	kernel.at<float>(2, 1) = -1.0;
	kernel.at<float>(1, 0) = -1.0;
	kernel.at<float>(1, 2) = -1.0;
	cv::filter2D(image_in, image_out, image_in.depth(), kernel);
}


void vic::getHarris(const cv::Mat &image_in, cv::Mat &image_out)
{
	cv::Mat image_gray;
	cv::cvtColor(image_in, image_gray, CV_BGR2GRAY);

	image_out.create(image_in.size(), image_in.type());
	HarrisDetector harris;
	harris.detect(image_gray);
	std::vector<cv::Point>pts;
	harris.getCorners(pts, 0.01);
	image_in.copyTo(image_out);
	harris.drawOnImage(image_out, pts);
}

