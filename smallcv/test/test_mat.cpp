#include "smallcv/api/mat.hpp"

int main() {
    // test memory leak/double free
    {
        unsigned char* data = new unsigned char[300];
        sv::Shape3d shape;
        shape.height = 10;
        shape.width = 10;
        shape.channels = 3;
        sv::Mat mat(shape, data);

        delete[] data;
    }

    {
        sv::Shape3d shape;
        shape.height = 10;
        shape.width = 10;
        shape.channels = 3;
        sv::Mat mat(shape);
    }

    return 0;
}
