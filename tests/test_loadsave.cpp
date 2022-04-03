#include "cv_helper.hpp"
#include "gtest/gtest.h"

TEST(loadsave, bgr)
{
    cv::Size size(256, 256);
    cv::Mat bgr(size, CV_8UC3);
    for (int i = 0; i < size.height; i++)
    {
        for (int j = 0; j < size.width; j++)
        {
            bgr.ptr(i, j)[0] = i;
            bgr.ptr(i, j)[1] = j;
            bgr.ptr(i, j)[2] = (i + j) % 256;
        }
    }
    
    std::string name = "bgr.png";
    cv::imwrite(name, bgr);
    cv::Mat bgr0 = cv::imread(name);
    EXPECT_TRUE(almostEqual(bgr, bgr0, 0));
}

TEST(loadsave, gray)
{
    cv::Size size(256, 256);
    cv::Mat gray(size, CV_8UC1);
    for (int i = 0; i < size.height; i++)
    {
        for (int j = 0; j < size.width; j++)
        {
            gray.ptr(i, j)[0] = (i + j) % 256;
        }
    }

    std::string name = "gray.png";
    cv::imwrite(name, gray);
    cv::Mat gray0 = cv::imread(name);
    EXPECT_TRUE(almostEqual(gray, gray0, 2));
    cv::imwrite("gray0.png", gray0);
}