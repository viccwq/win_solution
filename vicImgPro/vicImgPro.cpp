//------------------ vicImgPro.cpp ----------------  
//http://blog.csdn.net/testcs_dn/article/details/27237509
//ע��˴��ĺ궨����Ҫд��#include "SimpleDLL.h"֮ǰ  
//�������dll��Ŀ�ڲ�ʹ��__declspec(dllexport)����  
//��dll��Ŀ�ⲿʹ��ʱ����__declspec(dllimport)����  
#define DLL_IMPLEMENT  

#include "vicImgPro.h"  
using namespace std;

DLL_API int my_add(int x, int y)  
{  
	return x+y;  
}  


DLL_API void showImage(cv::Mat &image, int time, int flag)
{
	if (!flag)
		return;
	cv::namedWindow("Image");
	cv::imshow("Image", image);
	cv::waitKey(time);
	cv::destroyWindow("Image");
}


DLL_API void saveImage(cv::Mat &image, const char *file_name, int flag)
{
	if (!flag)
		return;
	cv::imwrite(file_name, image);
}

/*
DLL_API void saveImage(cv::Mat &image, CString file_name, int flag)
{
	if (!flag)
		return;
	cv::imwrite(file_name.GetBuffer(0), image);
}*/

DLL_API void saveImage(cv::Mat &image, const string file_name, int flag)
{
	if (!flag)
		return;
	cv::imwrite(file_name, image);
}


/**  @function Erosion  */
DLL_API void erosion(cv::Mat &src, cv::Mat &dst, int type, int size, cv::Mat el)
{
	int erosion_type;
	cv::Mat element;
	if( type == 0 ){ erosion_type = MORPH_RECT; }
	else if( type == 1 ){ erosion_type = MORPH_CROSS; }
	else if( type == 2) { erosion_type = MORPH_ELLIPSE; }
	else { erosion_type = MORPH_RECT; }

	if (type >= 0)
		element = cv::getStructuringElement(erosion_type, 
		cv::Size( 2*size + 1, 2*size+1 ), 
		cv::Point( size, size ));
	else
		element = el;
	/// ��ʴ����
	cv::erode( src, dst, element);
}

DLL_API void binReverse(cv::Mat &src)
{
	src = 255 - src;
}

/** @function Dilation */
DLL_API void dilation(cv::Mat &src, cv::Mat &dst, int type, int size, cv::Mat el)
{
	int dilation_type;
	cv::Mat element;
	if( type == 0 ){ dilation_type = MORPH_RECT; }
	else if( type == 1 ){ dilation_type = MORPH_CROSS; }
	else if( type == 2) { dilation_type = MORPH_ELLIPSE; }
	else { dilation_type = MORPH_RECT; }

	if (type >= 0)
		element = cv::getStructuringElement( dilation_type,
		cv::Size( 2*size + 1, 2*size+1 ),
		cv::Point( size, size ) );
	else
		element = el;
	/// ���Ͳ���
	cv::dilate( src, dst, element );
}

DLL_API void openClose(cv::Mat &src, cv::Mat &dst, int type, int size, int op, cv::Mat el)
{  
	int oc_type;
	cv::Mat element;
	if( type == 0 ){ oc_type = MORPH_RECT; }
	else if( type == 1 ){ oc_type = MORPH_CROSS; }
	else if( type == 2) { oc_type = MORPH_ELLIPSE; }
	else { oc_type = MORPH_RECT; }

	if (type >= 0)
		element = cv::getStructuringElement( oc_type,
		Size( 2*size + 1, 2*size+1 ),
		Point( size, size ) ); 
	else
		element = el;
	cv::morphologyEx(src, dst, op, element);
}


DLL_API int writeDataBin(const char *file_name, const void *data, size_t data_len)
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

DLL_API int readDataBin(const char *file_name, void **data, size_t data_len)
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

DLL_API void lawsTexture(cv::Mat &img_src, cv::Mat &img_dst)
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
	cv::accumulate(*p_mat_dst[12], img_average);
	cv::accumulate(*p_mat_dst[13], img_average);
//	cv::accumulate(*p_mat_dst[14], img_average); //�ų�
//	cv::accumulate(*p_mat_dst[16], img_average);
//	cv::accumulate(*p_mat_dst[17], img_average);
	cv::accumulate(*p_mat_dst[18], img_average);
	img_average = img_average / 5;
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
DLL_API int getSizeContours(cv::Mat &img_src, cv::Mat &img_dst)
{  
	if(!img_src.data)  
	{  
		cout << "Fail to load image" << endl;  
		return 0;  
	}
	cv::Mat temp =  img_src.clone();
	cv::Mat result(img_src.size(), CV_8UC3, Scalar(255, 255,255));  

	//�����������飬ÿһ��������һ��point���͵�vector��ʾ
	cv::vector<cv::vector<cv::Point>> contours;
	//������������ͬ��ÿ������contours[ i ]��Ӧ4��hierarchyԪ��hierarchy[ i ][ 0 ] ~hierarchy[ i ][ 3 ]���ֱ��ʾ��һ��������ǰһ������������������Ƕ������������ţ����û�ж�Ӧ���ֵ����Ϊ������
	cv::vector<cv::Vec4i> hierarchy;
	//CV_CHAIN_APPROX_NONE  ��ȡÿ������ÿ�����ص� 
	//CV_CHAIN_APPROX_SIMPLEѹ��ˮƽ���򣬴�ֱ���򣬶Խ��߷����Ԫ�أ�ֻ�����÷�����յ����꣬����һ����������ֻ��4����������������Ϣ
 	cv::findContours(temp, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_NONE);
//	cv::findContours(img_src, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);
	cout << contours.size() << endl;
	//findContours������drawContours���ʹ�ã��������������Ƴ��������е�һ������image��ʾĿ��ͼ�񣬵ڶ�������contours��ʾ����������飬ÿһ�������ɵ�vector���ɣ�����������contourIdxָ�����ڼ�������������ò���Ϊ��ֵ����ȫ�����������ĸ�����colorΪ��������ɫ�����������thicknessΪ�������߿����Ϊ��ֵ��CV_FILLED��ʾ��������ڲ�������������lineTypeΪ���ͣ����߸�����Ϊ�����ṹ��Ϣ���ڰ˸�����ΪmaxLevel
	if( !contours.empty() && !hierarchy.empty() )
	{
		int idx = 0;
		for( ; idx >= 0; idx = hierarchy[idx][0] )
		{
			cout<<idx<<":"<<contours[idx].size()<<endl;
			if (contours[idx].size() > 20)
			{
				cv::Scalar color( (rand()&255), (rand()&255), (rand()&255) );
				cv::drawContours(result, contours, idx, color, 1, 8, hierarchy);
			}
		}
	}
	img_dst = result;

	return 0;  
} 


DLL_API int getSizeContoursGray(cv::Mat &img_src, cv::Mat &img_dst, int area, int y_min, int y_max)
{  
	if(!img_src.data)  
	{  
		cout << "Fail to load image" << endl;  
		return 0;  
	}
	cv::Mat temp =  img_src.clone();
	cv::Mat result(img_src.size(), CV_8U, cv::Scalar(0, 0, 0));
	cv::Moments mo;
	cv::Point2f point;

	//�����������飬ÿһ��������һ��point���͵�vector��ʾ
	cv::vector<cv::vector<cv::Point>> contours;
	//������������ͬ��ÿ������contours[ i ]��Ӧ4��hierarchyԪ��hierarchy[ i ][ 0 ] ~hierarchy[ i ][ 3 ]���ֱ��ʾ��һ��������ǰһ������������������Ƕ������������ţ����û�ж�Ӧ���ֵ����Ϊ������
	cv::vector<cv::Vec4i> hierarchy;
	//CV_CHAIN_APPROX_NONE  ��ȡÿ������ÿ�����ص� 
	//CV_CHAIN_APPROX_SIMPLEѹ��ˮƽ���򣬴�ֱ���򣬶Խ��߷����Ԫ�أ�ֻ�����÷�����յ����꣬����һ����������ֻ��4����������������Ϣ
	cv::findContours(temp, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
//	cv::findContours(temp, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_NONE);
//	cv::findContours(img_src, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);
	cout << contours.size() << endl;
	//findContours������drawContours���ʹ�ã��������������Ƴ��������е�һ������image��ʾĿ��ͼ�񣬵ڶ�������contours��ʾ����������飬ÿһ�������ɵ�vector���ɣ�����������contourIdxָ�����ڼ�������������ò���Ϊ��ֵ����ȫ�����������ĸ�����colorΪ��������ɫ�����������thicknessΪ�������߿����Ϊ��ֵ��CV_FILLED��ʾ��������ڲ�������������lineTypeΪ���ͣ����߸�����Ϊ�����ṹ��Ϣ���ڰ˸�����ΪmaxLevel
	if( !contours.empty() && !hierarchy.empty() )
	{
		int idx = 0;
		for( ; idx >= 0; idx = hierarchy[idx][0] )
		{
			if (contours[idx].size() >= 4)
			{
				mo = cv::moments(contours[idx]);
				point = cv::Point2f( mo.m10/mo.m00, mo.m01/mo.m00 );
			}
			else
				point = cv::Point2f(0, 0);
			cout<<idx<<" size:"<<contours[idx].size()<<" moments:"<<point.x<<", "<<point.y<<endl;
			if (contours[idx].size() >= area && point.y >= y_min && point.y <= y_max)
			{
				cv::Scalar color(255, 255, 255);
				cv::drawContours(result, contours, idx, color, CV_FILLED, 8, hierarchy);
// 				showImage(result, 0, 1);
			}
		}
	}
	img_dst = result;

	return 0;  
}

//http://blog.csdn.net/hust_bochu_xuchao/article/details/51967846
//http://blog.csdn.net/wangyaninglm/article/details/47701047
DLL_API void fillHole(const cv::Mat &src, cv::Mat &dst)  
{  
	cv::Size size = src.size();  
	cv::Mat Temp = cv::Mat::zeros(size.height+2, size.width+2, src.type());//��չͼ��  
	src.copyTo(Temp(cv::Range(1, size.height + 1), cv::Range(1, size.width + 1)) );  

	cv::floodFill(Temp, cv::Point(0, 0), cv::Scalar(255));  

	cv::Mat cutImg;//�ü���չ��ͼ��  
	Temp(cv::Range(1, size.height + 1), cv::Range(1, size.width + 1)).copyTo(cutImg);  

	dst = src | (~cutImg);  
} 

//http://blog.csdn.net/hujingshuang/article/details/47910949
//http://blog.csdn.net/chongshangyunxiao321/article/details/51104462
//����ͼ����Ϣ�ؼ���
DLL_API double entropy(const Mat &img_src)
{
    double temp[256] = { 0.0 };
    if (img_src.empty())
    {
        return -1.0;
    }
    int rows = img_src.rows;
    int cols = img_src.cols;
    if (img_src.isContinuous())
    {
        cols = rows * cols;
        rows = 1;
    }
    // ����ÿ�����ص��ۻ�ֵ
    for (int m = 0; m < rows; m++)
    {
        const uchar* data = img_src.ptr<uchar>(m);
        for (int n = 0; n < cols; n++)
        {
            int i = data[n];
            temp[i] = temp[i] + 1;
        }
    }

    // ����ÿ�����صĸ���
    int size = (cols * rows);
    for (int i = 0; i < 256; i++)
    {
        temp[i] /= size;
    }

    double result = 0;
    // ����ͼ����Ϣ��
    const double log_2 =  log(2.0);
    for (int i = 0; i < 256; i++)
    {
        if (temp[i] > 0.0)
            result = result - temp[i] * (log(temp[i]) / log_2);
    }
    return result;

}

// ����ͼ��������Ϣ�ؼ���
DLL_API double comEntropy(const Mat &img_src1, const Mat &img_src2)
{
    double temp[256][256] = { 0.0 };
    if (img_src1.empty() || img_src2.empty())
    {
        return -1.0;
    }
    if (img_src1.cols != img_src2.cols || 
        img_src1.rows != img_src2.rows || 
        img_src1.flags != img_src2.flags)
    {
        return -1.0;
    }

    int rows = img_src1.rows;
    int cols = img_src1.cols;
    if (img_src1.isContinuous())
    {
        cols = rows * cols;
        rows = 1;
    }
    // ��������ͼ�����ص��ۻ�ֵ
    for (int m = 0; m < rows; m++)
    {    // ��Ч�������еķ�ʽ
        const uchar* data1 = img_src1.ptr<uchar>(m);
        const uchar* data2 = img_src2.ptr<uchar>(m);
        for (int n = 0; n < cols; n++)
        {
            int i = data1[n];
            int j = data2[n];
            temp[i][j] = temp[i][j] + 1.0;
        }
    }

    // ����ÿ���������صĸ���
    int size = (cols * rows);
    for (int i = 0; i < 256; i++)
    {
        for (int j = 0; j < 256; j++)
        {
            temp[i][j] = temp[i][j] / size;
        }
    }

    double result = 0.0;
    const double log_2 =  log(2.0);
    //����ͼ��������Ϣ��
    for (int i = 0; i < 256; i++)
    {
        for (int j = 0; j < 256; j++)
        {
            if (temp[i][j] > 0.0)
                result = result - temp[i][j] * (log(temp[i][j]) / log_2);
        }
    }

    return result;

}

DLL_API double mutualInfo(const Mat &img_src1, const Mat &img_src2)
{
    double entropy_1 = entropy(img_src1);
    double entropy_2 = entropy(img_src2);
    double entropy_12 = comEntropy(img_src1, img_src2);
    if (entropy_1 > 0.0 &&
        entropy_2 > 0.0 &&
        entropy_12 > 0.0)
    {
        return (entropy_1 + entropy_2 - entropy_12);
    }
    else
        return -1.0;
}
