#ifndef SMALLCV_H
#define SMALLCV_H


#include <stdio.h>
#include <string>
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
            bool empty() const;
            Mat clone() const;
            

        // public data members
        public:
            std::shared_ptr<uchar> data;

        // private data members
        private:
            size_t height;
            size_t width;
            size_t channels;
    };

    template<typename _Tp> class Rect_
    {
    public:
        typedef _Tp value_type;

        Rect_();
        Rect_(_Tp _x, _Tp _y, _Tp _width, _Tp _height);
        Rect_(const Rect_& r);
        Rect_(Rect_&& r) noexcept;

        Rect_& operator=(const Rect_& r);
        Rect_& operator=(Rect_&& r) noexcept;

        _Tp area() const;

        _Tp x; // top-left, x coordinate
        _Tp y; // top-left, y coordinate
        _Tp width; // rect width
        _Tp height; // rect height
    };

    Mat imread(const char* image_path);
    Mat imread(const std::string& image_path);
    void imwrite(const char* filename, const sv::Mat& image);
    void imwrite(const std::string& save_path, const sv::Mat& image);

    template<typename _Tp> static inline
    Rect_<_Tp> operator& (const Rect_<_Tp>& a, const Rect_<_Tp>& b)
    {
        Rect_<_Tp> c = a;
        return c &= b;
    }

    inline Mat Mat::clone() const
    {
        Mat m;
        copyTo(m);
        return m;
    }
 
}


#endif