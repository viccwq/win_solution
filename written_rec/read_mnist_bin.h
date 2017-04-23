#ifndef _READ_MNIST_H
#define _READ_MNIST_H
#include "comm_def_col.h"

#define FILE_TRAIN_IMG          "train-images.idx3-ubyte"
#define FILE_TRAIN_LABEL        "train-labels.idx1-ubyte"
#define FILE_TEST_IMG           "t10k-images.idx3-ubyte"
#define FILE_TEST_LABEL         "t10k-labels.idx1-ubyte"

#define FILE_MNIST_XML          "cv_mnist.xml"

class ReadMnist
{
public:
    ReadMnist(void);
    ~ReadMnist(void);

    static Mat train_img;
    static Mat train_label;
    static Mat test_img;
    static Mat test_label;
    static Mat avg_img;
    static void read_files();
    //add border around the image according to the avg_img;
    static Mat add_border(const Mat &img_src);

};



#endif