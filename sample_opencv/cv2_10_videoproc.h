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
    //�ص�����
    void(*process)(Mat &, Mat &);
    //�ص���־
    bool m_callIt;
    //����
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
    //���ٵ�������
    vector<Point2f> m_point;
    vector<Point2f> m_point_prev;
    //���ٵĵ��ʼλ��
    vector<Point2f> m_initial;
    vector<Point2f> m_features;

    //���������Ŀ
    int m_max_count;
    //��������е������ȼ�
    double m_qlevel;
    //����֮�����С����
    double m_min_dist;
    //��⵽������״̬
    vector<uchar> m_status;
    //���ٹ����еĴ���
    vector<float> m_error;
};


#endif