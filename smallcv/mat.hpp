#ifndef SMALLCV_MAT_HPP
#define SMALLCV_MAT_HPP


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

    class RectI {
    public:
        RectI(): x(0), y(0), width(0), height(0) {}
        RectI(int _x, int _y, int _width, int _height):
            x(_x), y(_y), width(_width), height(_height) {}
        RectI(const RectI& r):
            x(r.x), y(r.y), width(r.width), height(r.height) {}
        RectI& operator=(const RectI& r) {
            if (this!=&r) {
                x = r.x;
                y = r.y;
                width = r.width;
                height = r.height;
            }
            return *this;
        }
        int area() const {
            int result = width * height;
            return result;
        }
        int x2() const { // (inclusive) end point's x coordinate
            return x + width - 1;
        }
        int y2() const { // (inclusive) end point's y coordinate
            return y + height - 1;
        }
    public:
        int x;      // top-left, x coordinate
        int y;      // top-left, y coordinate
        int width;  // rect width
        int height; // rect height
    };

    class RectF {
    public:
        RectF(): x(0.f), y(0.f), width(0.f), height(0.f) {}
        RectF(float _x, float _y, float _width, float _height):
            x(_x), y(_y), width(_width), height(_height) {}
        RectF(const RectF& r):
            x(r.x), y(r.y), width(r.width), height(r.height) {}
        RectF& operator=(const RectF& r) {
            if (this!=&r) {
                x = r.x;
                y = r.y;
                width = r.width;
                height = r.height;
            }
            return *this;
        }
        float area() const {
            float result = width * height;
            return result;
        }
        float x2() const { // (inclusive) end point's x coordinate
            return x + width - 1;
        }
        float y2() const { // (inclusive) end point's y coordinate
            return y + height - 1;
        }
    public:
        float x;        // top-left, x coordinate
        float y;        // top-left, y coordinate
        float width;    // rect width
        float height;   // rect height
    };

    class Point2I
    {
    public:
        Point2I() :x(0), y(0) {}
        Point2I(int _x, int _y) : x(_x), y(_y) {}
        Point2I(const Point2I& p) : x(p.x), y(p.y) {}
        Point2I& operator=(const Point2I& p) {
            if (this!=&p) {
                x = p.x;
                y = p.y;
            }
            return *this;
        }
    public:
        int x;
        int y;
    };

    class Point2F
    {
    public:
        Point2F():x(0), y(0){}
        Point2F(float _x, float _y): x(_x), y(_y){}
        Point2F(const Point2F& p): x(p.x), y(p.y) {}
        Point2F& operator=(const Point2F& p) {
            if (this!=&p) {
                x = p.x;
                y = p.y;
            }
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
            if (this!=&s) {
                v0 = s.v0;
                v1 = s.v1;
                v2 = s.v2;
            }
            return *this;
        }
        int get_v0() const { return v0; }
        int get_v1() const { return v1; }
        int get_v2() const { return v2; }
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

    // template<typename _Tp> static inline
    // Rect_<_Tp> operator& (const Rect_<_Tp>& a, const Rect_<_Tp>& b)
    // {
    //     Rect_<_Tp> c = a;
    //     return c &= b;
    // }
 
}


#endif
