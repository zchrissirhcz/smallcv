#ifndef SMALLCV_IMSHOW_HPP
#define SMALLCV_IMSHOW_HPP

#include "mat.hpp"

namespace sv {

void imshow(const char* winname, const Mat& im);

void waitKey(int milli_secs);

} // namespace fc

#endif
