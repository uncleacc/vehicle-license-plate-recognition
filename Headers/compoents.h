#ifndef TEST_COMPONENTS_H
#define TEST_COMPONENTS_H
#include <iostream>
#include <string>
#include <algorithm>
#include <opencv2/opencv.hpp>
#include <cmath>

using namespace std;
using namespace cv;

extern int cnt;
string C(int n);
void Show(Mat &src, string name = C(++cnt));
void RotatedRect_Drawing(Mat &image, RotatedRect &rt, Scalar color, int thickness);
Mat nearestNeighRotate(cv::Mat img, float angle);
void Rotate(Mat &img1, Mat &img2, double angle, double ratio);
void Rotate_Point(Mat &src, Point2f p1, Point2f &res, double theta);



#endif //TEST_COMPONENTS_H
