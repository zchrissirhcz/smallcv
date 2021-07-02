#ifndef SMALLCV_IMAGEIO_HPP
#define SMALLCV_IMAGEIO_HPP

#include "smallcv/mat.hpp"

namespace cv {

    Mat imread(const char* image_path);
    Mat imread(const std::string& image_path);
    void imwrite(const char* filename, const cv::Mat& image);
    void imwrite(const std::string& save_path, const cv::Mat& image);

} // tv

#endif // SMALLCV_IMAGEIO_HPP