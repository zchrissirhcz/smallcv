#include "smallcv/api/tensor.hpp"


namespace sv {

size_t Tensor::get_height() const
{
    return height;
}

size_t Tensor::get_width() const
{
    return width;
}

size_t Tensor::get_channels() const
{
    return channels;
}


Tensor::Tensor(Shape3d shape):
    height(shape.height), width(shape.width), channels(shape.channels),
    data(nullptr)
{
    size_t size = height * width * channels;
    data.reset(new uchar[size], [](uchar* p) {
        delete[] p;
    });
}

Tensor::Tensor(Shape3d shape, uchar* _data)
    : height(shape.height), width(shape.width), channels(shape.channels)
{
    data.reset(_data, [](uchar* p) { } );
}

Tensor::~Tensor()
{
    data.reset();
}

bool Tensor::empty() const
{
    return data == nullptr;
}

}