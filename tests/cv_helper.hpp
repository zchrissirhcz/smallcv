#pragma once

//#include <opencv2/opencv.hpp> // use opencv
#include "smallcv.hpp" // use smallcv
#include <iostream>
#include <stdint.h>

//--------------------------------------------------------------------------------
// 1. 打印 Mat 数据类型(包含通道)
//--------------------------------------------------------------------------------
// https://stackoverflow.com/questions/10167534/how-to-find-out-what-type-of-a-mat-object-is-with-mattype-in-opencv
// NOTE: There is `cv::typeToString()` in OpenCV since 3.4.2 and 4.0.0
// May also search the table:
// +--------+----+----+----+----+------+------+------+------+
// |        | C1 | C2 | C3 | C4 | C(5) | C(6) | C(7) | C(8) |
// +--------+----+----+----+----+------+------+------+------+
// | CV_8U  |  0 |  8 | 16 | 24 |   32 |   40 |   48 |   56 |
// | CV_8S  |  1 |  9 | 17 | 25 |   33 |   41 |   49 |   57 |
// | CV_16U |  2 | 10 | 18 | 26 |   34 |   42 |   50 |   58 |
// | CV_16S |  3 | 11 | 19 | 27 |   35 |   43 |   51 |   59 |
// | CV_32S |  4 | 12 | 20 | 28 |   36 |   44 |   52 |   60 |
// | CV_32F |  5 | 13 | 21 | 29 |   37 |   45 |   53 |   61 |
// | CV_64F |  6 | 14 | 22 | 30 |   38 |   46 |   54 |   62 |
// +--------+----+----+----+----+------+------+------+------+
static std::string mat_type_to_string(const cv::Mat& mat)
{
    std::string r;
    const int depth = mat.depth();
    const int cn = mat.channels();

    switch ( depth ) {
        case CV_8U:  r = "8U"; break;
        case CV_8S:  r = "8S"; break;
        case CV_16U: r = "16U"; break;
        case CV_16S: r = "16S"; break;
        case CV_32S: r = "32S"; break;
        case CV_32F: r = "32F"; break;
        case CV_64F: r = "64F"; break;
        case CV_16F: r = "16F"; break;
        default:     r = "User"; break;
    }

    r += "C";
    r += (cn+'0');

    return r;
}

template<typename T>
static
bool almostEqual(const cv::Mat& expected, const cv::Mat& actual, double eps)
{
    if ( (expected.rows != actual.rows) || (expected.cols != actual.cols) || (expected.channels() != actual.channels()) || (expected.type() != actual.type()) )
    {
        return false;
    }
    const int height = expected.rows;
    const int width = expected.cols;
    const int channels = expected.channels();
    const int row_elems = width * channels;
    const int expected_line_elements = expected.step1();
    const int actual_line_elements = actual.step1();

    T* expected_data = (T*)expected.data;
    T* actual_data = (T*)actual.data;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            for (int k = 0; k < channels; k++)
            {
                //T diff = expected.ptr<T>(i, j) - actual.ptr<T>(i, j);
                int expected_idx = i * expected_line_elements + j * channels + k;
                int actual_idx = i * actual_line_elements + j * channels + k;
                const T expected_elem = expected_data[expected_idx];
                const T actual_elem = actual_data[actual_idx];
                T diff;
                if (expected_elem > actual_elem) {
                    diff = expected_elem - actual_elem;
                }
                else {
                    diff = actual_elem - expected_elem;
                }

                if ( (double)diff > eps)
                {
                    std::cout << "i=" << i << ", j=" << j << ", k=" << k
                              //<< ", expected=" << expected_data << ", actual=" << actual << ", diff=" << diff
                              << ", expected_idx=" << expected_idx << ", actual_idx=" << actual_idx
                              << ", expected_elem=" << (int)expected_data[expected_idx] << ", actual_elem=" << (int)actual_data[actual_idx]
                              << ", diff=" << (int)diff
                              << std::endl;

                    // printf("i=%d, j=%d, k=%d， expected=%lf, actual=%lf, diff=%lf\n",
                    //     i, j, k,
                    //     expected_data[expected_idx],
                    //     actual_data[actual_idx],
                    //     diff
                    // );
                    return false;
                }
            }
        }
    }
    return true;
}

static bool almostEqual(const cv::Mat& expected, const cv::Mat& actual, double eps=0)
{
    if ( (expected.rows != actual.rows) || (expected.cols != actual.cols) )
    {
        fprintf(stderr, "dims not match: expected(rows=%d, cols=%d) vs actual(rows=%d, cols=%d)\n",
            expected.rows, expected.cols,
            actual.rows, actual.cols
        );
        return false;
    }
    if ( expected.channels() != actual.channels() )
    {
        fprintf(stderr, "channels not match: expected(%d) vs actual(%d)\n",
            expected.channels(),
            actual.channels()
        );
        return false;
    }
    if ( expected.type() != actual.type() )
    {
        fprintf(stderr, "types not match: expected(%s) vs actual(%s)\n",
            mat_type_to_string(expected).c_str(),
            mat_type_to_string(actual).c_str()
        );
        return false;
    }

    int depth = expected.depth();
    switch ( depth ) {
        case CV_8U:  return almostEqual<uint8_t>(expected, actual, eps);
        case CV_8S:  return almostEqual<int8_t>(expected, actual, eps);
        case CV_16U: return almostEqual<uint16_t>(expected, actual, eps);
        case CV_16S: return almostEqual<int16_t>(expected, actual, eps);
        case CV_32S: return almostEqual<int>(expected, actual, eps);
        case CV_32F: return almostEqual<float>(expected, actual, eps);
        case CV_64F: return almostEqual<double>(expected, actual, eps);
        case CV_16F: fprintf(stderr, "not implemented\n"); break;
        default:     fprintf(stderr, "not implemented\n"); break;
    }

    return false;
}

