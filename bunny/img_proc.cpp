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
	cv::GaussianBlur(m_imageSrc, image_temp, cv::Size(5, 5), 2, 2);
	cv::pyrDown(m_imageSrc, image_temp);
//	cv::medianBlur(image_temp, m_imageDst, 3);
// 	lbp<UINT8>(image_temp, m_imageDst);
//  cv::Laplacian(image_temp, image_dst, CV_8U, 3);
	lawsTexture(image_temp, m_imageDst);
//	showImage(m_imageDst, 0);
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


