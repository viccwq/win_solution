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

void project_v(const Mat &src, vector<int> &v_data)
{
    if (src.empty())
    {
        return;
    }
    for (int i = 0; i < src.cols; i++)
    {
        Mat data = src.col(i);
        Scalar s = sum(data);
        v_data.push_back(s[0]);
    }
}


static int find_min_box(const Mat &img_src, Rect &rect)
{

    if (img_src.empty())
    {
        return -1;
    }

    vector<int> h_data, v_data;
    project_h(img_src, h_data);
    project_v(img_src, v_data);

    int th;
    int temp_min, temp_max;

    th = img_src.cols * 255;
    temp_min = 0;
    temp_max = 0;
    for (int i = 0; i < h_data.size(); i++)
    {
        if (h_data[i] < th)
        {
            temp_min = i;
            break;
        }
    }
    for (int i = h_data.size() - 1; i > temp_min; i--)
    {
        if (h_data[i] < th)
        {
            temp_max = i;
            break;
        }
    }

    if ((temp_max - temp_min) < 2)
    {
        return -1;
    }
    else
    {
        rect.y = temp_min;
        rect.height = (temp_max - temp_min + 1);
    }


    th = img_src.rows * 255;
    temp_min = 0;
    temp_max = 0;
    for (int i = 0; i < v_data.size(); i++)
    {
        if (v_data[i] < th)
        {
            temp_min = i;
            break;
        }
    }
    for (int i = v_data.size() - 1; i > temp_min; i--)
    {
        if (v_data[i] < th)
        {
            temp_max = i;
            break;
        }
    }

    if ((temp_max - temp_min) < 2)
    {
        return -1;
    }
    else
    {
        rect.x = temp_min;
        rect.width = (temp_max - temp_min + 1);
    }
    return 0;
}

Mat pre_process(const Mat &img_src)
{
    Mat img_dst;
    if (img_src.empty())
    {
        return img_dst;
    }
    
    Mat img_gray;
    //splite the RGB image
    if (img_src.channels() > 1)
    {
        vector<Mat> mv;
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
    if (find_min_box(img_gray, rect))
        return img_dst;

    //convert the image to a square
    img_gray = img_gray(rect).clone();
    if ((img_gray.rows - img_gray.cols) > 0)
    {
        int m = (img_gray.rows - img_gray.cols) / 2;
        int k = (img_gray.rows - img_gray.cols) % 2;
        copyMakeBorder(img_gray, img_dst, 0, 0, m, m + k,\
            BORDER_CONSTANT, Scalar::all(255));
    }
    else if ((img_gray.cols - img_gray.rows) > 0)
    {
        int m = (img_gray.cols - img_gray.rows) / 2;
        int k = (img_gray.cols - img_gray.rows) % 2;
        copyMakeBorder(img_gray, img_dst, m, m + k, 0, 0,\
            BORDER_CONSTANT, Scalar::all(255));
    }
    else
        img_dst = img_gray.clone();

    GaussianBlur(img_dst, img_dst, Size(3, 3), 0, 0); 

    return img_dst;
}