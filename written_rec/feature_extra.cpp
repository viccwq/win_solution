#include "feature_extra.h"

Mat feature_dft( const Mat &img_src )
{
    if (img_src.empty())
    {
        return Mat::Mat();
    }

    Mat padded;                            //expand input image to optimal size
    int m = getOptimalDFTSize( img_src.rows );
    int n = getOptimalDFTSize( img_src.cols ); // on the border add zero values
    copyMakeBorder(img_src, padded, 0, m - img_src.rows, 0, n - img_src.cols, \
        BORDER_CONSTANT, Scalar::all(0));

    Mat planes[] = {Mat_<float>(padded), Mat::zeros(padded.size(), CV_32F)};
    Mat complexI;
    merge(planes, 2, complexI);         // Add to the expanded another plane with zeros

    dft(complexI, complexI);            // this way the result may fit in the source matrix

    // compute the magnitude and switch to logarithmic scale
    // => log(1 + sqrt(Re(DFT(I))^2 + Im(DFT(I))^2))
    split(complexI, planes);                   // planes[0] = Re(DFT(I), planes[1] = Im(DFT(I))
    magnitude(planes[0], planes[1], planes[0]);// planes[0] = magnitude
    Mat magI = planes[0];

    magI += Scalar::all(1);                    // switch to logarithmic scale
    log(magI, magI);

    // crop the spectrum, if it has an odd number of rows or columns
    magI = magI(Rect(0, 0, magI.cols & -2, magI.rows & -2));
/*
    // rearrange the quadrants of Fourier image  so that the origin is at the image center
    int cx = magI.cols/2;
    int cy = magI.rows/2;

    Mat q0(magI, Rect(0, 0, cx, cy));   // Top-Left - Create a ROI per quadrant
    Mat q1(magI, Rect(cx, 0, cx, cy));  // Top-Right
    Mat q2(magI, Rect(0, cy, cx, cy));  // Bottom-Left
    Mat q3(magI, Rect(cx, cy, cx, cy)); // Bottom-Right

    Mat tmp;                           // swap quadrants (Top-Left with Bottom-Right)
    q0.copyTo(tmp);
    q3.copyTo(q0);
    tmp.copyTo(q3);

    q1.copyTo(tmp);                    // swap quadrant (Top-Right with Bottom-Left)
    q2.copyTo(q1);
    tmp.copyTo(q2);
*/
    normalize(magI, magI, 0, 1, CV_MINMAX); // Transform the matrix with float values into a
    // viewable image form (float between values 0 and 1).


    imshow("Input Image"       , img_src   );    // Show the result
    imshow("spectrum magnitude", magI);
    waitKey(1);
    return magI;
}


vector<Point> feature_edge(const Mat &img_src)
{
    if (img_src.empty())
    {
        return vector<Point>::vector(0);
    }

    Mat temp =  img_src.clone();
    Mat result(img_src.size(), CV_8U, Scalar(255, 255,255));  

    //检测的轮廓数组，每一个轮廓用一个point类型的vector表示
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;
    findContours(temp, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_NONE);
    cout << contours.size() << endl;

    //找出最大的轮廓
    if( !contours.empty() && !hierarchy.empty() )
    {
        int idx = 0, idx_max = 0;
        int size_max = 20;
        for( ; idx >= 0; idx = hierarchy[idx][0] )
        {
            if (contours[idx].size() > size_max)
            {
                size_max = contours[idx].size();
                idx_max = idx;
            }
        }
//         drawContours(result, contours, idx_max, Scalar(0, 0, 0), 1, 8, hierarchy);
//         imwrite("./Debug/contours.bmp", result);
        return contours[idx_max];
    }
    else
        return vector<Point>::vector(0);
}

int draw_points_8U(const vector<Point> &points, unsigned char val, Mat &img_dst)
{
    if (points.size() < 1 || img_dst.empty())
    {
        return -1;
    }

    for (int i = 0; i < points.size(); i++)
    {
        img_dst.at<uchar>(points[i]) = val;
    }
    return 0;
}
