#include "comm_def_col.h"

int sift_feature()
{
    //change console color
    system("color 5F");
    
    Mat train_image = imread("cv3.jpg");
    Mat train_image_gray;
    imshow("source", train_image);
    cvtColor(train_image, train_image_gray, CV_BGR2GRAY);

    SiftFeatureDetector feature_detector;
    SiftDescriptorExtractor feature_extractor;

    vector<KeyPoint> train_key_point;
    Mat train_desc;
    feature_detector.detect(train_image_gray, train_key_point);    
    feature_extractor.compute(train_image_gray, train_key_point, train_desc);

    BFMatcher matcher;
    vector<Mat> train_desc_collection(1, train_desc);
    matcher.add(train_desc_collection);
    matcher.train();

	//create video object
	VideoCapture cap(0);
	unsigned int frame_count = 0;

	while ('q' != char(waitKey(1)))
	{
		double time0 = getTickCount();
		Mat capture_img, capture_img_gray;
        
        //capture a image
        cap >> capture_img;
        if (capture_img.empty())
        {
            continue;
        }

        cvtColor(capture_img, capture_img_gray, CV_BGR2GRAY);

        vector<KeyPoint> test_key_point;
        Mat test_desc;
        feature_detector.detect(capture_img_gray, test_key_point);
        feature_extractor.compute(capture_img_gray, test_key_point, test_desc);

        vector<vector<DMatch>> matches;
        matcher.knnMatch(test_desc, matches, 2);

        vector<DMatch> good_matches;
        for (unsigned int i = 0; i < matches.size(); i++)
        {
            if (matches[i][0].distance < 0.6 * matches[i][1].distance)
            {
                good_matches.push_back(matches[i][0]);
            }
        }
        
        //plot
        Mat dst_img;
        drawMatches(capture_img, test_key_point, train_image, train_key_point, good_matches, dst_img);

        imshow("dst", dst_img);
        cout<<"frequency:"<< getTickFrequency() / (getTickCount() - time0) <<endl;
	}

    return 0;
}