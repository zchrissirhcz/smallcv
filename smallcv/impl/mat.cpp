#include "smallcv/api/mat.hpp"

namespace sv {

size_t Mat::get_height() const
{
    return height;
}

size_t Mat::get_width() const
{
    return width;
}

size_t Mat::get_channels() const
{
    return channels;
}


Mat::Mat(Shape3d shape):
    height(shape.height), width(shape.width), channels(shape.channels),
    data(nullptr)
{
    size_t size = height * width * channels;
    data.reset(new uchar[size], [](uchar* p) {
        delete[] p;
    });
}

Mat::Mat(Shape3d shape, uchar* _data)
    : height(shape.height), width(shape.width), channels(shape.channels)
{
    data.reset(_data, [](uchar* p) { } );
}

Mat::~Mat()
{
    data.reset();
}

bool Mat::empty() const
{
    return data == nullptr;
}

}