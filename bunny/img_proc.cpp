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


void Bunny::procImage()
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


Bunny::Bunny(void)
{
}


Bunny::~Bunny(void)
{
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


