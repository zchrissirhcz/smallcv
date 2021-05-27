#ifndef SMALLCV_IMSHOW_HPP
#define SMALLCV_IMSHOW_HPP

#include "mat.hpp"

namespace cv {

void imshow(const char* winname, const Mat& im);

void waitKey(int milli_secs);

} // namespace cv

#endif // SMALLCV_IMSHOW_HPP
