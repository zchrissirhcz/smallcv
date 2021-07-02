#include "smallcv/smallcv.hpp"


#include <assert.h>
#include <cmath>
#include <limits>

namespace cv {

Mat::Mat()
    : rows(0), cols(0), channels_(0), data_type_(kEmpty), data(NULL)
{

}

Mat::Mat(int _rows, int _cols, int type)
    : rows(_rows), cols(_cols)
{
    data_type_ = static_cast<Mat::DataType>(type / 10);
    channels_ = type % 10;

    size_t buf_size = rows * cols * channels_;
    data_.reset(new uchar[buf_size], [](uchar* p) {
        delete[] p;
    });
    
    data = data_.get();
}

Mat::Mat(int _rows, int _cols, int type, void* _data)
    : rows(_rows), cols(_cols)
{
    data_type_ = static_cast<Mat::DataType>(type / 10);
    channels_ = type % 10;

    data_.reset((uchar*)_data, [](uchar* p) { });
    data = data_.get();
}

Mat::Mat(Size size, int type)
    : rows(size.height), cols(size.width)
{
    data_type_ = static_cast<Mat::DataType>(type / 10);
    channels_ = type % 10;

    size_t buf_size = rows * cols * channels_;
    data_.reset(new uchar[buf_size], [](uchar* p) {
        delete[] p;
    });
    data = data_.get();
}

Mat::Mat(Size size, int type, void* _data)
    : rows(size.height), cols(size.width)
{
    data_type_ = static_cast<Mat::DataType>(type / 10);
    channels_ = type % 10;

    data_.reset((uchar*)_data, [](uchar* p) { });
    data = data_.get();
}

Mat::Mat(const Mat& mat)
    : rows(mat.rows), cols(mat.cols), data_type_(mat.data_type_), channels_(mat.channels_), data_(mat.data_), data(mat.data)
{

}

Mat& Mat::operator=(const Mat& mat)
{
    if (this!=&mat) {
        data_ = mat.data_;
        data = mat.data;
        rows = mat.rows;
        cols = mat.cols;
        data_type_ = mat.data_type_;
        channels_ = mat.channels_;
    }
    return *this;
}

Mat::~Mat()
{

}

int Mat::channels() const
{
    return channels_;
}

bool Mat::empty() const
{
    if (rows == 0 || cols == 0) {
        return true;
    }
    return false;
}

    void cvtColor(Mat src, Mat dst, int flag)
    {
        if (flag==COLOR_BGR2RGB && src.data==dst.data) {
            Mat& image = src;
            size_t h = image.rows;
            size_t w = image.cols;
            size_t c = image.channels();
            assert(c == 3);
            uchar* data = image.data;
            for (size_t i = 0; i < h; i++) {
                for (size_t j = 0; j < w; j++) {
                    size_t idx = i * w * 3 + j * 3;
                    unsigned char t = data[idx];
                    data[idx] = data[idx + 2];
                    data[idx + 2] = t;
                }
            }
        } else {
            fprintf(stderr, "! error: not supported yet\n");
            exit(1);
        }
    }
}

