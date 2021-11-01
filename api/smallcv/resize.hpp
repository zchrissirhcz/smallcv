#pragma once

#include "smallcv/mat.hpp"

namespace cv
{
    enum class Interpolation
    {
        NEAREST = 0,
        LINEAR = 1,
        AREA = 2,
        CUBIC = 3
    };

    void resize(Mat src, Mat dst, Size dsize, double fx = 0, double fy = 0, Interpolation method = Interpolation::LINEAR);

} // namespace cv