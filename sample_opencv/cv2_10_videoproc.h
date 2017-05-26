#ifndef _CV2_10_VIDEOPROC_H
#define _CV2_10_VIDEOPROC_H
#include "comm_def_col.h"


class VideoProc
{
public:
    VideoProc():m_callIt(true),m_delay(0),m_fnumber(0)
    {}
    void setFrameProcessor(void(*frameProcessCallBack)(Mat &, Mat &));
    void setFrameProcessorEnable(bool en);
    bool setInput(string filename);
    bool setOutput(const string &filename, int codec = 0);
    void setFrameRate(int ms);
    void displayInput(string wn);
    void displayOutput(string wn);
    void displayClear();
    void run();

protected:
private:
    VideoCapture m_capture;
    VideoWriter m_witer;
    //回调函数
    void(*process)(Mat &, Mat &);
    //回调标志
    bool m_callIt;
    //窗口
    string m_winNameInput;
    string m_winNameOutput;

    int m_delay;
    long m_fnumber;
  
    
};


class FeatureTraker
{
public:
    FeatureTraker():m_max_count(500),m_qlevel(0.01),m_min_dist(10.){}
    void porcess(Mat &src, Mat &dst);
private:
    Mat m_gray;
    Mat m_gray_prev;
    //跟踪的特征点
    vector<Point2f> m_point;
    vector<Point2f> m_point_prev;
    //跟踪的点初始位置
    vector<Point2f> m_initial;
    vector<Point2f> m_features;

    //最大特征数目
    int m_max_count;
    //特征检测中的质量等级
    double m_qlevel;
    //两点之间的最小距离
    double m_min_dist;
    //检测到的特征状态
    vector<uchar> m_status;
    //跟踪过程中的错误
    vector<float> m_error;
};


#endif