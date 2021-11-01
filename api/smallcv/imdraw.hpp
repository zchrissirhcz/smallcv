#pragma once

#include "smallcv/mat.hpp"

namespace cv {

    void line(Mat& image, Point pt1, Point pt2, const Scalar& color, int thickness = 1);
    void line(Mat& image, Point2f pt1, Point2f pt2, const Scalar& color, int thickness = 1);

    void circle(Mat& image, Point center, int radius, const Scalar& color, int thickness = 1);
    void circle(Mat& image, Point2f center, int radius, const Scalar& color, int thickness = 1);

    void rectangle(Mat& im, const Rect& rect, const Scalar& color, int thickness=1);

}

