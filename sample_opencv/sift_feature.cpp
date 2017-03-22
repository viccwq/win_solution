#include "comm_def_col.h"

int sift_feature()
{
    //change console color
    system("color 5F");
    
    Mat train_image = imread("1.jpg");
    Mat train_image_gray;
    imshow("source", train_image);
    cvtColor(train_image, train_image_gray, CV_BGR2GRAY);

    vector<KeyPoint> key_point;
    Mat train_desc;

    SiftFeatureDetector feature_detector;
    feature_detector.detect(train_image_gray, key_point);

    SiftDescriptorExtractor feature_extractor;
    feature_extractor.compute(train_image_gray, key_point, train_desc);

    BFMatcher matcher;
    vector<Mat> train_desc_collection(1, train_desc);
    matcher.add(train_desc_collection);
    matcher.train();


    return 0;
}