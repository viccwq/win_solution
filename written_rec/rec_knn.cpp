//Ğ´×Ö°å(KNN)
//http://blog.csdn.net/wangyaninglm/article/details/17091901
//knn simple sample
//http://blog.csdn.net/IAccepted/article/details/41776665

#include "pre_proc.hpp"
#include "rec_knn.h"


RecKnn::RecKnn()
{
    m_trained = 0;
    m_K = 5;
}

RecKnn::~RecKnn()
{

}

void RecKnn::train()
{
    if (m_trained)
    {
        return;
    }
    //load data
    ReadMnist::train_img(Range(0,2000), Range::all()).convertTo(this->m_train_img, CV_32FC1, 1.0 / 255.0);
    ReadMnist::train_label(Range(0,2000), Range::all()).convertTo(this->m_train_label, CV_32FC1, 1.0);
    ReadMnist::test_img(Range(0,1000), Range::all()).convertTo(this->m_test_img, CV_32FC1, 1.0 / 255.0);
    ReadMnist::test_label(Range(0,1000), Range::all()).convertTo(this->m_test_label, CV_32FC1, 1.0);

    m_knn = new CvKNearest;
    m_knn->train(m_train_img, m_train_label, Mat(), false, m_K);


    m_trained = 1;
}

void RecKnn::test()
{
    float ret;
    int rate = 0;
    for (int i = 0; i < 1000; i++)
    {
        ret = m_knn->find_nearest(m_test_img.row(i), 5);
//        cout<<"label:"<<m_test_label.at<float>(i, 0)<<"\t"<<"result:"<<ret<<endl;
        if (abs(ret - m_test_label.at<float>(i, 0)) < 0.0001)
            rate++;
    }
    cout<<"Pass rate is: "<<(rate/10.0)<<"%"<<endl;
}

int RecKnn::classcify( const Mat &img_src )
{
    //reshape the image
    Mat img_test;
    resize(img_src, img_test, Size(28, 28));
    img_test = img_test.reshape(0, 1);

    Mat img_classify;
    img_test.convertTo(img_classify, CV_32FC1, 1.0 / 255.0);

    float ret = m_knn->find_nearest(img_classify, 5);

    return cvRound(ret);
}
