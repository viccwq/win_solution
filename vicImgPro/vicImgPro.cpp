//------------------ vicImgPro.cpp ----------------  
//http://blog.csdn.net/testcs_dn/article/details/27237509
//注意此处的宏定义需要写在#include "SimpleDLL.h"之前  
//以完成在dll项目内部使用__declspec(dllexport)导出  
//在dll项目外部使用时，用__declspec(dllimport)导入  
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
	/// 腐蚀操作
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
	/// 膨胀操作
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


//《laws纹理模板在立体匹配中的应用》
//《低对比度医学图像全自动分割算法》
//《textured image segment》
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
	cv::accumulate(*p_mat_dst[12], img_average);
	cv::accumulate(*p_mat_dst[13], img_average);
//	cv::accumulate(*p_mat_dst[14], img_average); //排除
//	cv::accumulate(*p_mat_dst[16], img_average);
//	cv::accumulate(*p_mat_dst[17], img_average);
	cv::accumulate(*p_mat_dst[18], img_average);
	img_average = img_average / 5;
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

//http://blog.sina.com.cn/s/blog_662c78590100z0rg.html
//http://blog.csdn.net/lu597203933/article/details/17362457
//http://www.opencv.org.cn/opencvdoc/2.3.2/html/doc/tutorials/imgproc/shapedescriptors/moments/moments.html#moments
// 计算连通区域的轮廓，即二值图像中相连像素的形状  
DLL_API int getSizeContours(cv::Mat &img_src, cv::Mat &img_dst)
{  
	if(!img_src.data)  
	{  
		cout << "Fail to load image" << endl;  
		return 0;  
	}
	cv::Mat temp =  img_src.clone();
	cv::Mat result(img_src.size(), CV_8UC3, Scalar(255, 255,255));  

	//检测的轮廓数组，每一个轮廓用一个point类型的vector表示
	cv::vector<cv::vector<cv::Point>> contours;
	//和轮廓个数相同，每个轮廓contours[ i ]对应4个hierarchy元素hierarchy[ i ][ 0 ] ~hierarchy[ i ][ 3 ]，分别表示后一个轮廓、前一个轮廓、父轮廓、内嵌轮廓的索引编号，如果没有对应项，该值设置为负数。
	cv::vector<cv::Vec4i> hierarchy;
	//CV_CHAIN_APPROX_NONE  获取每个轮廓每个像素点 
	//CV_CHAIN_APPROX_SIMPLE压缩水平方向，垂直方向，对角线方向的元素，只保留该方向的终点坐标，例如一个矩形轮廓只需4个点来保存轮廓信息
 	cv::findContours(temp, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_NONE);
//	cv::findContours(img_src, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);
	cout << contours.size() << endl;
	//findContours经常与drawContours配合使用，用来将轮廓绘制出来。其中第一个参数image表示目标图像，第二个参数contours表示输入的轮廓组，每一组轮廓由点vector构成，第三个参数contourIdx指明画第几个轮廓，如果该参数为负值，则画全部轮廓，第四个参数color为轮廓的颜色，第五个参数thickness为轮廓的线宽，如果为负值或CV_FILLED表示填充轮廓内部，第六个参数lineType为线型，第七个参数为轮廓结构信息，第八个参数为maxLevel
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

	//检测的轮廓数组，每一个轮廓用一个point类型的vector表示
	cv::vector<cv::vector<cv::Point>> contours;
	//和轮廓个数相同，每个轮廓contours[ i ]对应4个hierarchy元素hierarchy[ i ][ 0 ] ~hierarchy[ i ][ 3 ]，分别表示后一个轮廓、前一个轮廓、父轮廓、内嵌轮廓的索引编号，如果没有对应项，该值设置为负数。
	cv::vector<cv::Vec4i> hierarchy;
	//CV_CHAIN_APPROX_NONE  获取每个轮廓每个像素点 
	//CV_CHAIN_APPROX_SIMPLE压缩水平方向，垂直方向，对角线方向的元素，只保留该方向的终点坐标，例如一个矩形轮廓只需4个点来保存轮廓信息
	cv::findContours(temp, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
//	cv::findContours(temp, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_NONE);
//	cv::findContours(img_src, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);
	cout << contours.size() << endl;
	//findContours经常与drawContours配合使用，用来将轮廓绘制出来。其中第一个参数image表示目标图像，第二个参数contours表示输入的轮廓组，每一组轮廓由点vector构成，第三个参数contourIdx指明画第几个轮廓，如果该参数为负值，则画全部轮廓，第四个参数color为轮廓的颜色，第五个参数thickness为轮廓的线宽，如果为负值或CV_FILLED表示填充轮廓内部，第六个参数lineType为线型，第七个参数为轮廓结构信息，第八个参数为maxLevel
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
	cv::Mat Temp = cv::Mat::zeros(size.height+2, size.width+2, src.type());//延展图像  
	src.copyTo(Temp(cv::Range(1, size.height + 1), cv::Range(1, size.width + 1)) );  

	cv::floodFill(Temp, cv::Point(0, 0), cv::Scalar(255));  

	cv::Mat cutImg;//裁剪延展的图像  
	Temp(cv::Range(1, size.height + 1), cv::Range(1, size.width + 1)).copyTo(cutImg);  

	dst = src | (~cutImg);  
} 

//http://blog.csdn.net/hujingshuang/article/details/47910949
//http://blog.csdn.net/chongshangyunxiao321/article/details/51104462
//单幅图像信息熵计算
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
    // 计算每个像素的累积值
    for (int m = 0; m < rows; m++)
    {
        const uchar* data = img_src.ptr<uchar>(m);
        for (int n = 0; n < cols; n++)
        {
            int i = data[n];
            temp[i] = temp[i] + 1;
        }
    }

    // 计算每个像素的概率
    int size = (cols * rows);
    for (int i = 0; i < 256; i++)
    {
        temp[i] /= size;
    }

    double result = 0;
    // 计算图像信息熵
    const double log_2 =  log(2.0);
    for (int i = 0; i < 256; i++)
    {
        if (temp[i] > 0.0)
            result = result - temp[i] * (log(temp[i]) / log_2);
    }
    return result;

}

// 两幅图像联合信息熵计算
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
    // 计算联合图像像素的累积值
    for (int m = 0; m < rows; m++)
    {    // 有效访问行列的方式
        const uchar* data1 = img_src1.ptr<uchar>(m);
        const uchar* data2 = img_src2.ptr<uchar>(m);
        for (int n = 0; n < cols; n++)
        {
            int i = data1[n];
            int j = data2[n];
            temp[i][j] = temp[i][j] + 1.0;
        }
    }

    // 计算每个联合像素的概率
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
    //计算图像联合信息熵
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

static complex comp_multi(const complex &a, const complex &b ) 
{
    complex tmp = {0.0, 0.0};
    tmp.x = a.x*b.x - a.y*b.y;  
    tmp.y = a.x*b.y + a.y*b.x;  
    return tmp;  
}

static complex comp_add(const complex &a, const complex &b ) 
{
    complex tmp = {0.0, 0.0};
    tmp.x = a.x + b.x;  
    tmp.y = a.y + b.y;
    return tmp;  
}

//reference
//http://blog.csdn.net/calcular/article/details/46804779
//http://blog.csdn.net/TonyShengTan/article/details/41178255
//principle
//http://www.cnblogs.com/amanlikethis/archive/2014/01/04/3505248.html
//动态链接库中导出模板函数
//http://blog.csdn.net/liyuanbhu/article/details/50363670

template<typename T> 
int DFT(const T *r_in, const T *i_in, double *r_out, double *i_out, const int N)
{
    if (NULL == r_in || NULL == r_out || NULL == i_out)
    {
        return -1;
    }
    //calculate WN
    //WN = exp((-j * 2 * Pi / N) * k * n)
    //k = 0, 1, ..., N-1
    //n = 0, 1, ..., N-1
    int size = (N - 1) * (N - 1) + 1;
    complex *WN = new complex[size];
    WN[0].x = 1;    //cos(0)
    WN[0].y = 0;    //sin(0)
    WN[1].x = cos(2.0 * PI / N);
    WN[1].y = -1.0 * sin(2.0 * PI / N);
    for(int i = 2; i < size; i++)
    {  
        WN[i] = comp_multi(WN[1], WN[i - 1]);
    }

    //fill W with WN
    vector<vector<complex> > W(N, vector<complex>(N)); 
    for(int i = 0; i < N; i++)
    {  
        for(int j = 0; j < N; j++)
        {  
            W[i][j] = WN[i*j];  
        }  
    }  
    
    //calculate
    complex sum;

    if (NULL != i_in)
    {
        for(int i = 0; i < N; i++)
        {  
            sum.x = 0;
            sum.y = 0;  
            for(int j = 0; j < N; j++)
            {  
                sum.x += (((T *)r_in)[j] * W[i][j].x -\
                        ((T *)i_in)[j] * W[i][j].y);
                sum.y += (((T *)r_in)[j] * W[i][j].y +\
                        ((T *)i_in)[j] * W[i][j].x);
            }
            r_out[i] = sum.x;
            i_out[i] = sum.y;
        }
    }
    else
    {
        for(int i = 0; i < N; i++)
        {  
            sum.x = 0;
            sum.y = 0;  
            for(int j = 0; j < N; j++)
            {  
                sum.x += (((T *)r_in)[j] * W[i][j].x);
                sum.y += (((T *)r_in)[j] * W[i][j].y);
            }
            r_out[i] = sum.x;
            i_out[i] = sum.y;
        }
    }
    delete []WN;
    return 0;
}


template DLL_API int DFT(const int *r_in, const int *i_in, double *r_out, double *i_out, const int N);
template DLL_API int DFT(const char *r_in, const char *i_in, double *r_out, double *i_out, const int N);
template DLL_API int DFT(const short *r_in, const short *i_in, double *r_out, double *i_out, const int N);
template DLL_API int DFT(const float *r_in, const float *i_in, double *r_out, double *i_out, const int N);
template DLL_API int DFT(const double *r_in, const double *i_in, double *r_out, double *i_out, const int N);

template DLL_API int DFT(const unsigned int *r_in, const unsigned int *i_in, double *r_out, double *i_out, const int N);
template DLL_API int DFT(const unsigned char *r_in, const unsigned char *i_in, double *r_out, double *i_out, const int N);
template DLL_API int DFT(const unsigned short *r_in, const unsigned short *i_in, double *r_out, double *i_out, const int N);

template<typename T> 
int IDFT(const T *r_in, const T *i_in, double *r_out, double *i_out, const int N)
{
    if (NULL == r_in || NULL == r_out || NULL == i_out)
    {
        return -1;
    }
    //calculate WN
    //WN = exp((-j * 2 * Pi / N) * k * n)
    //k = 0, 1, ..., N-1
    //n = 0, 1, ..., N-1
    int size = (N - 1) * (N - 1) + 1;
    complex *WN = new complex[size];
    WN[0].x = 1;    //cos(0)
    WN[0].y = 0;    //sin(0)
    WN[1].x = cos(2.0 * PI / N);
    WN[1].y = sin(2.0 * PI / N);
    for(int i = 2; i < size; i++)
    {  
        WN[i] = comp_multi(WN[1], WN[i - 1]);
    }

    //fill W with WN
    vector<vector<complex> > W(N, vector<complex>(N)); 
    for(int i = 0; i < N; i++)
    {  
        for(int j = 0; j < N; j++)
        {  
            W[i][j] = WN[i*j];  
        }  
    }  

    //calculate
    complex sum;

    if (NULL != i_in)
    {
        for(int i = 0; i < N; i++)
        {  
            sum.x = 0;
            sum.y = 0;  
            for(int j = 0; j < N; j++)
            {  
                sum.x += (((T *)r_in)[j] * W[i][j].x -\
                    ((T *)i_in)[j] * W[i][j].y);
                sum.y += (((T *)r_in)[j] * W[i][j].y +\
                    ((T *)i_in)[j] * W[i][j].x);
            }
            r_out[i] = sum.x / N;
            i_out[i] = sum.y / N;
        }
    }
    else
    {
        for(int i = 0; i < N; i++)
        {  
            sum.x = 0;
            sum.y = 0;  
            for(int j = 0; j < N; j++)
            {  
                sum.x += (((T *)r_in)[j] * W[i][j].x);
                sum.y += (((T *)r_in)[j] * W[i][j].y);
            }
            r_out[i] = sum.x / N;
            i_out[i] = sum.y / N;
        }
    }
    delete []WN;
    return 0;
}

template DLL_API int IDFT(const int *r_in, const int *i_in, double *r_out, double *i_out, const int N);
template DLL_API int IDFT(const char *r_in, const char *i_in, double *r_out, double *i_out, const int N);
template DLL_API int IDFT(const short *r_in, const short *i_in, double *r_out, double *i_out, const int N);
template DLL_API int IDFT(const float *r_in, const float *i_in, double *r_out, double *i_out, const int N);
template DLL_API int IDFT(const double *r_in, const double *i_in, double *r_out, double *i_out, const int N);

template DLL_API int IDFT(const unsigned int *r_in, const unsigned int *i_in, double *r_out, double *i_out, const int N);
template DLL_API int IDFT(const unsigned char *r_in, const unsigned char *i_in, double *r_out, double *i_out, const int N);
template DLL_API int IDFT(const unsigned short *r_in, const unsigned short *i_in, double *r_out, double *i_out, const int N);

//傅里叶描述子
//http://www.cnblogs.com/edie0902/p/3658174.html
DLL_API int dftLowPass(const vector<Point> &point_in, const int N, const int n, vector<Point> &point_out)
{
    int ret = 0;
    if (point_in.size() < 1 || N < 1)
    {
        return -1;
    }

    if (N < 2 * n)
    {
        return -1;
    }

    double *x_in = new double[N]();
    double *y_in = new double[N]();
    double *x_out = new double[N]();
    double *y_out = new double[N]();

    //import data
    for (int i = 0; i < N; i++)
    {
        x_in[i] = static_cast<double>(point_in[i].x);
        y_in[i] = static_cast<double>(point_in[i].y);
    }

    ret = DFT<double>(x_in, y_in, x_out, y_out, N);
    //clear the height frequency
    for (int i = (1 + n); i < (N - n); i++)
    {
        x_out[i] = 0.0;
        y_out[i] = 0.0;
    }

    ret = IDFT<double>(x_out, y_out, x_in, y_in, N);
    //export data
    for (int i = 0; i < N; i++)
    {
        Point point;
        point.x = static_cast<int>(x_in[i]);
        point.y = static_cast<int>(y_in[i]);
        point_out.push_back(point);
    }

    delete []x_in;
    delete []y_in;
    delete []x_out;
    delete []y_out;
    return 0;
}

DLL_API int dftFreqMagnitude(const vector<Point> &point_in, const int N, const int n, vector<double> &Mag)
{
    int ret = 0;
    if (point_in.size() < 1 || N < 1)
    {
        return -1;
    }

    if (N < 2 * n)
    {
        return -1;
    }

    double *x_in = new double[N]();
    double *y_in = new double[N]();
    double *x_out = new double[N]();
    double *y_out = new double[N]();

    //import data
    for (int i = 0; i < N; i++)
    {
        x_in[i] = static_cast<double>(point_in[i].x);
        y_in[i] = static_cast<double>(point_in[i].y);
    }

    ret = DFT<double>(x_in, y_in, x_out, y_out, N);

    Mat mat_r(Size(N, 1), CV_64F, x_out);
    Mat mat_i(Size(N, 1), CV_64F, y_out);
    multiply(mat_r, mat_r, mat_r);
    multiply(mat_i, mat_i, mat_i);

    mat_r = mat_r + mat_i;

    for (int i = (N - n); i < N; i++)
    {
        Mag.push_back(sqrt(x_out[i]));
    }
    for (int i = 1; i < (n + 1); i++)
    {
        Mag.push_back(sqrt(x_out[i]));
    }

    delete []x_in;
    delete []y_in;
    delete []x_out;
    delete []y_out;
    return 0;
}