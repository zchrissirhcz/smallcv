#ifndef SMALLCV_API_IMSHOW_HPP
#define SMALLCV_API_IMSHOW_HPP

#include "mat.hpp"

namespace sv {

void imshow(const char* winname, const Mat& im);

void waitKey(int milli_secs);

} // namespace sv

#endif // SMALLCV_API_IMSHOW_HPP
