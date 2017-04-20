//references
//http://www.linuxidc.com/Linux/2016-06/132758.htm
//http://blog.csdn.net/firefight/article/details/6452188
#include "comm_def_col.h"
#define FILE_TRAIN_IMG          "train-images.idx3-ubyte"
#define FILE_TRAIN_LABEL        "train-labels.idx1-ubyte"
#define FILE_TEST_IMG           "t10k-images.idx3-ubyte"
#define FILE_TEST_LABEL         "t10k-labels.idx1-ubyte"

using namespace cv;

extern Mat read_mnist_image(const string file_name);
extern Mat read_mnist_label(const string file_name);
extern double g_cost_time;
extern clock_t g_start_time;
extern clock_t g_end_time;


int rec_svm()
{
    Mat train_img;
    Mat train_img_i;
    Mat train_label;
    Mat train_label_i;
    Mat test_img;
    Mat test_img_i;
    Mat test_label;
    Mat test_label_i;

    //try to load database from XML
    FileStorage fs_r;
    bool exist = fs_r.open("cv_mnist.xml", FileStorage::READ);
    if (exist)
    {
        fs_r["train_img"] >> train_img_i;
        fs_r["train_label"] >> train_label_i;
        fs_r["test_img"] >> test_img_i;
        fs_r["test_label"] >> test_label_i;
        
    }
    fs_r.release();

    //read the original mnist data and save as XML file
    if (!exist)
    {
        train_img_i = read_mnist_image(FILE_TRAIN_IMG);
        if (!train_img_i.data)
        {
            return -1;
        }

        train_label_i = read_mnist_label(FILE_TRAIN_LABEL);
        if (!train_label_i.data)
        {
            return -1;
        }

        test_img_i = read_mnist_image(FILE_TEST_IMG);
        if (!test_img_i.data)
        {
            return -1;
        }

        test_label_i = read_mnist_label(FILE_TEST_LABEL);
        if (!test_label_i.data)
        {
            return -1;
        }
        FileStorage fs("cv_mnist.xml", FileStorage::WRITE);
        fs << "train_img" << train_img_i;
        fs << "train_label" << train_label_i;
        fs << "test_img" << test_img_i;
        fs << "test_label" << test_label_i;
        fs.release();
    }
    

    //normalized
    train_img_i.convertTo(train_img, CV_32FC1, (1.0 / 255));
    train_label_i.convertTo(train_label, CV_32SC1);
    //normalized
    test_img_i.convertTo(test_img, CV_32FC1, (1.0 / 255));
    test_label_i.convertTo(test_label, CV_32SC1);

    //Set up the support vector machines parameters
    //Set SVM params
    CvSVMParams SVM_params;
    SVM_params.svm_type = CvSVM::C_SVC;
    SVM_params.kernel_type = CvSVM::RBF;
    //SVM_params.degree = 10.0;
    SVM_params.gamma = 0.01;
    //SVM_params.coef0 = 1.0;
    SVM_params.C = 10.0;
    //SVM_params.nu = 0.5;
    //SVM_params.p = 0.1;
    SVM_params.term_crit = cvTermCriteria(CV_TERMCRIT_EPS, 1000, FLT_EPSILON);

    //Train the svm
    cout << "Starting training process" << endl;
    g_start_time = clock();
    CvSVM svmClassifier(train_img, train_label, Mat(), Mat(), SVM_params);
    g_end_time = clock();
    g_cost_time = (g_end_time - g_start_time) / CLOCKS_PER_SEC;
    cout << "Finished training process...cost " << g_cost_time << " seconds..." << endl;
     
    //save the svm
    svmClassifier.save("mnist_svm.xml");
    cout << "save as mnist_svm.xml" << endl;
    
    
    //load the svm
    //cout << "import SVM file ...\n";
    //Ptr<SVM> svm1 = StatModel::load<SVM>("mnist_svm.xml");
    //cout << "import SVM successfully...\n";

    float count = 0;
    for (int i = 0; i < test_img.rows; i++) 
    {
        Mat sample = test_img.row(i);
        float res = svmClassifier.predict(sample);
        res = std::abs(res - test_label.at<unsigned int>(i, 0)) <= FLT_EPSILON ? 1.f : 0.f;
        count += res;
    }
    cout << "correct count = " << count << endl;
    cout << "fail rate..." << (10000 - count + 0.0) / 10000 * 100.0 << "%....\n";

    system("pause");
    return 0;
}
