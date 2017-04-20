//写字板
//http://blog.csdn.net/wangyaninglm/article/details/17091901
#include "comm_def_col.h"

Mat g_img_draw;
Mat g_img_buff;
int g_radius = 5;
int g_drawing = 0;

int g_last_x = 0;
int g_last_y = 0;

#if 0

void draw(int x,int y)
{
    //Draw a circle where is the mouse
    cvCircle(imagen, cvPoint(x,y), r, CV_RGB(red,green,blue), -1, 4, 0);
    //Get clean copy of image
    screenBuffer=cvCloneImage(imagen);
    cvShowImage( "手写板", screenBuffer );
}

#endif

void drawCursor(int x, int y)
{
    //Get clean copy of image
    g_img_buff = g_img_draw.clone();
    //Draw a circle where is the mouse
    circle(g_img_buff, Point(x, y), g_radius, Scalar(0, 0,0), 1, 4, 0);
    imshow("手写板", g_img_buff);
}


void on_mouse( int event, int x, int y, int flags, void* param )
{
    g_last_x = x;
    g_last_y = y;
    drawCursor(x, y);
    //Select mouse Event
    if(event==CV_EVENT_LBUTTONDOWN)
    {
        g_drawing = 1;
//        draw(x,y);
    }
    else if(event==CV_EVENT_LBUTTONUP)
    {
        //drawing=!drawing;
        g_drawing = 0;
    }
    else if(event == CV_EVENT_MOUSEMOVE  &&  (flags & CV_EVENT_FLAG_LBUTTON))
    {
        if(g_drawing);
//            draw(x,y);
    }
    else if (event == CV_EVENT_MOUSEMOVE)
    {
        
    }
}

int main()
{
    printf( "                                 数字字符识别\n"
        "快捷键: \n"
        "\tr - 重置白版\n"
        "\t+ - 笔迹增粗 ++\n"
        "\t- - 笔迹减细 --\n"
        "\ts - 保存输入为 out.pbm\n"	//输入可以作为样本再次部署进去
        "\tc - 输入分类识别, 结果在console显示\n"
        "\tESC - 退出程序\n");

    //Create image
    g_img_draw.create(128, 128, CV_8UC3);
    g_img_draw.setTo(Scalar(255, 255, 0));

    g_img_buff = g_img_draw.clone();

    //Create window
    namedWindow("手写板", 0 );

    resizeWindow("手写板", 512, 512);

    //Create mouse CallBack
    setMouseCallback("手写板",&on_mouse, 0 );
    for(;;)
    {
        int c;

        imshow("手写板", g_img_buff);
        c = waitKey(10);
        if( (char) c == 27 )
            break;
        if( (char) c == '+')
        {
            g_radius++;
            cout<<g_radius<<endl;
            drawCursor(g_last_x, g_last_y);
        }
        if(((char)c == '-') && (g_radius > 1))
        {
            g_radius--;
            cout<<g_radius<<endl;
            drawCursor(g_last_x, g_last_y);
        }
        if( (char)c == 'r')
        {
//            cvSet(imagen, cvRealScalar(255),NULL);
//            drawCursor(last_x,last_y);
        }
        if( (char)c == 's')
        {
//            cvSaveImage("out.pbm", imagen);
        }
        if( (char)c == 'c')
        {
//            ocr.classify(imagen,1);
        }

    }
 

    destroyWindow("手写板");
    return 0;
}