#include "pre_proc.hpp"

void project_h(const Mat &src, vector<int> &h_data)
{
    if (src.empty())
    {
        return;
    }
    for (int i = 0; i < src.rows; i++)
    {
        Mat data = src.row(i);
        Scalar s = sum(data);
        h_data.push_back(s[0]);
    }
}


int find_min_box(const Mat &img_src, Rect &rect)
{
    vector<int> h_data, v_data;
    project_h(img_src, h_data);
    return 0;
}

Mat pre_process(const Mat &img_src)
{
    Mat img_dst;
    vector<Mat> mv;
    if (img_src.empty())
    {
        return img_dst;
    }
    
    Mat img_gray;
    //splite the RGB image
    if (img_src.channels() > 1)
    {
        split(img_src, mv);
        img_gray = mv[0].clone();
    }
    else
        img_gray = img_src.clone();

#if 0
    GaussianBlur(img_gray, img_gray, Size(3, 3), 0, 0);
    threshold(img_gray, img_gray, 128, 255);
#endif

    //find the minimum box of the binary image
    Rect rect;
    find_min_box(img_gray, rect);


    resize(img_gray(rect), img_dst, Size(IMG_WIDTH, IMG_HEIGHT));
    threshold(img_dst, img_dst, 128, 255, THRESH_BINARY);

    return img_dst;
}