/**
 * @function Geometric_Transforms_Demo.cpp
 * @brief Demo code for Geometric Transforms
 * @author OpenCV team
 */
//http://blog.csdn.net/chenjiazhou12/article/details/21827069
#include "comm_def_col.h"
#include <iostream>
#include <stdio.h>

using namespace cv;
using namespace std;

/// Global variables
const char* source_window = "Source image";
const char* warp_window = "Warp";
const char* warp_rotate_window = "Warp + Rotate";

/**
 * @function main
 */
int geom_trans(const string file_name)
{
  Point2f srcTri[3];
  Point2f dstTri[3];

  Mat rot_mat( 2, 3, CV_32FC1 );
  Mat warp_mat( 2, 3, CV_32FC1 );
  Mat src, warp_dst, warp_rotate_dst;

  /// Load the image
  if (!file_name.empty())
      src = imread(file_name, 1 );
  else   
      src = imread("./affine.bmp", 1 );



  /// Set the dst image the same type and size as src
  warp_dst = Mat::zeros( src.rows, src.cols, src.type() );

  /// Set your 3 points to calculate the  Affine Transform
#if 0
  srcTri[0] = Point2f( 0,0 );
  srcTri[1] = Point2f( src.cols - 1.f, 0 );
  srcTri[2] = Point2f( 0, src.rows - 1.f );

  dstTri[0] = Point2f( src.cols*0.0f, src.rows*0.1f );
  dstTri[1] = Point2f( src.cols*0.85f, src.rows*0.25f );
  dstTri[2] = Point2f( src.cols*0.5f, src.rows*0.7f );
#else
  srcTri[0] = Point2f( 84, 87);
  srcTri[1] = Point2f( 103, 265);
  srcTri[2] = Point2f( 414, 251);

//   srcTri[0] = Point2f( 87, 84);
//   srcTri[1] = Point2f( 265, 103);
//   srcTri[2] = Point2f( 251, 414);


  dstTri[0] = Point2f( 0, 0 );
  dstTri[1] = Point2f( 0, (src.rows - 2.f));
  dstTri[2] = Point2f( (src.cols - 2.f), (src.rows - 2.f));

#endif
  /// Get the Affine Transform
  warp_mat = getAffineTransform( srcTri, dstTri );

  /// Apply the Affine Transform just found to the src image
  warpAffine( src, warp_dst, warp_mat, warp_dst.size() );

  /** Rotating the image after Warp */

  /// Compute a rotation matrix with respect to the center of the image
  Point center = Point( warp_dst.cols/2, warp_dst.rows/2 );
  double angle = -50.0;
  double scale = 0.6;

  /// Get the rotation matrix with the specifications above
  rot_mat = getRotationMatrix2D( center, angle, scale );

  /// Rotate the warped image
  warpAffine( warp_dst, warp_rotate_dst, rot_mat, warp_dst.size() );


  /// Show what you got
  namedWindow( source_window, WINDOW_AUTOSIZE );
  imshow( source_window, src );

  namedWindow( warp_window, WINDOW_AUTOSIZE );
  imshow( warp_window, warp_dst );

  namedWindow( warp_rotate_window, WINDOW_AUTOSIZE );
  imshow( warp_rotate_window, warp_rotate_dst );

  /// Wait until user exits the program
  waitKey(0);

  return 0;
}
