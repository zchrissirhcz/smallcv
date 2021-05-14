#include "smallcv/api/mat.hpp"

namespace sv {

Scalar::Scalar()
    :v0(0), v1(0), v2(0)
{

}

Scalar::Scalar(int _v0, int _v1, int _v2)
    :v0(_v0), v1(_v1), v2(_v2)
{

}

Scalar::Scalar(const Scalar& s)
    :v0(s.v0), v1(s.v1), v2(s.v2)
{

}

Scalar& Scalar::operator=(const Scalar& s) {
    if (this!=&s) {
        v0 = s.v0;
        v1 = s.v1;
        v2 = s.v2;
    }
    return *this;
}

int Scalar::get_v0() const 
{ 
    return v0;
}

int Scalar::get_v1() const
{
    return v1;
}

int Scalar::get_v2() const
{
    return v2; 
}

}