#ifndef _FEATURE_EXTRA_H
#define _FEATURE_EXTRA_H
#include "comm_def_col.h"

extern Mat feature_dft(const Mat &img_src);

extern vector<Point> feature_edge(const Mat &img_src);

extern int draw_points_8U(const vector<Point> &points, unsigned char val, Mat &img_dst);
#endif