//Ð´×Ö°å
//http://blog.csdn.net/wangyaninglm/article/details/17091901
#include "comm_def_col.h"

Mat g_img_draw;
Mat g_img_buff;
int g_radius = 5;
int g_drawing = 0;
int g_erase = 0;

int g_last_x = 0;
int g_last_y = 0;

//background color
Scalar g_background = Scalar(255, 255, 0);
//ink color
Scalar g_ink = Scalar(0, 0, 0);
//brush color
Scalar g_brush = g_ink;

void inline draw(int x,int y)
{
    //Draw a circle where is the mouse
    circle(g_img_draw, Point(x, y), g_radius, g_brush, -1, 4, 0);
}

void inline draw_clean()
{
    g_img_draw.setTo(g_background);
}

void inline drawCursor(int x, int y)
{
    //Get clean copy of image
    g_img_buff = g_img_draw.clone();
    //Draw a circle where is the mouse
    circle(g_img_buff, Point(x, y), g_radius, Scalar(0, 0, 255), 1, 4, 0);
}


void on_mouse( int event, int x, int y, int flags, void* param )
{
    g_last_x = x;
    g_last_y = y;
    drawCursor(x, y);
    //Select mouse Event
    if(event == CV_EVENT_LBUTTONDOWN)
    {
        g_drawing = 1;
        draw(x,y);
    }
    else if(event == CV_EVENT_LBUTTONUP)
    {
        //drawing=!drawing;
        g_drawing = 0;
    }
    else if(event == CV_EVENT_MOUSEMOVE  &&  (flags & CV_EVENT_FLAG_LBUTTON))
    {
        if(g_drawing)
           draw(x,y);
    }
    else if (event == CV_EVENT_RBUTTONUP)
    {
        draw_clean();
    }
}

int main()
{
    printf( "                                 Êý×Ö×Ö·ûÊ¶±ð\n"
        "¿ì½Ý¼ü: \n"
        "\tr - ÖØÖÃ°×°æ\n"
        "\t+ - ±Ê¼£Ôö´Ö\n"
        "\t- - ±Ê¼£¼õÏ¸\n"
        "\te - ±Ê¼£²Á³ý(ÓÒ»÷)\n"
        "\ts - ±£´æÍ¼Æ¬\n"
        "\tc - Êý×ÖÊ¶±ð\n"
        "\tESC - ÍË³ö³ÌÐò\n");

    //Create image
    g_img_draw.create(128, 128, CV_8UC3);
    g_img_draw.setTo(g_background);

    g_img_buff = g_img_draw.clone();

    //Create window
    namedWindow("ÊÖÐ´°å", 0 );

    resizeWindow("ÊÖÐ´°å", 512, 512);

    //Create mouse CallBack
    setMouseCallback("ÊÖÐ´°å",&on_mouse, 0 );
    for(;;)
    {
        int c;

        imshow("ÊÖÐ´°å", g_img_buff);
        c = waitKey(10);

        if( (char) c == 27 )
            break;

        switch ((char) c)
        {
        case '+':
            {
                g_radius++;
                cout<<g_radius<<endl;
                drawCursor(g_last_x, g_last_y);    
            }
            break;
        case '-':
            {
                g_radius--;
                cout<<g_radius<<endl;
                drawCursor(g_last_x, g_last_y);
            }
            break;
        case 'r':
            {
                draw_clean();
                drawCursor(g_last_x, g_last_y);
            }
            break;
        case 's':
            {
                imwrite("./Debug/char.bmp", g_img_draw);
                cout<<"/Debug/char.bmp is saved"<<endl;
            }
            break;
        case 'e':
            {
                if (0 == g_erase)
                {
                    g_erase = 1;
                    g_brush = g_background;
                    cout<<"Ereasing . . ."<<endl;
                }
                else
                {
                    g_erase = 0;
                    g_brush = g_ink;
                    cout<<"Drawing . . ."<<endl;
                }
            }
            break;
        case 'c':
            {
                
            }
            break;
        default:
            break;
        }
    }
 

    destroyWindow("ÊÖÐ´°å");
    return 0;
}