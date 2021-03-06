#include "cv2_10_videoproc.h"

//设置回调函数
void VideoProc::setFrameProcessor(void(*frameProcessCallBack)(Mat &, Mat &))
{
    frameProcessor = NULL;
    process = frameProcessCallBack;
}

void VideoProc::setFrameProcessor( FrameProcessor *frameProcessorPtr)
{
    process = NULL;
    frameProcessor = frameProcessorPtr;
}

//设置视频的名称
bool VideoProc::setInput(string filename)
{
    m_fnumber = 0;
    m_capture.release();

    return m_capture.open(filename);
}
//设置输出视频
bool VideoProc::setOutput( const string &filename, int codec)
{
    union code
    {
        int val;
        char ch[4];
    } ret_codec;

    if (codec == 0)
    {
        ret_codec.val = static_cast<int>(m_capture.get(CV_CAP_PROP_FOURCC));
    }
    else
    {
        ret_codec.val = codec;
    }
    cout<<"Codec:"\
        <<ret_codec.ch[0]\
        <<ret_codec.ch[1]\
        <<ret_codec.ch[2]\
        <<ret_codec.ch[3]<<endl;

    double fps = static_cast<double>(m_capture.get(CV_CAP_PROP_FPS));
    Size size;
    size.height = static_cast<int>(m_capture.get(CV_CAP_PROP_FRAME_HEIGHT));
    size.width = static_cast<int>(m_capture.get(CV_CAP_PROP_FRAME_WIDTH));

    return m_witer.open(filename, codec, fps, size);
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
            if (process)
                process(frame, frame_out);
            else if (frameProcessor)
                frameProcessor->process(frame, frame_out);
            m_fnumber++;
        }
        else
        {
            frame_out = frame;
        }
        //输出图像序列
        if (frame_out.channels() == 1)
        {
 //           cvtColor(img, cimg, CV_GRAY2RGB);
            cvtColor(frame_out, frame_out, CV_GRAY2RGB);
        }
        m_witer.write(frame_out);
        imshow(m_winNameOutput, frame_out);

        if (m_delay >=0 && waitKey(m_delay) >= 0)
        {
            break;
        }
    }
    m_capture.release();
    m_witer.release();
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

    videoProc.setOutput("./Debug/bike2.avi", CV_FOURCC('M', 'J', 'P', 'G'));

    videoProc.run();
    videoProc.displayClear();
}

void FeatureTraker::process( Mat &src, Mat &dst )
{
    src.copyTo(dst);
    cvtColor(src, m_gray, CV_BGR2GRAY);

    //m_point_prev添加特征点
    if (m_point_prev.size() <= 10)
    {
        m_features.clear();
        goodFeaturesToTrack(m_gray, m_features, m_max_count, m_qlevel, m_min_dist);
        m_point_prev.insert(m_point_prev.end(), m_features.begin(), m_features.end());
        m_initial.insert(m_initial.end(), m_features.begin(), m_features.end());
    }

    if (m_gray_prev.empty())
    {
        m_gray.copyTo(m_gray_prev);
    }
    //跟踪特征点
    calcOpticalFlowPyrLK(
        m_gray_prev, m_gray,
        m_point_prev,
        m_point,
        m_status,
        m_error);

    //筛选所有跟踪点
    int k = 0;
    for (int i = 0; i < m_point.size(); i++)
    {
        //保留相关特征点
        bool ret = m_status[i] && (abs(m_point_prev[i].x - m_point[i].x) + abs(m_point_prev[i].y - m_point[i].y)) > 2;
        if (ret)
        {
            m_initial[k] = m_initial[i];
            m_point[k] = m_point[i];
            k++;
        }
    }
    //去除不成功的点
    m_initial.resize(k);
    m_point.resize(k);

    //处理接受的跟踪点
    for (int i = 0, k = 0; i < m_point.size(); i++)
    {
        line(dst, m_initial[i], m_point[i], Scalar(0, 255, 0));
        circle(dst, m_point[i], 3, Scalar(0, 0, 255), -1);
    }
    swap(m_point_prev, m_point);
    swap(m_gray_prev, m_gray);
}


void cv2_10_freaturetracker(void)
{
    FeatureTraker featTracker;
    VideoProc videoProc;
    videoProc.setInput("./Debug/bike.avi");
    videoProc.displayInput("current frame");
    videoProc.displayOutput("output frame");
    videoProc.setFrameRate(1000);

    videoProc.setFrameProcessor(&featTracker);
    videoProc.setFrameProcessorEnable(true);

    videoProc.setOutput("./Debug/bike2.avi", CV_FOURCC('M', 'J', 'P', 'G'));

    videoProc.run();
    videoProc.displayClear();
}
