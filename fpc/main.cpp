#include "comm_def_col.h"


int fpc_circle(Mat &img)
{
    vector<Mat> channels;
    split(img, channels);
    Mat img_r = channels[2];
    GaussianBlur(img_r, img_r, Size(7,7), 2);
    threshold(img_r, img_r, 40, 255, CV_THRESH_BINARY);

    vector<Vec3f> circles;
    HoughCircles(img_r, circles, CV_HOUGH_GRADIENT, 1, 30, 200, 50);

    for (int i = 0; i < circles.size(); i++)
    {
        Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
        int radius = cvRound(circles[i][2]);
        circle(img, center, 3, Scalar(0,255,0),-1);
        circle(img, center,radius, Scalar(0,0,255), 3);
    }

    return 0;
}

int main()
{
    Mat img = imread("./fpc2.jpg");
    fpc_circle(img);
    return 0;
}