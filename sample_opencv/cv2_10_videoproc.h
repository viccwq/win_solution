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


#endif