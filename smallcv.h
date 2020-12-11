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

    class Point2i
    {
    public:
        Point2i() :x(0), y(0) {}
        Point2i(int _x, int _y) : x(_x), y(_y) {}
        Point2i(const Point2i& p) : x(p.x), y(p.y) {}
        Point2i& operator=(const Point2i& p) {
            x = p.x;
            y = p.y;
            return *this;
        }
    public:
        int x;
        int y;
    };

    class Point2f 
    {
    public:
        Point2f():x(0), y(0){}
        Point2f(float _x, float _y): x(_x), y(_y){}
        Point2f(const Point2f& p): x(p.x), y(p.y) {}
        Point2f& operator=(const Point2f& p) {
            x = p.x;
            y = p.y;
            return *this;
        }
    public:
        float x;
        float y;
    };

    class Scalar
    {
    public:
        Scalar() :v0(0), v1(0), v2(0){}
        Scalar(int _v0, int _v1, int _v2): v0(_v0), v1(_v1), v2(_v2){}
        Scalar(const Scalar& s): v0(s.v0), v1(s.v1), v2(s.v2){}
        Scalar& operator=(const Scalar& s) {
            v0 = s.v0;
            v1 = s.v1;
            v2 = s.v2;
            return *this;
        }
    private:
        int v0;
        int v1;
        int v2;
    };

    Mat imread(const char* image_path);
    Mat imread(const std::string& image_path);
    void imwrite(const char* filename, const sv::Mat& image);
    void imwrite(const std::string& save_path, const sv::Mat& image);

    void rgb_bgr_swap_inplace(sv::Mat& image);

    void line(Mat& image, Point2i pt1, Point2i pt2, const Scalar& color, int thickness = 1);
    void line(Mat& image, Point2f pt1, Point2f pt2, const Scalar& color, int thickness = 1);

    void circle(Mat& image, Point2i center, int radius, const Scalar& color, int thickness = 1);
    void circle(Mat& image, Point2f center, int radius, const Scalar& color, int thickness = 1);

    template<typename _Tp> static inline
    Rect_<_Tp> operator& (const Rect_<_Tp>& a, const Rect_<_Tp>& b)
    {
        Rect_<_Tp> c = a;
        return c &= b;
    }
 
}


#endif