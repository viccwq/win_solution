#include "comm_def_col.h"


int fpc_circle(Mat &img)
{
    vector<Mat> channels;
    split(img, channels);
    Mat img_r = channels[2];
    //平滑
    GaussianBlur(img_r, img_r, Size(7,7), 2);
	img_r = img_r * 1.5;
//    threshold(img_r, img_r, 40, 255, CV_THRESH_BINARY);


    vector<Vec3f> circles;
    //霍夫变换
    HoughCircles(img_r, circles, CV_HOUGH_GRADIENT, 1, 50, 150, 60);

    for (int i = 0; i < circles.size(); i++)
    {
        Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
        int radius = cvRound(circles[i][2]);
        circle(img, center, 3, Scalar(0,255,0), 2);
        circle(img, center, radius, Scalar(0,0,255), 3);
        cout<<"x:"<<center.x<<", y:"<<center.y<<", radius:"<<radius<<endl;
    }
	imwrite("./fpc2_ret.jpg", img);
    return 0;
}

static unsigned char max_pix(const Mat &img)
{
    unsigned char max = 0;
    //计算图像局部最大值
    for (int i = 0; i < img.rows; i++)
    {
        const unsigned char *data = img.ptr<unsigned char>(i);
        for (int j = 0; j < img.cols; j++)
        {
            if (data[j] > max)
                max = data[j];
        }
    }
    return max;
}

int fpc_contours(const Mat &img)
{
    Mat img_blur;
    Mat img_bin;
    //BGR 2 gray
    cvtColor(img, img_blur, CV_RGB2GRAY);
    //去椒盐
    medianBlur(img_blur, img_blur, 9);
    //平滑
    GaussianBlur(img_blur, img_blur, Size(9,9), 2);
    threshold(img_blur, img_bin, 60, 255, CV_THRESH_BINARY);

    Mat temp = img_bin.clone();
    cv::vector<cv::vector<cv::Point>> contours;
    cv::vector<cv::Vec4i> hierarchy;
    //寻找轮廓（周长>1000）
    cv::findContours(temp, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
    cout << contours.size() << endl;

    unsigned char max = 0;
    int idx = -1;
    if( !contours.empty() && !hierarchy.empty() )
    {
        
        for( size_t i = 0; i< contours.size(); i++ )
        {
            if (contours[i].size() < 1000)
                continue;
            Scalar color = Scalar(255, 255, 255);
            //创建连通域的蒙板
            Mat mask = Mat::zeros(img_bin.size(), CV_8UC1);
            drawContours( mask, contours, (int)i, color, -1, 8, hierarchy);
            Mat ret;
            //通过蒙板将原图分割
            img_blur.copyTo(ret, mask);
            //保存局部最大值最高的区域index
            unsigned char temp_max = max_pix(ret);
            if (temp_max > max)
            {
                max = temp_max;
                idx = i;
            }            
        }
    }

    if (idx > -1)
    {
        Scalar color = Scalar(255, 255, 255);
        //创建连通域的蒙板
        Mat mask = Mat::zeros(img_bin.size(), CV_8UC1);
        drawContours( mask, contours, idx, color, -1, 8, hierarchy);
        Mat ret;
        //通过蒙板将原图分割
        img.copyTo(ret, mask);
        //保存图像
        imwrite("./fpc1_ret.jpg", ret);
    }
    return 0;
}

int main()
{
    Mat img = imread("./fpc2.jpg");
    fpc_circle(img);
    img = imread("./fpc1.jpg");
    fpc_contours(img);
    system("pause");
    return 0;
}