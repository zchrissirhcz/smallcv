#include "smallcv.hpp"


#include <assert.h>
#include <cmath>
#include <limits>

#define CV_DbgAssert assert

namespace cv {

void Mat::init(int _rows, int _cols, int type)
{
    rows = _rows;
    cols = _cols;
    data_type_ = static_cast<Mat::DataType>(type / 10);
    channels_ = type % 10;
    step = cols * channels_;

    size_t buf_size = rows * cols * channels_;
    data_.reset(new uchar[buf_size], [](uchar* p) {
        delete[] p;
    });
    
    data = data_.get();
}

void Mat::create(Size size, int type)
{
    if (rows == size.height && cols == size.width && data!=nullptr) return;
    init(size.height, size.width, type);
}

int Mat::type() const
{
    return data_type_ * 10 + channels_;
}

int Mat::depth() const
{
    return data_type_ * 10 + 1;
}

Mat::Mat()
{
    init(0, 0, kEmpty);
}

Mat::Mat(int _rows, int _cols, int type)
{
    init(_rows, _cols, type);
}

Mat::Mat(int _rows, int _cols, int type, void* _data)
{
    init(_rows, _cols, type);

    data_.reset((uchar*)_data, [](uchar* p) { });
    data = data_.get();
}

Mat::Mat(Size size, int type)
{
    init(size.height, size.width, type);
}

Mat::Mat(Size size, int type, void* _data)
    : rows(size.height), cols(size.width)
{
    init(size.height, size.width, type);

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

uchar* Mat::ptr(int y)
{
    //CV_DbgAssert( y == 0 || (data && dims >= 1 && (unsigned)y < (unsigned)size.p[0]) );
    return data + step * y;
}



uchar* Mat::ptr(int i0, int i1)
{
    //CV_DbgAssert(dims >= 2);
    CV_DbgAssert(data);
    // CV_DbgAssert((unsigned)i0 < (unsigned)size.p[0]);
    // CV_DbgAssert((unsigned)i1 < (unsigned)size.p[1]);
    //return data + i0 * step.p[0] + i1 * step.p[1];
    return data + i0 * step + i1 * channels_;
}

const uchar* Mat::ptr(int i0, int i1) const
{
    //CV_DbgAssert(dims >= 2);
    CV_DbgAssert(data);
    // CV_DbgAssert((unsigned)i0 < (unsigned)size.p[0]);
    // CV_DbgAssert((unsigned)i1 < (unsigned)size.p[1]);
    //return data + i0 * step.p[0] + i1 * step.p[1];
    return data + i0 * step + i1 * channels_;
}

}

