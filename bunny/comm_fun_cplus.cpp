#include "stdafx.h"
#include "comm_fun_cplus.h"

#if SHOW_IMAGE
void showImage(cv::Mat &image, int time)
{
	cv::namedWindow("Image");
	cv::imshow("Image", image);
	cv::waitKey(time);
	cv::destroyWindow("Image");
}
#else
void showImage(cv::Mat &image, int time)
{

}
#endif


int writeDataBin(const char *file_name, const void *data, size_t data_len)
{
	int ret = 0;
	FILE *fp = fopen(file_name, "wb+");
	if (NULL == fp) 
	{	
		printf("can't open file\n"); 
		return -1;
	}
	size_t write_size;

	fseek(fp, 0, SEEK_END);
	write_size = fwrite(data, 1, data_len, fp);
	if (write_size != data_len)
	{
		printf("unexpected length\n"); 
		ret = -1;
		goto RETURN;
	}
	else
		ret = write_size;
RETURN:
	fclose(fp);
	return ret;
}

int readDataBin(const char *file_name, void **data, size_t data_len)
{
	int ret = 0;
	FILE *fp = fopen(file_name, "rb+");
	if (NULL == fp) 
	{	
		printf("can't open file\n"); 
		return -1;
	}
	size_t string_size, read_size;
	char *buffer = NULL;

	fseek(fp, 0 ,SEEK_END);  
	string_size = ftell (fp);  
	rewind(fp);
	if (data_len > 0)
	{
		if (data_len > string_size)
		{
			printf("unexpected length\n"); 
			ret = -1;
			goto RETURN;
		}
		else
			string_size = data_len;
	}

	buffer = (char *)malloc(string_size); 
	memset(buffer, 0x0, string_size);
	read_size = fread(buffer, 1 ,string_size, fp);
	if (string_size != read_size) 
	{  
		free(buffer);  
		buffer = NULL;
		ret = -1;
	}
	else
	{
		*data = buffer;
		ret = read_size;
	}
RETURN:
	fclose(fp);
	return ret;
}


//《laws纹理模板在立体匹配中的应用》
//《低对比度医学图像全自动分割算法》
//《textured image segment》
float L5[5] = { 1,  4,  6,  4,  1};
float E5[5] = {-1, -2,  0,  2,  1};
float S5[5] = {-1,  0,  2,  0, -1};
float W5[5] = {-1,  2,  0, -2,  1};
float R5[5] = { 1, -4,  6, -4,  1};

void lawsTexture(cv::Mat &img_src, cv::Mat &img_dst)
{
	cv::Mat mat_l5(1, 5, CV_32FC1, L5);
	cv::Mat mat_e5(1, 5, CV_32FC1, E5);
	cv::Mat mat_s5(1, 5, CV_32FC1, S5);
	cv::Mat mat_w5(1, 5, CV_32FC1, W5);
	cv::Mat mat_r5(1, 5, CV_32FC1, R5);
	cv::Mat *p_mat[5], *p_mat_dst[25];
	p_mat[0] = &mat_l5;
	p_mat[1] = &mat_e5;
	p_mat[2] = &mat_s5;
	p_mat[3] = &mat_w5;
	p_mat[4] = &mat_r5;
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			int index = j + i * 5;
			cv::Mat mat_tran;
			//模板向量转置
			cv::transpose(*p_mat[i], mat_tran);
			//卷积
			p_mat_dst[index] = new cv::Mat;
			cv::filter2D(img_src, *p_mat_dst[index], -1, mat_tran * (*p_mat[j]));
			//绝对值
//			*p_mat_dst[index] = cv::abs(*p_mat_dst[index]);
			cv::threshold(*p_mat_dst[index], *p_mat_dst[index], 0, 255, THRESH_TOZERO);
			
/*
			if (8 == index ||
				12 == index ||
				13 == index ||
				18 == index)
				cv::medianBlur(*p_mat_dst[index], *p_mat_dst[index], 3);
*/
//			cv::GaussianBlur(*p_mat_dst[index], *p_mat_dst[index], cv::Size(5, 5), 2, 2);
			cv::GaussianBlur(*p_mat_dst[index], *p_mat_dst[index], cv::Size(3, 3), 1, 1);
//			showImage( *p_mat_dst[index], 0);
		}
	}
	//求平均值
	cv::Mat img_average(img_src.size(), CV_32FC1, cv::Scalar(0, 0, 0));

#if 0
	//	cv::accumulate(*p_mat_dst[2], img_average); //排除
	//	cv::accumulate(*p_mat_dst[7], img_average);
	cv::accumulate(*p_mat_dst[8], img_average);
	cv::accumulate(*p_mat_dst[9], img_average);
	cv::accumulate(*p_mat_dst[11], img_average);
	cv::accumulate(*p_mat_dst[12], img_average);
	cv::accumulate(*p_mat_dst[12], img_average);
	cv::accumulate(*p_mat_dst[12], img_average);
	cv::accumulate(*p_mat_dst[13], img_average);
	//	cv::accumulate(*p_mat_dst[14], img_average); //排除
	//	cv::accumulate(*p_mat_dst[16], img_average);
	cv::accumulate(*p_mat_dst[17], img_average);
	cv::accumulate(*p_mat_dst[17], img_average);
	cv::accumulate(*p_mat_dst[18], img_average);
	cv::accumulate(*p_mat_dst[19], img_average);
	img_average = img_average / 11;
#elif 1 //最早
//	cv::accumulate(*p_mat_dst[2], img_average); //排除
//	cv::accumulate(*p_mat_dst[7], img_average);
	cv::accumulate(*p_mat_dst[8], img_average);
	cv::accumulate(*p_mat_dst[12], img_average);
	cv::accumulate(*p_mat_dst[13], img_average);
//	cv::accumulate(*p_mat_dst[14], img_average); //排除
//	cv::accumulate(*p_mat_dst[16], img_average);
//	cv::accumulate(*p_mat_dst[17], img_average);
	cv::accumulate(*p_mat_dst[18], img_average);
	img_average = img_average / 4;
#else
//	cv::accumulate(*p_mat_dst[2], img_average); //排除
//	cv::accumulate(*p_mat_dst[7], img_average);
	cv::accumulate(*p_mat_dst[8], img_average);
	cv::accumulate(*p_mat_dst[12], img_average);
	cv::accumulate(*p_mat_dst[12], img_average);
	cv::accumulate(*p_mat_dst[13], img_average);
//	cv::accumulate(*p_mat_dst[14], img_average); //排除
//	cv::accumulate(*p_mat_dst[16], img_average);
//	cv::accumulate(*p_mat_dst[17], img_average);
	cv::accumulate(*p_mat_dst[19], img_average);
	img_average = img_average / 5;
#endif
	img_average.convertTo(img_average, CV_8U);
	//图像分割
	cv::threshold(img_average, img_dst, 0, 255, CV_THRESH_OTSU);

	for (int i = 0; i < 25; i++)
	{
		delete p_mat_dst[i];
	}
}
