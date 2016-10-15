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

#if SAVE_IMAGE
void saveImage(cv::Mat &image, const char *file_name)
{
	cv::imwrite(file_name, image);
}

void saveImage(cv::Mat &image, CString file_name)
{
	cv::imwrite(file_name.GetBuffer(0), image);
}

void saveImage(cv::Mat &image, const string file_name)
{
	cv::imwrite(file_name, image);
}
#else
void saveImage(cv::Mat &image, void)
{

}
#endif

/**  @function Erosion  */
void erosion(cv::Mat &src, cv::Mat &dst, int type, int size)
{
	int erosion_type;
	if( type == 0 ){ erosion_type = MORPH_RECT; }
	else if( type == 1 ){ erosion_type = MORPH_CROSS; }
	else if( type == 2) { erosion_type = MORPH_ELLIPSE; }
	else { erosion_type = MORPH_RECT; }

	Mat element = cv::getStructuringElement(erosion_type,
		Size( 2*size + 1, 2*size+1 ),
		Point( size, size ) );
	/// ��ʴ����
	cv::erode( src, dst, element);
}

void binReverse(cv::Mat &src)
{
	src = 255 - src;
}

/** @function Dilation */
void dilation(cv::Mat &src, cv::Mat &dst, int type, int size)
{
	int dilation_type;
	if( type == 0 ){ dilation_type = MORPH_RECT; }
	else if( type == 1 ){ dilation_type = MORPH_CROSS; }
	else if( type == 2) { dilation_type = MORPH_ELLIPSE; }
	else { dilation_type = MORPH_RECT; }

	Mat element = cv::getStructuringElement( dilation_type,
		Size( 2*size + 1, 2*size+1 ),
		Point( size, size ) );
	/// ���Ͳ���
	cv::dilate( src, dst, element );
}

void openClose(cv::Mat &src, cv::Mat &dst, int type, int size, int open_close_flag)
{  
	int oc_type;
	if( type == 0 ){ oc_type = MORPH_RECT; }
	else if( type == 1 ){ oc_type = MORPH_CROSS; }
	else if( type == 2) { oc_type = MORPH_ELLIPSE; }
	else { oc_type = MORPH_RECT; }

	Mat element = cv::getStructuringElement( oc_type,
		Size( 2*size + 1, 2*size+1 ),
		Point( size, size ) ); 

	if( !open_close_flag )  
	{  
		cv::erode( src, dst, element);
		cv::dilate( dst, dst, element );
	}  
	else  
	{  
		cv::dilate( src, dst, element );
		cv::erode( dst, dst, element);
	}  
}


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


//��laws����ģ��������ƥ���е�Ӧ�á�
//���ͶԱȶ�ҽѧͼ��ȫ�Զ��ָ��㷨��
//��textured image segment��
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
			//ģ������ת��
			cv::transpose(*p_mat[i], mat_tran);
			//���
			p_mat_dst[index] = new cv::Mat;
			cv::filter2D(img_src, *p_mat_dst[index], -1, mat_tran * (*p_mat[j]));
			//����ֵ
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
	//��ƽ��ֵ
	cv::Mat img_average(img_src.size(), CV_32FC1, cv::Scalar(0, 0, 0));

#if 0
	//	cv::accumulate(*p_mat_dst[2], img_average); //�ų�
	//	cv::accumulate(*p_mat_dst[7], img_average);
	cv::accumulate(*p_mat_dst[8], img_average);
	cv::accumulate(*p_mat_dst[9], img_average);
	cv::accumulate(*p_mat_dst[11], img_average);
	cv::accumulate(*p_mat_dst[12], img_average);
	cv::accumulate(*p_mat_dst[12], img_average);
	cv::accumulate(*p_mat_dst[12], img_average);
	cv::accumulate(*p_mat_dst[13], img_average);
	//	cv::accumulate(*p_mat_dst[14], img_average); //�ų�
	//	cv::accumulate(*p_mat_dst[16], img_average);
	cv::accumulate(*p_mat_dst[17], img_average);
	cv::accumulate(*p_mat_dst[17], img_average);
	cv::accumulate(*p_mat_dst[18], img_average);
	cv::accumulate(*p_mat_dst[19], img_average);
	img_average = img_average / 11;
#elif 1 //����
//	cv::accumulate(*p_mat_dst[2], img_average); //�ų�
//	cv::accumulate(*p_mat_dst[7], img_average);
	cv::accumulate(*p_mat_dst[8], img_average);
	cv::accumulate(*p_mat_dst[12], img_average);
	cv::accumulate(*p_mat_dst[13], img_average);
//	cv::accumulate(*p_mat_dst[14], img_average); //�ų�
//	cv::accumulate(*p_mat_dst[16], img_average);
//	cv::accumulate(*p_mat_dst[17], img_average);
	cv::accumulate(*p_mat_dst[18], img_average);
	img_average = img_average / 4;
#else
//	cv::accumulate(*p_mat_dst[2], img_average); //�ų�
//	cv::accumulate(*p_mat_dst[7], img_average);
	cv::accumulate(*p_mat_dst[8], img_average);
	cv::accumulate(*p_mat_dst[12], img_average);
	cv::accumulate(*p_mat_dst[12], img_average);
	cv::accumulate(*p_mat_dst[13], img_average);
//	cv::accumulate(*p_mat_dst[14], img_average); //�ų�
//	cv::accumulate(*p_mat_dst[16], img_average);
//	cv::accumulate(*p_mat_dst[17], img_average);
	cv::accumulate(*p_mat_dst[19], img_average);
	img_average = img_average / 5;
#endif
	img_average.convertTo(img_average, CV_8U);
	//ͼ��ָ�
	cv::threshold(img_average, img_dst, 0, 255, CV_THRESH_OTSU);

	for (int i = 0; i < 25; i++)
	{
		delete p_mat_dst[i];
	}
}

//http://blog.sina.com.cn/s/blog_662c78590100z0rg.html
//http://blog.csdn.net/lu597203933/article/details/17362457
//http://www.opencv.org.cn/opencvdoc/2.3.2/html/doc/tutorials/imgproc/shapedescriptors/moments/moments.html#moments
// ������ͨ���������������ֵͼ�����������ص���״  
int getSizeContours(cv::Mat &img_src, cv::Mat &img_dst)
{  
	if(!img_src.data)  
	{  
		cout << "Fail to load image" << endl;  
		return 0;  
	}
// 	cv::Mat temp =  img_src.clone();
	cv::Mat result(img_src.size(), CV_8UC3, Scalar(255, 255,255));  

	//�����������飬ÿһ��������һ��point���͵�vector��ʾ
	cv::vector<cv::vector<cv::Point>> contours;
	//������������ͬ��ÿ������contours[ i ]��Ӧ4��hierarchyԪ��hierarchy[ i ][ 0 ] ~hierarchy[ i ][ 3 ]���ֱ��ʾ��һ��������ǰһ������������������Ƕ������������ţ����û�ж�Ӧ���ֵ����Ϊ������
	cv::vector<cv::Vec4i> hierarchy;
	//CV_CHAIN_APPROX_NONE  ��ȡÿ������ÿ�����ص� 
	//CV_CHAIN_APPROX_SIMPLEѹ��ˮƽ���򣬴�ֱ���򣬶Խ��߷����Ԫ�أ�ֻ�����÷�����յ����꣬����һ����������ֻ��4����������������Ϣ
 	cv::findContours(img_src, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_NONE);
//	cv::findContours(img_src, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);
	cout << contours.size() << endl;
	//findContours������drawContours���ʹ�ã��������������Ƴ��������е�һ������image��ʾĿ��ͼ�񣬵ڶ�������contours��ʾ����������飬ÿһ�������ɵ�vector���ɣ�����������contourIdxָ�����ڼ�������������ò���Ϊ��ֵ����ȫ�����������ĸ�����colorΪ��������ɫ�����������thicknessΪ�������߿����Ϊ��ֵ��CV_FILLED��ʾ��������ڲ�������������lineTypeΪ���ͣ����߸�����Ϊ�����ṹ��Ϣ���ڰ˸�����ΪmaxLevel
	if( !contours.empty() && !hierarchy.empty() )
	{
		int idx = 0;
		for( ; idx >= 0; idx = hierarchy[idx][0] )
		{
			cv::Scalar color( (rand()&255), (rand()&255), (rand()&255) );
			cv::drawContours(result, contours, idx, color, 1, 8, hierarchy);
		}
	}
	showImage(result, 0);

	return 0;  
}  

int test( Mat &img_src)
{
	Mat src = img_src.clone();


	Mat dst = Mat::zeros(src.rows, src.cols, CV_8UC3);

	src = src > 1;
	namedWindow( "Source", 1 );
	imshow( "Source", src );

	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;

	findContours( src, contours, hierarchy,
		CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE );

	// iterate through all the top-level contours,
	// draw each connected component with its own random color
	int idx = 0;
	for( ; idx >= 0; idx = hierarchy[idx][0] )
	{
		Scalar color( rand()&255, rand()&255, rand()&255 );
		drawContours( dst, contours, idx, color, CV_FILLED, 8, hierarchy );
	}

	namedWindow( "Components", 1 );
	imshow( "Components", dst );
	waitKey(0);
	return 0;
}
