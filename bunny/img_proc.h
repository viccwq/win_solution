#ifndef _IMG_PROC_H
#define _IMG_PROC_H
#include "comm_def.h"


#define BUNNY_H				512
#define BUNNY_W				512
#if 0
#define BUNNY_H_CUT			422
#define BUNNY_W_CUT			512
#else
#define BUNNY_H_CUT			512
#define BUNNY_W_CUT			512
#endif
#define BUNNY_P_SIZE		2
#define BUNNY_SIZE			524288


class Bunny
{
public:
	Bunny(void);
	~Bunny(void);
	int loadData(const char *file_name);
	void resizeSrcImage()
	{
		m_imageSrc = m_imageRead((cv::Rect(0, 0, BUNNY_W_CUT, BUNNY_H_CUT)));
	}
	void getSrcImage(cv::Mat &image)
	{
		image = m_imageSrc.clone();
	}
	void getDstImage(cv::Mat &image)
	{
		image = m_imageDst.clone();
	}
	void procImage();
	void procImage_old();
	void procImageSingle();
private:
	cv::Mat m_imageRead;
	cv::Mat m_imageSrc;
	cv::Mat m_imageDst;
	void imageSegment(cv::Mat &img_src, cv::Mat &img_dst);
};

extern int getBunny(const char *file_name, cv::Mat &image, cv::Mat &image_dst);
extern int getBunnySingle(const char *file_name, cv::Mat &image, cv::Mat &image_dst);
#endif