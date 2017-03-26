//http://www.linuxidc.com/Linux/2016-06/132758.htm

#include "comm_def_col.h"
#define FILE_TRAIN_IMG          "train-images.idx3-ubyte"
#define FILE_TRAIN_LABEL        "train-labels.idx1-ubyte"
#define FILE_TEST_IMG           "t10k-images.idx3-ubyte"
#define FILE_TEST_LABEL         "t10k-labels.idx1-ubyte"

using namespace cv;
using namespace cv::ml;

extern Mat read_mnist_image(const string file_name);
extern Mat read_mnist_label(const string file_name);
extern double g_cost_time;
extern clock_t g_start_time;
extern clock_t g_end_time;


int main()
{
    Mat train_img;;
    Mat train_img_i = read_mnist_image(FILE_TRAIN_IMG);
    if (!train_img_i.data)
    {
        return -1;
    }

    Mat train_label;
    Mat train_label_i = read_mnist_label(FILE_TRAIN_LABEL);
    if (!train_label_i.data)
    {
        return -1;
    }

    Mat test_img;
    Mat test_img_i = read_mnist_image(FILE_TEST_IMG);
    if (!test_img_i.data)
    {
        return -1;
    }

    Mat test_label;
    Mat test_label_i = read_mnist_label(FILE_TEST_LABEL);
    if (!test_label_i.data)
    {
        return -1;
    }

    train_img = Mat::convertTo(train_img_i, CV_32FC1);
    train_label = Mat::convertTo(train_label_i, CV_32SC1);
    test_img = Mat::convertTo(test_img_i, CV_32FC1);
    test_label = Mat::convertTo(test_label_i, CV_32SC1);

    //Set up the support vector machines parameters
    Ptr<SVM> svm = SVM::create();
    svm->setType(SVM::C_SVC);
    svm->setKernel(SVM::RBF);
    //svm->setDegree(10.0);
    svm->setGamma(0.01);
    //svm->setCoef0(1.0);
    svm->setC(10.0);
    //svm->setNu(0.5);
    //svm->setP(0.1);
    svm->setTermCriteria(TermCriteria(CV_TERMCRIT_EPS, 1000, FLT_EPSILON));


    //Train the svm
    cout << "Starting training process" << endl;
    g_start_time = clock();
    svm->train(train_img, ROW_SAMPLE, train_label);
    g_end_time = clock();
    g_cost_time = (g_end_time - g_start_time) / CLOCKS_PER_SEC;
    cout << "Finished training process...cost " << g_cost_time << " seconds..." << endl;
    
    //save the svm
    svm->save("mnist_svm.xml");
    cout << "save as /mnist_dataset/mnist_svm.xml" << endl;
    
    
    //load the svm ----------------------------------------------------
    cout << "import SVM file ...\n";
    Ptr<SVM> svm1 = StatModel::load<SVM>("mnist_svm.xml");
    cout << "import SVM successfully...\n";
    
    
    //read the test dataset
    cout << "import test data...\n";    
    
    float count = 0;
    for (int i = 0; i < test_img.rows; i++) {
        Mat sample = test_img.row(i);
        float res = svm1->predict(sample);
        res = std::abs(res - tLabel.at<unsigned int>(i, 0)) <= FLT_EPSILON ? 1.f : 0.f;
        count += res;
    }
    cout << "correct count = " << count << endl;
    cout << "fail rate..." << (10000 - count + 0.0) / 10000 * 100.0 << "%....\n";

    system("pause");
    return 0;
}
