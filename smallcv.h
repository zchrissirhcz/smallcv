#ifndef SMALLCV_H
#define SMALLCV_H


#include <stdio.h>
#include <memory>


typedef unsigned char uchar;

struct Shape2d {
    size_t height;
    size_t width;
};

struct Shape3d {
    size_t height;
    size_t width;
    size_t channels;
};

namespace sv {
    class Mat {
        // public member functions
        public:
            Mat(Shape3d shape);
            ~Mat();
            size_t get_height() const { return height; }
            size_t get_width() const { return width; }
            size_t get_channels() const { return channels; }

        // public data members
        public:
            std::shared_ptr<uchar> data;

        // private data members
        private:
            size_t height;
            size_t width;
            size_t channels;
    };

    Mat imread(const char* image_path);
    Mat imread(const std::string& image_path);
    void imwrite(const char* filename, const sv::Mat& image);
    void imwrite(const std::string& save_path, const sv::Mat& image);

}


#endif