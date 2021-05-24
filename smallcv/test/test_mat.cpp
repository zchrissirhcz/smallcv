#include "smallcv/api/mat.hpp"

int main() {
    // test memory leak/double free
    {
        unsigned char* data = new unsigned char[300];
        cv::Size size;
        size.height = 10;
        size.width = 10;
        cv::Mat mat(size, CV_8UC3, data);

        delete[] data;
    }

    {
        cv::Size size;
        size.height = 10;
        size.width = 10;
        cv::Mat mat(size, CV_8UC3);
    }

    return 0;
}
