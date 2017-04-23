
//data resource
//http://yann.lecun.com/exdb/mnist/
//reference
//http://www.linuxidc.com/Linux/2016-06/132758.htm
////////////////////////////////// FILE FORMATS FOR THE MNIST DATABASE ////////////////////////////////////////
/*
All the integers in the files are stored in the MSB first (high endian) format used by most 
non-Intel processors. Users of Intel processors and other low-endian machines must flip the 
bytes of the header.

There are 4 files:

train-images-idx3-ubyte: training set images 
train-labels-idx1-ubyte: training set labels 
t10k-images-idx3-ubyte:  test set images 
t10k-labels-idx1-ubyte:  test set labels

The training set contains 60000 examples, and the test set 10000 examples.

The first 5000 examples of the test set are taken from the original NIST training set. The 
last 5000 are taken from the original NIST test set. The first 5000 are cleaner and easier 
than the last 5000.

TRAINING SET LABEL FILE (train-labels-idx1-ubyte):

[offset] [type]          [value]          [description] 
0000     32 bit integer  0x00000801(2049) magic number (MSB first) 
0004     32 bit integer  60000            number of items 
0008     unsigned byte   ??               label 
0009     unsigned byte   ??               label 
........ 
xxxx     unsigned byte   ??               label
The labels values are 0 to 9.

TRAINING SET IMAGE FILE (train-images-idx3-ubyte):

[offset] [type]          [value]          [description] 
0000     32 bit integer  0x00000803(2051) magic number 
0004     32 bit integer  60000            number of images 
0008     32 bit integer  28               number of rows 
0012     32 bit integer  28               number of columns 
0016     unsigned byte   ??               pixel 
0017     unsigned byte   ??               pixel 
........ 
xxxx     unsigned byte   ??               pixel
Pixels are organized row-wise. Pixel values are 0 to 255. 0 means background (white), 255 means 
foreground (black).

TEST SET LABEL FILE (t10k-labels-idx1-ubyte):

[offset] [type]          [value]          [description] 
0000     32 bit integer  0x00000801(2049) magic number (MSB first) 
0004     32 bit integer  10000            number of items 
0008     unsigned byte   ??               label 
0009     unsigned byte   ??               label 
........ 
xxxx     unsigned byte   ??               label
The labels values are 0 to 9.

TEST SET IMAGE FILE (t10k-images-idx3-ubyte):

[offset] [type]          [value]          [description] 
0000     32 bit integer  0x00000803(2051) magic number 
0004     32 bit integer  10000            number of images 
0008     32 bit integer  28               number of rows 
0012     32 bit integer  28               number of columns 
0016     unsigned byte   ??               pixel 
0017     unsigned byte   ??               pixel 
........ 
xxxx     unsigned byte   ??               pixel
Pixels are organized row-wise. Pixel values are 0 to 255. 0 means background (white), 255
means foreground (black). 
*/

#include "read_mnist_bin.h"
#include "io.h"

double g_cost_time = 0;
clock_t g_start_time;
clock_t g_end_time;

static int reverse_int(int i) 
{
    unsigned char c[4];
    
    c[3] = i & 255;
    c[2] = (i >> 8) & 255;
    c[1] = (i >> 16) & 255;
    c[0] = (i >> 24) & 255;    
    return *(int *)c;
}


Mat read_mnist_image(const string file_name) 
{
    int magic_number = 0;
    int number_of_images = 0;
    int n_rows = 0;
    int n_cols = 0;
    
    Mat DataMat;
    
    ifstream file(file_name, ios::binary);
    if (file.is_open())
    {
        file.read((char*)&magic_number, sizeof(magic_number));
        file.read((char*)&number_of_images, sizeof(number_of_images));
        file.read((char*)&n_rows, sizeof(n_rows));
        file.read((char*)&n_cols, sizeof(n_cols));
        //cout << magic_number << " " << number_of_images << " " << n_rows << " " << n_cols << endl;
    
        magic_number = reverse_int(magic_number);
        number_of_images = reverse_int(number_of_images);
        n_rows = reverse_int(n_rows);
        n_cols = reverse_int(n_cols);
        cout << "MAGIC NUMBER = " << magic_number
            << "\nNUMBER OF IMAGES = " << number_of_images
            << "\nNUMBER OF ROWS = " << n_rows
            << "\nNUMBER OF COLS = " << n_cols << endl;
    
        //-test-
        //number_of_images = testNum;

        Mat s = Mat::zeros(n_rows, 4 * n_cols, CV_8UC1);
        Mat e = Mat::zeros(n_rows, 4 * n_cols, CV_8UC1);
    
        cout << "start read image......\n";
        g_start_time = clock();

        DataMat = Mat::zeros(number_of_images, n_rows * n_cols, CV_8UC1);
        for (int i = 0; i < number_of_images; i++) 
        {
            uchar *data = DataMat.ptr<uchar>(i);
            file.read((char*)data, n_rows * n_cols);

            if (i == 0) 
            {
                for (int j = 0; j < n_rows; j++)
                {
                    uchar *data_1 = s.ptr<uchar>(j);
                    memcpy(data_1, data + (j * n_cols), n_cols);
                }
            }
            else if (i == number_of_images - 1) 
            {
                for (int j = 0; j < n_rows; j++)
                {
                    uchar *data_1 = e.ptr<uchar>(j);
                    memcpy(data_1, data + (j * n_cols), n_cols);
                }
            }
        }

        g_end_time = clock();
        g_cost_time = (g_end_time - g_start_time) / CLOCKS_PER_SEC;
        cout << "read image finished......" << g_cost_time << "s\n";
    
        imshow("first image", s);
        imshow("last image", e);
        waitKey(0);
    }
    file.close();
    return DataMat;
}


Mat read_mnist_label(const string file_name) 
{
    int magic_number = 0;
    int number_of_labels = 0;
    int n_rows = 0;
    int n_cols = 0;

    Mat DataMat;

    ifstream file(file_name, ios::binary);
    if (file.is_open())
    {
        file.read((char*)&magic_number, sizeof(magic_number));
        file.read((char*)&number_of_labels, sizeof(number_of_labels));
        //cout << magic_number << " " << number_of_images << " " << n_rows << " " << n_cols << endl;

        magic_number = reverse_int(magic_number);
        number_of_labels = reverse_int(number_of_labels);

        cout << "MAGIC NUMBER = " << magic_number
            << "\nNUMBER OF IMAGES = " << number_of_labels << endl;

        //-test-
        unsigned int s = 0, e = 0;

        cout << "start read label ......\n";
        g_start_time = clock();

        DataMat = Mat::zeros(number_of_labels, 1, CV_8UC1);
        for (int i = 0; i < number_of_labels; i++) 
        {
            uchar *data = DataMat.ptr<uchar>(i);
            file.read((char*)data, 1);

            if (i == 0) 
            {
                s = (unsigned int)(data[0]);
            }
            else if (i == number_of_labels - 1) 
            {
                e = (unsigned int)(data[0]);
            }
        }

        g_end_time = clock();
        g_cost_time = (g_end_time - g_start_time) / CLOCKS_PER_SEC;
        cout << "read label finished......" << g_cost_time << "s\n";

        cout << "first label = " << s << endl;
        cout << "last label = " << e << endl;

        //waitKey(0);
    }
    file.close();
    return DataMat;
}


Mat ReadMnist::train_img    = Mat::Mat(Size(1,1), CV_8UC1, NULL);
Mat ReadMnist::train_label  = Mat::Mat(Size(1,1), CV_8UC1, NULL);
Mat ReadMnist::test_img     = Mat::Mat(Size(1,1), CV_8UC1, NULL);
Mat ReadMnist::test_label   = Mat::Mat(Size(1,1), CV_8UC1, NULL);
Mat ReadMnist::avg_img      = Mat::Mat(Size(1,1), CV_8UC1, NULL);

void ReadMnist::read_files()
{
    if (NULL != ReadMnist::train_img.data &&
        NULL != ReadMnist::train_label.data &&
        NULL != ReadMnist::test_img.data &&
        NULL != ReadMnist::test_label.data)
    {
        return;
    }

    //load the image and label from xml
    if (-1 != (_access(FILE_MNIST_XML, 0)))
    {
        FileStorage fs_r;
        fs_r.open(FILE_MNIST_XML, FileStorage::READ);
        fs_r["train_img"] >> ReadMnist::train_img;
        fs_r["train_label"] >> ReadMnist::train_label;
        fs_r["test_img"] >> ReadMnist::test_img;
        fs_r["test_label"] >> ReadMnist::test_label;
        fs_r.release();
    }
    //load the image and label from binary files
    else
    {
        ReadMnist::train_img = read_mnist_image(FILE_TRAIN_IMG);
        ReadMnist::train_label = read_mnist_image(FILE_TRAIN_LABEL);
        ReadMnist::test_img = read_mnist_image(FILE_TEST_IMG);
        ReadMnist::test_label = read_mnist_image(FILE_TRAIN_LABEL);

        FileStorage fs(FILE_MNIST_XML, FileStorage::WRITE);
        fs << "train_img" << ReadMnist::train_img;
        fs << "train_label" << ReadMnist::train_label;
        fs << "test_img" << ReadMnist::test_img;
        fs << "test_label" << ReadMnist::test_label;
        fs.release();
    }
}

cv::Mat ReadMnist::add_border( const Mat &img_src )
{
    if (NULL == ReadMnist::train_img.data ||
        NULL == ReadMnist::train_label.data ||
        NULL == ReadMnist::test_img.data ||
        NULL == ReadMnist::test_label.data)
    {
        return Mat::Mat(Size(1,1), CV_8UC1, NULL);
    }


    if (NULL == ReadMnist::avg_img.data)
    {
        Mat img_sum = Mat::zeros(Size(ReadMnist::train_img.cols, 1), CV_32FC1);
        int count = 300;
        for (int i = 0; i <count; i++)
        {
            Mat img_temp = ReadMnist::train_img.row(i);
            img_temp.convertTo(img_temp, CV_32FC1, 1.0);
            img_sum += img_temp;
        }
        threshold(img_sum, img_sum, 1, 255, THRESH_BINARY);
    
        img_sum.convertTo(img_sum, CV_8UC1, 1.0);
        ReadMnist::avg_img = img_sum.reshape(0, 28);
//         imshow("img_sum", ReadMnist::avg_img);
//         waitKey(0);
    }

    //expand input image to optimal size
    Mat padded;                            
    int m = img_src.rows;
    int n = img_src.cols;
    int delta = m - n;
    if (delta > 0)
    {
        m += floor(img_src.rows * 0.2);
        n = m;
    }
    else
    {
        n += floor(img_src.cols * 0.2);
        m = n;
    }

    copyMakeBorder(img_src, padded, \
        floor((m - img_src.rows)/2.0), floor((m - img_src.rows)/2.0), \
        floor((m - img_src.cols)/2.0), floor((m - img_src.cols)/2.0), \
        BORDER_CONSTANT, Scalar::all(0));

    return padded;

}

ReadMnist::ReadMnist(void)
{
}

ReadMnist::~ReadMnist(void)
{
}


