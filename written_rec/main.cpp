#include "comm_def_col.h"
#include "pre_proc.hpp"
#include "read_mnist_bin.h"
#include "rec_knn.h"
#include "feature_extra.h"

extern int rec_svm();

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
    circle(g_img_draw, Point(x, y), g_radius, g_brush, -1, CV_AA, 0);
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
    printf( "                                 数字字符识别\n"
        "快捷键: \n"
        "\tr - 重置白版\n"
        "\t+ - 笔迹增粗\n"
        "\t- - 笔迹减细\n"
        "\te - 笔迹擦除(右击)\n"
        "\ts - 保存图片\n"
        "\tc - 数字识别\n"
        "\tESC - 退出程序\n");

    //load the mnist image
    ReadMnist::read_files();
    RecKnn rec_knn;
    rec_knn.train();
    rec_knn.test();

    //Create image
    g_img_draw.create(128, 128, CV_8UC3);
    g_img_draw.setTo(g_background);

    g_img_buff = g_img_draw.clone();

    //Create window
    namedWindow("手写板", 0 );

    resizeWindow("手写板", 512, 512);

    namedWindow("Input Image", WINDOW_NORMAL);
    resizeWindow("Input Image", 120, 120);

    namedWindow("spectrum magnitude", WINDOW_NORMAL);
    resizeWindow("spectrum magnitude", 120, 120);

    //Create mouse CallBack
    setMouseCallback("手写板",&on_mouse, 0 );
    for(;;)
    {
        int c;

        imshow("手写板", g_img_buff);
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
                imwrite("./Debug/char2.bmp", pre_process(g_img_draw));
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
        case 'd':
            {
                //remove the redundant border of the image
                //and pad the image with same width and height
                Mat img = pre_process(g_img_draw);
                
                if (!img.empty())
                {
                    //revert the color
                    img = 255 - img;
                    img = ReadMnist::add_border(img);

                    open_close(img, 6, 1, img);
                    //find the contours of the hand write
                    vector<Point> edge = feature_edge(img);
                    Mat img_draw(img.size(), CV_8U, Scalar::all(255));
                    draw_points_8U(edge, 0, img_draw);
                    imwrite("./Debug/contours.bmp", img_draw);

                    //选取不同数量的谐波分量
                    for (int i = 0; i < 10; i++)
                    {
                        vector<Point> edge2;
                        dftLowPass(edge, edge.size(), (i * 2 + 1), edge2);

                        Mat img_draw2(img.size(), CV_8U, Scalar::all(255));
                        draw_points_8U(edge2, 0, img_draw2);
                        char file_name[128] = {0};
                        sprintf(file_name, "./Debug/contours%02d.bmp", (i * 2 + 1));
                        string str(file_name);   
                        imwrite(str, img_draw2); 
                    }

                    //打印不同图像的低频分量
#if defined(_DEBUG) && 1 
                    {
                        int len = 5;
                        vector<double> mag;
                        dftFreqMagnitude(edge, edge.size(), len, mag);
                        
                        //normalize the magnitude
                        double max = mag[0], min = mag[0], delta = 0.0;
                        for (int i = 0; i < mag.size(); i++)
                        {
                            if (mag[i] > max)
                                max = mag[i];
                            if (mag[i] < min)
                                min = mag[i];
                        }
                        delta = max - min;
                        for (int i = 0; i < mag.size(); i++)
                        {
                            mag[i] = (mag[i] - min)/delta;
                            cout<<"\t"<<mag[i];
                        }
                        cout<<endl;                        

                    }
#endif // _DEBUG

            
                }
            }
            break;
        case 'c':
            {
                //remove the redundant border of the image
                //and pad the image with same width and height
                Mat img = pre_process(g_img_draw);

                if (!img.empty())
                {
                    //revert the color
                    img = 255 - img;
                    img = ReadMnist::add_border(img);

                    open_close(img, 7, 1, img);
                    cout<<rec_knn.classcify(img)<<endl;               
                }
            }
            break;
        default:
            break;
        }
    }
 

    destroyWindow("手写板");
    destroyWindow("Input Image");

    destroyWindow("spectrum magnitude");
    return 0;
}