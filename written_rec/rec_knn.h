#ifndef _REC_KNN_H
#define _REC_KNN_H
#include "comm_def_col.h"
#include "read_mnist_bin.h"
class RecKnn
{
public:
    RecKnn();
    ~RecKnn();
    void train();
    void test();
    int classify(const Mat &img_src);
protected:
private:
    CvKNearest *m_knn;
    int m_trained;
    Mat m_train_img;
    Mat m_train_label;
    Mat m_test_img;
    Mat m_test_label;
    int m_K;

};

#endif