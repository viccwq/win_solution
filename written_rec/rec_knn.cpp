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
    ReadMnist::train_img(Range(0,1000), Range::all()).convertTo(this->m_train_img, CV_32FC1, 1.0 / 255.0);
    ReadMnist::train_label(Range(0,1000), Range::all()).convertTo(this->m_train_label, CV_32FC1, 1.0);
    ReadMnist::test_img(Range(0,1000), Range::all()).convertTo(this->m_test_img, CV_32FC1, 1.0 / 255.0);
    ReadMnist::test_label(Range(0,1000), Range::all()).convertTo(this->m_test_label, CV_32FC1, 1.0);

    m_knn = new CvKNearest;
    m_knn->train(m_train_img, m_train_label, Mat(), false, m_K);


    m_trained = 1;
}

void RecKnn::test()
{
    float ret;
    for (int i = 0; i < 100; i++)
    {
        ret = m_knn->find_nearest(m_test_img.row(i), 3);
        cout<<"label:"<<m_test_label.at<float>(i, 0)<<"\t"<<"result:"<<ret<<endl;
    }
}
