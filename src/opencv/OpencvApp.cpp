//
// Created by yijin on 2024-11-04 at 14:26:58.
//
#include "OpencvApp.h"
#include <opencv2/opencv.hpp>

void OpencvApp::loadImg() {
}

void OpencvApp::test() {
    const cv::Mat image = cv::imread("2.jpg", cv::IMREAD_GRAYSCALE);
    if (image.empty()) {
        std::cout << "Could not open or find the image" << std::endl;
        return;
    }
    // imshow("display image", image);
    // cv::waitKey(0);
    std::cout << "Image Size: " << image.cols << " x " << image.rows << std::endl;
    // 遍历每个像素并打印其值
    // for (int y = 0; y < image.rows; y++) {
    //     for (int x = 0; x < image.cols; x++) {
    //         const auto& pixel = image.at<cv::Vec3b>(y, x); // 访问像素
    //         std::cout << "Pixel at (" << x << ", " << y << "): "
    //                   << "B: " << static_cast<int>(pixel[0]) << ", "
    //                   << "G: " << static_cast<int>(pixel[1]) << ", "
    //                   << "R: " << static_cast<int>(pixel[2]) << std::endl;
    //     }
    // }
    // 遍历每个像素并打印 1 或 0
    // 缩小图像
    cv::Mat resizedImage;
    resize(image, resizedImage, cv::Size(image.cols / 8, image.rows / 8)); // 将图像缩小为原来的 1/8
    for (int y = 0; y < resizedImage.rows; y++) {
        for (int x = 0; x < resizedImage.cols; x++) {
            if (const uchar pixel = resizedImage.at<uchar>(y, x); pixel < 128) {
                std::cout << "0 ";
            } else {
                std::cout << "1 ";
            } 
        }
        std::cout << std::endl;   
    }
}

