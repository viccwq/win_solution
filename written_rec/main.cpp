//д�ְ�
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
    cvShowImage( "��д��", screenBuffer );
}

#endif

void drawCursor(int x, int y)
{
    //Get clean copy of image
    g_img_buff = g_img_draw.clone();
    //Draw a circle where is the mouse
    circle(g_img_buff, Point(x, y), g_radius, Scalar(0, 0,0), 1, 4, 0);
    imshow("��д��", g_img_buff);
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
    printf( "                                 �����ַ�ʶ��\n"
        "��ݼ�: \n"
        "\tr - ���ðװ�\n"
        "\t+ - �ʼ����� ++\n"
        "\t- - �ʼ���ϸ --\n"
        "\ts - ��������Ϊ out.pbm\n"	//���������Ϊ�����ٴβ����ȥ
        "\tc - �������ʶ��, �����console��ʾ\n"
        "\tESC - �˳�����\n");

    //Create image
    g_img_draw.create(128, 128, CV_8UC3);
    g_img_draw.setTo(Scalar(255, 255, 0));

    g_img_buff = g_img_draw.clone();

    //Create window
    namedWindow("��д��", 0 );

    resizeWindow("��д��", 512, 512);

    //Create mouse CallBack
    setMouseCallback("��д��",&on_mouse, 0 );
    for(;;)
    {
        int c;

        imshow("��д��", g_img_buff);
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
 

    destroyWindow("��д��");
    return 0;
}