#pragma once

#include "smallcv/mat.hpp"

namespace cv {

void imshow(const char* winname, const Mat& im);

void waitKey(int milli_secs);

} // namespace cv


