#include "cv2_10_videoproc.h"

//设置回调函数
void VideoProc::setFrameProcessor(void(*frameProcessCallBack)(Mat &, Mat &))
{
    process = frameProcessCallBack;
}
//设置视频的名称
bool VideoProc::setInput(string filename)
{
    m_fnumber = 0;
    m_capture.release();

    return m_capture.open(filename);
}

//设置延时
void VideoProc::setFrameRate( int ms )
{
    if (!m_capture.isOpened())
    {
        return;
    }
    m_delay = static_cast<int>(1000 / m_capture.get(CV_CAP_PROP_FPS));
}

void VideoProc::displayInput(string wn)
{
    m_winNameInput = wn;
    namedWindow(m_winNameInput);
}

void VideoProc::displayOutput(string wn)
{
    m_winNameOutput = wn;
    namedWindow(m_winNameOutput);
}

void VideoProc::displayClear()
{
    destroyWindow(m_winNameInput);
    destroyWindow(m_winNameOutput);
    m_winNameInput.clear();
    m_winNameOutput.clear();
}

void VideoProc::run()
{
    Mat frame;
    Mat frame_out;
    
    if (!m_capture.isOpened())
    {
        return;
    }

    while (1)
    {
        //读取下一帧并显示
        if (!m_capture.read(frame))
        {
            break;
        }
        imshow(m_winNameInput, frame);

        //处理当前帧并显示
        if (m_callIt)
        {
            process(frame, frame_out);
            m_fnumber++;
        }
        else
        {
            frame_out = frame;
        }
        imshow(m_winNameOutput, frame_out);

        if (m_delay >=0 && waitKey(m_delay) >= 0)
        {
            break;
        }
    }
    m_capture.release();
}
//是否调用回调函数
void VideoProc::setFrameProcessorEnable( bool en )
{
    m_callIt = en;
}

int open_video_file(const string file)
{
    VideoCapture capture(file);

    //检测打开
    if (!capture.isOpened())
    {
        return -1;
    }
    //获取帧率,计算延时
    double rate = capture.get(CV_CAP_PROP_FPS);
    int delay = static_cast<int>(1000 / rate);

    Mat frame;
    namedWindow("Video");

    //显示帧
    while (1)
    {
        if (!capture.read(frame))
        {
            break;
        }
        imshow("Video", frame);

        if (waitKey(delay * 2) >= 0)
        {
            break;
        }
    }
    capture.release();

    return 0;
}

void canny(Mat &src, Mat &dst)
{
    if (src.channels() == 3)
    {
        cvtColor(src, dst, CV_BGR2BGRA);
    }
    Canny(dst, dst, 100, 200);
    threshold(dst, dst, 128, 255, CV_THRESH_BINARY_INV);
}

void cv2_10_videoprocess(void)
{
    VideoProc videoProc;
    videoProc.setInput("./Debug/bike.avi");
    videoProc.displayInput("current frame");
    videoProc.displayOutput("output frame");
    videoProc.setFrameRate(1000);

    videoProc.setFrameProcessor(canny);
    videoProc.setFrameProcessorEnable(true);

    videoProc.run();
    videoProc.displayClear();
}