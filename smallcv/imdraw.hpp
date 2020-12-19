#ifndef SMALLCV_IMDRAW_H
#define SMALLCV_IMDRAW_H

#include "mat.hpp"

namespace sv {

    void line(Mat& image, Point2I pt1, Point2I pt2, const Scalar& color, int thickness = 1);
    void line(Mat& image, Point2F pt1, Point2F pt2, const Scalar& color, int thickness = 1);

    void circle(Mat& image, Point2I center, int radius, const Scalar& color, int thickness = 1);
    void circle(Mat& image, Point2F center, int radius, const Scalar& color, int thickness = 1);

    void rectangle(Mat& im, const RectI& rect, const Scalar color, int thickness=1);

}

#endif
