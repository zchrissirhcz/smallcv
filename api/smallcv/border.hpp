#pragma once

#include "smallcv/mat.hpp"
#include <vector>

namespace cv
{
    void findContours(cv::Mat image, std::vector<std::vector<cv::Point>>& contours,
        int mode, int method, cv::Point offset=cv::Point());

    cv::Rect maskBoundingRect(const cv::Mat& image);
}
