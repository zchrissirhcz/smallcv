#include "cv_helper.hpp"
#include "gtest/gtest.h"

TEST(size, multiply)
{
    cv::Size size(1, 2);
    for (int scale: {1, 2, 3})
    {
        cv::Size size2 = size * scale;
        ASSERT_EQ(size2.width, size.width * scale);
        ASSERT_EQ(size2.height, size.height * scale);
    }
    
    for (float scale: {1.5, 2.0, 2.5, 3.0, 3.5})
    {
        cv::Size size2 = size * scale;
        ASSERT_EQ(size2.width, (int)(size.width * scale));
        ASSERT_EQ(size2.height, (int)(size.height * scale));
    }

    const cv::Size size3(1, 2);
    cv::Size size4 = size3 * 3;
    for (int scale: {1, 2, 3})
    {
        cv::Size size2 = size4 * scale;
        ASSERT_EQ(size2.width, size4.width * scale);
        ASSERT_EQ(size2.height, size4.height * scale);
    }
}