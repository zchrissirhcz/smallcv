#ifndef SMALLCV_IMDRAW_H
#define SMALLCV_IMDRAW_H

#include "mat.hpp"

namespace sv {

void rectangle(Mat& im, const RectI& rect, const Scalar color, int thickness=1);

}

#endif
