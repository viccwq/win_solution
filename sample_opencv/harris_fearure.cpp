//http://blog.csdn.net/poem_qianmo/article/details/29356187
//http://blog.csdn.net/crzy_sparrow/article/details/7391511
//Դ�����
//http://blog.csdn.net/ap1005834/article/details/54381878
#include "comm_def_col.h"

Mat g_src_img, g_src_img1, g_gray_img;  
int thresh = 30; //��ǰ��ֵ  
int max_thresh = 175; //�����ֵ 

void on_coner_harris( int, void* )  
{  

    Mat dst_img;//Ŀ��ͼ  
    Mat norm_img;//��һ�����ͼ  
    Mat scaled_img;//���Ա任��İ�λ�޷������͵�ͼ  


    //���㵱ǰ��Ҫ��ʾ������ͼ���������һ�ε��ô˺���ʱ���ǵ�ֵ  
    dst_img = Mat::zeros( g_src_img.size(), CV_32FC1 );  
    g_src_img1 = g_src_img.clone(); 

  
    //���нǵ���  
    cornerHarris( g_gray_img, dst_img, 2, 3, 0.04, BORDER_DEFAULT );
    // ��һ����ת��  
    normalize( dst_img, norm_img, 0, 255, NORM_MINMAX, CV_32FC1, Mat() );  
    //����һ�����ͼ���Ա任��8λ�޷�������  
    convertScaleAbs( norm_img, scaled_img ); 


    // ����⵽�ģ��ҷ�����ֵ�����Ľǵ���Ƴ���  
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
    //�ı�console������ɫ  
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

    //�������ں͹�����  
    namedWindow( "src", CV_WINDOW_AUTOSIZE );  
    createTrackbar( "��ֵ: ", "src", &thresh, max_thresh, on_coner_harris);  

    //����һ�λص����������г�ʼ��  
    on_coner_harris( 0, 0 );  

    waitKey(0);  
    return(0);  
}