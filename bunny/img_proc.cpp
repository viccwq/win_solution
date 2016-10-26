#include "stdafx.h"
#include "img_proc.h"

int Bunny::loadData(const char *file_name)
{

	if (file_name == NULL)
		return -1;
	m_imageRead.create(BUNNY_H, BUNNY_W, CV_8UC1);
	int ret = 0;
	int rows = m_imageRead.rows;
	int cols = m_imageRead.cols * m_imageRead.channels();


	//¶ÁÈ¡ÎÄ¼þ
	UINT8 *buff = NULL;
	ret = readDataBin(file_name, (void **)(&buff), 0);
	if (ret < 0)
	{
		ret = -1;
		goto RETURN;
	}

	if (m_imageRead.isContinuous())
	{
		cols = rows * cols;
		rows = 1;
	}
	int i, j;
	UINT8 *data_row = NULL;
	UINT16 data_bunny;
	int index = 0;
	for (i = 0; i < rows; i++)
	{
		data_row = m_imageRead.ptr<UINT8>(i);
		for (j = 0; j < cols; j++)
		{
			memcpy(&data_bunny, buff + index , 2);
			index += 2;
			data_row[j] = (UINT8)(data_bunny >> 8);
		}
	}
RETURN:
	if (NULL == buff)
	{
		free(buff);
		buff = NULL;
	}
	return ret;
}

void Bunny::imageSegment(cv::Mat &img_src, cv::Mat &img_dst)
{
	cv::Mat image_temp;
//	cv::GaussianBlur(img_src, image_temp, cv::Size(5, 5), 2, 2);
	cv::pyrDown(img_src, image_temp);
	lawsTexture(image_temp, image_temp);
	//fill the hole
	openClose(image_temp, image_temp, 0, 2, cv::MORPH_CLOSE);
	saveImage(image_temp, "./img_bunny_deb/s02_close.bmp", SAVE_FLAG);
	//select the component
	erosion(image_temp, image_temp, 0, 1);
	erosion(image_temp, image_temp, 0, 1);
// 	saveImage(image_temp, "./img_bunny_deb/s03_erode.bmp", SAVE_FLAG);
	getSizeContoursGray(image_temp, image_temp, 2, 0, 220);
// 	saveImage(image_temp, "./img_bunny_deb/s04_contours.bmp", SAVE_FLAG);
	dilation(image_temp, image_temp, 0, 1);
	dilation(image_temp, image_temp, 0, 1);
// 	saveImage(image_temp, "./img_bunny_deb/s05_dilate.bmp", SAVE_FLAG);

	//select again
	getSizeContoursGray(image_temp, image_temp, 100, 0, 220);

	//fill the hole again
/*
	openClose(image_temp, image_temp, 2, 3, cv::MORPH_CLOSE);
	saveImage(image_temp, "./img_bunny_deb/s06_close.bmp", SAVE_FLAG);
*/
	//filter
	cv::GaussianBlur(image_temp, image_temp, cv::Size(7, 7), 5);
	cv::threshold(image_temp, img_dst, 0, 255, CV_THRESH_OTSU);
	saveImage(img_dst, "./img_bunny_deb/s07_OTSU.bmp", SAVE_FLAG);

}

void Bunny::procImage_old()
{
	cv::Mat image_temp;
	cv::Mat image_temp_2;
	cv::Mat image_delta;
	cv::GaussianBlur(m_imageSrc, image_temp, cv::Size(5, 5), 2, 2);
	cv::pyrDown(m_imageSrc, image_temp);
//	cv::medianBlur(image_temp, m_imageDst, 3);
// 	lbp<UINT8>(image_temp, m_imageDst);
//  cv::Laplacian(image_temp, image_dst, CV_8U, 3);
	lawsTexture(image_temp, image_temp);
	dilation(image_temp, image_temp, 0, 1);
	erosion(image_temp, image_temp, 0, 1);
	saveImage(image_temp, "./img_bunny_deb/bin.bmp");

	image_delta = cv::imread("./img_bunny_deb/delta.bmp", -1);
	image_temp_2 = image_temp - image_delta;

	openClose(image_temp_2, image_temp_2, 1, 1, cv::MORPH_OPEN);
	openClose(image_temp_2, image_temp_2, 0, 1, cv::MORPH_CLOSE);
	saveImage(image_temp_2, "./img_bunny_deb/close.bmp");

	dilation(image_temp_2, image_temp_2, 0, 1);
	image_delta = image_temp - image_temp_2;
	getSizeContoursGray(image_delta, image_delta, 1, 190);
	dilation(image_delta, image_delta, 0, 1);
	saveImage(image_delta, "./img_bunny_deb/delta.bmp");
}


void Bunny::procImage()
{
	cv::Mat front;
	cv::Mat back;
	cv::Mat source;
	cv::Mat mask;
	cv::Mat mask_2;

	//generate mask
	mask = cv::imread("./img_bunny_deb/delta.bmp", -1);
// 	dilation(mask, mask, 1, 1);
	cv::pyrUp(mask, mask_2);
	cv::threshold(mask_2, mask_2, 0, 255, CV_THRESH_OTSU);
	fillHole(mask_2, mask_2);
	saveImage(mask_2, "./img_bunny_deb/s20_fill.bmp", SAVE_FLAG);
	m_imageSrc.copyTo(front, mask_2);
	m_imageSrc.copyTo(back);
	back.setTo(0, mask_2);
	//adjust the resource image
// 	saveImage(front, "./img_bunny_deb/s21_front.bmp", SAVE_FLAG);
// 	saveImage(back, "./img_bunny_deb/s22_back.bmp", SAVE_FLAG);
	source.create(mask_2.size(), CV_32FC1);
	source.setTo(0.0);
	cv::accumulate(m_imageSrc, source);
	cv::accumulate(m_imageSrc, source);
	cv::accumulate(front, source);
	source = source / 3;
	source.convertTo(source, CV_8UC1);
	saveImage(source, "./img_bunny_deb/s23_source.bmp", SAVE_FLAG);
	//call segment
	cv::Mat image_temp;
	cv::Mat image_delta;
	imageSegment(source, image_temp);
	saveImage(image_temp, "./img_bunny_deb/temp.bmp", SAVE_FLAG);
	//get previous image
	image_delta = cv::imread("./img_bunny_deb/delta.bmp", -1);
	dilation(image_delta, image_delta, 1, 1);
	cv::bitwise_and(image_temp, image_delta, image_temp);
	cv::Mat element(cv::Size(5, 3), CV_8UC1, cv::Scalar(1));
	openClose(image_temp, image_temp, -1, 3, cv::MORPH_CLOSE, element);
	saveImage(image_temp, "./img_bunny_deb/s13_close.bmp", SAVE_FLAG);
	openClose(image_temp, m_imageDst, 2, 7, cv::MORPH_CLOSE);
	saveImage(m_imageDst, "./img_bunny_deb/s14_close.bmp", SAVE_FLAG);
	saveImage(m_imageDst, "./img_bunny_deb/delta.bmp", SAVE_FLAG);

#if 0
	cv::Mat image_temp;
	cv::Mat image_delta;
	imageSegment(m_imageSrc, image_temp);
	//get previous image
	image_delta = cv::imread("./img_bunny_deb/delta.bmp", -1);
	dilation(image_delta, image_delta, 1, 1);
	saveImage(image_delta, "./img_bunny_deb/s12_dilate.bmp", SAVE_FLAG);
	cv::bitwise_and(image_temp, image_delta, image_temp);
	cv::Mat element(cv::Size(5, 3), CV_8UC1, cv::Scalar(1));
	openClose(image_temp, image_temp, -1, 3, cv::MORPH_CLOSE, element);
	saveImage(image_temp, "./img_bunny_deb/s13_close.bmp", SAVE_FLAG);
	openClose(image_temp, m_imageDst, 2, 7, cv::MORPH_CLOSE);
	saveImage(m_imageDst, "./img_bunny_deb/s14_close.bmp", SAVE_FLAG);
	saveImage(m_imageDst, "./img_bunny_deb/delta.bmp", SAVE_FLAG);
#endif
}


void Bunny::procImageSingle()
{
	imageSegment(m_imageSrc, m_imageDst);
/*
	cv::Mat image_temp;
	cv::GaussianBlur(m_imageSrc, image_temp, cv::Size(5, 5), 2, 2);
	cv::pyrDown(m_imageSrc, image_temp);
	lawsTexture(image_temp, image_temp);
	openClose(image_temp, image_temp, 0, 1, cv::MORPH_CLOSE);
	saveImage(image_temp, "./img_bunny_deb/s2_close.bmp", SAVE_FLAG);
	erosion(image_temp, image_temp, 0, 1);
	saveImage(image_temp, "./img_bunny_deb/s3_erode.bmp", SAVE_FLAG);
	getSizeContoursGray(image_temp, image_temp, 20, 0, 212);
	saveImage(image_temp, "./img_bunny_deb/s4_contours.bmp", SAVE_FLAG);
	dilation(image_temp, image_temp, 0, 1);
	saveImage(image_temp, "./img_bunny_deb/s5_dilate.bmp", SAVE_FLAG);
	openClose(image_temp, m_imageDst, 2, 3, cv::MORPH_CLOSE);
	saveImage(m_imageDst, "./img_bunny_deb/s6_close.bmp", SAVE_FLAG);
*/
}


Bunny::Bunny(void)
{
}


Bunny::~Bunny(void)
{
}


int getBunnySingle(const char *file_name, cv::Mat &image_src, cv::Mat &image_dst)
{
	Bunny bunny;
	bunny.loadData(file_name);
	bunny.resizeSrcImage();
	bunny.procImageSingle();
	bunny.getSrcImage(image_src);
	bunny.getDstImage(image_dst);
	return 0;
}

int getBunny(const char *file_name, cv::Mat &image_src, cv::Mat &image_dst)
{
	Bunny bunny;
	bunny.loadData(file_name);
	bunny.resizeSrcImage();
	bunny.procImage();
	bunny.getSrcImage(image_src);
	bunny.getDstImage(image_dst);
	return 0;
}


