//http://blog.csdn.net/poem_qianmo/article/details/29356187
//http://blog.csdn.net/crzy_sparrow/article/details/7391511
//源码分析
//http://blog.csdn.net/ap1005834/article/details/54381878
#include "comm_def_col.h"

Mat g_src_img, g_src_img1, g_gray_img;  
int thresh = 30; //当前阈值  
int max_thresh = 175; //最大阈值 

void on_coner_harris( int, void* )  
{  

    Mat dst_img;//目标图  
    Mat norm_img;//归一化后的图  
    Mat scaled_img;//线性变换后的八位无符号整型的图  


    //置零当前需要显示的两幅图，即清除上一次调用此函数时他们的值  
    dst_img = Mat::zeros( g_src_img.size(), CV_32FC1 );  
    g_src_img1 = g_src_img.clone(); 

  
    //进行角点检测  
    cornerHarris( g_gray_img, dst_img, 2, 3, 0.04, BORDER_DEFAULT );
    // 归一化与转换  
    normalize( dst_img, norm_img, 0, 255, NORM_MINMAX, CV_32FC1, Mat() );  
    //将归一化后的图线性变换成8位无符号整型  
    convertScaleAbs( norm_img, scaled_img ); 


    // 将检测到的，且符合阈值条件的角点绘制出来  
    for( int j = 0; j < norm_img.rows ; j++ )  
    { 
        for( int i = 0; i < norm_img.cols; i++ )  
        {  
            if( (int) norm_img.at<float>(j,i) > thresh+80 )  
            {  
                circle( g_src_img1, Point( i, j ), 5,  Scalar(10,10,255), 2, 8, 0 );  
                circle( scaled_img, Point( i, j ), 5,  Scalar(0,10,255), 2, 8, 0 );  
            }  
        }  
    }  

    imshow( "src", g_src_img1 );  
    imshow( "dst", scaled_img );  

} 

int harris_feature()
{
    //改变console字体颜色  
    system("color 3F");    

    g_src_img = imread( "house.jpg", 1 );  
    if( !g_src_img.data )
    { 
        printf("read image error\n"); 
        return -1; 
    }    
    imshow("src",g_src_img);  

    g_src_img1 = g_src_img.clone();
    cvtColor( g_src_img1, g_gray_img, CV_BGR2GRAY );  

    //创建窗口和滚动条  
    namedWindow( "src", CV_WINDOW_AUTOSIZE );  
    createTrackbar( "阈值: ", "src", &thresh, max_thresh, on_coner_harris);  

    //调用一次回调函数，进行初始化  
    on_coner_harris( 0, 0 );  

    waitKey(0);  
    return(0);  
}