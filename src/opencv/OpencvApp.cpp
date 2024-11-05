//
// Created by yijin on 2024-11-04 at 14:26:58.
//
#include "OpencvApp.h"

void OpencvApp::loadImg() {
}

void OpencvApp::test() {
    cv::Mat image = cv::imread("2.jpg");
    if (image.empty()) {
        std::cout << "Could not open or find the image" << std::endl;
        return;
    }
    // printTev(image);
    printPiexl(image);
    // printRgb256(resizedImage);
}

void OpencvApp::printPiexl(cv::Mat image) {
    // 缩小图片至原来的1/8
    Mat resizedImage;
    double scaleFactor = 1.0 / 8;
    resize(image, resizedImage, Size(), image.cols / 8, scaleFactor, INTER_AREA);
    // 遍历每个像素并打印其值
    for (int y = 0; y < resizedImage.rows; y++) {
        for (int x = 0; x < resizedImage.cols; x++) {
            const auto& pixel = resizedImage.at<cv::Vec3b>(y, x); // 访问像素
            // 获取 RGB 值并归一化到 0-255 之间
            int r = static_cast<int>(pixel[2]); // Red
            int g = static_cast<int>(pixel[1]); // Green
            int b = static_cast<int>(pixel[0]); // Blue
            // 打印字符 "1"，并设置前景色
            std::cout << "\033[38;2;" << r << ";" << g << ";" << b << "m"
                      << "1"
                      << "\033[0m"; // 重置颜色
        }
        std::cout << std::endl;     // 换行到下一行
    }
}

void OpencvApp::printTev(cv::Mat image) {
    // 将彩色图像转换为灰度图像
    Mat grayImage;
    cvtColor(image, grayImage, COLOR_BGR2GRAY);
    // 创建一个 Mat 对象来存储二值图像
    Mat binaryImage;
    // 设置阈值，类型为THRESH_BINARY，不进行最大值设置（即THRESH_TRUNC），用于二值化
    double thresholdValue = 128; // 可以根据需要调整这个值
    threshold(grayImage, binaryImage, thresholdValue, 255, THRESH_BINARY);
    // 显示二值图像
    imshow("Binary Image", binaryImage);
    waitKey(0); // 修正了这里
    // 打印二值图像的前10个像素值（仅作为示例）
    cout << "First 10 pixel values of the binary image:" << endl;
    for (int i = 0; i < min(10, binaryImage.cols); i++) {
        cout << binaryImage.at<uchar>(0, i) << " ";
    }
    cout << endl;
}

void OpencvApp::printRgb256(cv::Mat image) {
    for (int y = 0; y < image.rows; ++y) {
        for (int x = 0; x < image.cols; ++x) {
            cv::Vec3b pixel = image.at<cv::Vec3b>(y, x);
            // 获取 RGB 值
            int r = pixel[2]; // OpenCV 使用 BGR 格式
            int g = pixel[1];
            int b = pixel[0];
            // 将 RGB 值映射到 ANSI 256 色
            int ansiColor = 16 + (36 * (r / 51)) + (6 * (g / 51)) + (b / 51);
            // 打印彩色方块
            std::cout << "\033[38;5;" << ansiColor << "m"
                      << "  ";
        }
        std::cout << "\033[0m" << std::endl; // 重置颜色
    }
}

