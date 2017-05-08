#include "comm_def_col.h"
//http://blog.csdn.net/u011630458/article/details/50561188

std::vector<Point2f> frame1_features_ok, frame2_features_ok;

void getMatcher(const Mat &src1, const Mat &src2)
{
    //-- Step 1: Detect the keypoints using SURF Detector
    int minHessian = 800;
    SurfFeatureDetector detector(minHessian);
    std::vector<KeyPoint> keypoints_1, keypoints_2;
    detector.detect(src1, keypoints_1);
    detector.detect(src2, keypoints_2);
    int i, count = 0;

    //-- Step 2: Calculate descriptors (feature vectors)
    SurfDescriptorExtractor extractor;
    Mat descriptors_1, descriptors_2;
    extractor.compute(src1, keypoints_1, descriptors_1);
    extractor.compute(src2, keypoints_2, descriptors_2);

    //-- Step 3: Matching descriptor vectors with a brute force matcher
    FlannBasedMatcher matcher;
    std::vector< DMatch > matches;
    matcher.match(descriptors_1, descriptors_2, matches);

    //-- Step 4: Find good matcher
    double max_dist = 0; double min_dist = 100;
    for( int i = 0; i < descriptors_1.rows; i++ )
    {
        double dist = matches[i].distance;
        if(dist < min_dist ) 
            min_dist = dist;
        if(dist > max_dist )
            max_dist = dist; 
    }  


    std::vector<KeyPoint> good_keypoints_1, good_keypoints_2;
    std::vector< DMatch > good_matches;  
    for(i = 0; i < descriptors_1.rows; i++ )
    {
        if(matches[i].distance < 2*min_dist)
        {
            count += 1;
            good_matches.push_back(matches[i]);
            good_keypoints_1.push_back(keypoints_1[matches[i].queryIdx]);
            good_keypoints_2.push_back(keypoints_2[matches[i].trainIdx]);
        }
    }

    Point2f tmpPoint;
    Mat src1_t = src1.clone();
    Mat src2_t = src2.clone();
    for(i=0; i<count; i++)
    {
        circle(src1_t, cvPoint(good_keypoints_1[i].pt.x, good_keypoints_1[i].pt.y),
            3, cvScalar(255, 0, 0), -1);
        circle(src2_t, cvPoint(good_keypoints_2[i].pt.x, good_keypoints_2[i].pt.y),
            3, cvScalar(255, 0, 0), -1);
        tmpPoint.x = good_keypoints_1[i].pt.x;		
        tmpPoint.y = good_keypoints_1[i].pt.y;		
        frame1_features_ok.push_back(tmpPoint);

        tmpPoint.x = good_keypoints_2[i].pt.x;		
        tmpPoint.y = good_keypoints_2[i].pt.y;		
        frame2_features_ok.push_back(tmpPoint);
    }
    //-- Step 5:Show detected matches
    imshow("src1", src1_t);
    imshow("src2", src2_t);
}


int cutSport(const Mat &src, Mat &dst)
{
    Mat mat;
    mat = estimateRigidTransform(frame1_features_ok, frame2_features_ok, true);
    warpAffine(src, dst, mat, Size(2,3));

    imshow("dst", dst);
    return 0;
}


void surf_resigtration()
{

    Mat src1 = imread("./1.jpg", 0);
    Mat src2 = imread("./2.jpg", 0);
    Mat dst;
    getMatcher(src1, src2);
    cutSport(src1, dst);
}