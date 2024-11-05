//
// Created by yijin on 2024-11-04 at 14:26:58.
//
#ifndef OPENCV_APP_H
#define OPENCV_APP_H
#include <opencv2/opencv.hpp>
#include <iostream>
#include <cmath>

using namespace std;
using namespace cv;
class OpencvApp {
public:
    static void loadImg();
    static void test();
    static void printPiexl(cv::Mat image);
    static void printTev(cv::Mat image);
    static void printRgb256(cv::Mat image);
};

#endif // OPENCV_APP_H
