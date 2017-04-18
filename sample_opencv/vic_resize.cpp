//http://blog.csdn.net/fengbingchun/article/details/17335477

//一维DFT源码
//http://blog.csdn.net/calcular/article/details/46804779
//图像傅里叶变换原理
//http://www.360doc.com/content/10/1128/20/2226925_73234298.shtml
#include "comm_def_col.h"
void vic_resize()
{
    cv::Mat matSrc, matDst1, matDst2;

    matSrc = cv::imread("cv3.jpg", 2 | 4);
    matDst1 = cv::Mat(cv::Size(800, 1000), matSrc.type(), cv::Scalar::all(0));
    matDst2 = cv::Mat(matDst1.size(), matSrc.type(), cv::Scalar::all(0));

    double scale_x = (double)matSrc.cols / matDst1.cols;
    double scale_y = (double)matSrc.rows / matDst1.rows;
    uchar* dataDst = matDst1.data;
    int stepDst = matDst1.step;
    uchar* dataSrc = matSrc.data;
    int stepSrc = matSrc.step;
    int iWidthSrc = matSrc.cols;
    int iHiehgtSrc = matSrc.rows;

    for (int j = 0; j < matDst1.rows; ++j)
    {
        float fy = (float)((j + 0.5) * scale_y - 0.5);
        int sy = cvFloor(fy);
        fy -= sy;
        sy = std::min(sy, iHiehgtSrc - 2);
        sy = std::max(0, sy);

        short cbufy[2];
        cbufy[0] = cv::saturate_cast<short>((1.f - fy) * 2048);
        cbufy[1] = 2048 - cbufy[0];

        for (int i = 0; i < matDst1.cols; ++i)
        {
            float fx = (float)((i + 0.5) * scale_x - 0.5);
            int sx = cvFloor(fx);
            fx -= sx;

            if (sx < 0) {
                fx = 0, sx = 0;
            }
            if (sx >= iWidthSrc - 1) {
                fx = 0, sx = iWidthSrc - 2;
            }

            short cbufx[2];
            cbufx[0] = cv::saturate_cast<short>((1.f - fx) * 2048);
            cbufx[1] = 2048 - cbufx[0];

            for (int k = 0; k < matSrc.channels(); ++k)
            {
                *(dataDst+ j*stepDst + 3*i + k) = (*(dataSrc + sy*stepSrc + 3*sx + k) * cbufx[0] * cbufy[0] + 
                    *(dataSrc + (sy+1)*stepSrc + 3*sx + k) * cbufx[0] * cbufy[1] + 
                    *(dataSrc + sy*stepSrc + 3*(sx+1) + k) * cbufx[1] * cbufy[0] + 
                    *(dataSrc + (sy+1)*stepSrc + 3*(sx+1) + k) * cbufx[1] * cbufy[1]) >> 22;
            }
        }
    }
    cv::imwrite("./Debug/linear_1.jpg", matDst1);

    cv::resize(matSrc, matDst2, matDst1.size(), 0, 0, 1);
    cv::imwrite("./Debug/linear_2.jpg", matDst2);
}