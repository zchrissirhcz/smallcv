#ifndef SMALLCV_MAT_HPP
#define SMALLCV_MAT_HPP


#include <stdio.h>
#include <string>
#include <memory>


typedef unsigned char uchar;

enum {
    CV_8UC1 =  1*10 + 1,
    CV_8UC3 =  1*10 + 3,
    CV_32FC1 = 2*10 + 1,
    CV_32FC3 = 2*10 + 3,
    CV_64FC1 = 3*10 + 1,
    CV_64FC3 = 3*10 + 3
};

namespace cv {

struct Size
{
    int height;
    int width;
};

class Mat
{
public:
    Mat();
    Mat(int rows, int cols, int type);
    Mat(int rows, int cols, int type, void* data);
    Mat(Size size, int type);
    Mat(Size size, int type, void* data);
    Mat(const Mat& mat);
    Mat& operator=(const Mat& mat);
    ~Mat();

public:
    int rows;
    int cols;
    uchar* data;
    int channels() const;
    bool empty() const;

private:
    std::shared_ptr<uchar> data_;

    void addref();
    void release();

    int channels_;

    enum DataType {
        kEmpty = 0,
        kUchar = 1,
        kFloat = 2,
        kDouble = 3
    };

    DataType data_type_;
};

template<class T>
class Point_
{
public:
    Point_() :x(0), y(0) {}
    Point_(T _x, T _y) : x(_x), y(_y) {}
    Point_(const Point_& p) : x(p.x), y(p.y) {}
    Point_& operator=(const Point_& p) {
        if (this!=&p) {
            x = p.x;
            y = p.y;
        }
        return *this;
    }
public:
    T x;
    T y;
};

typedef Point_<int> Point2i;
typedef Point_<float> Point2f;
typedef Point2i Point;

template<typename T>
class Scalar_
{
public:
    Scalar_() {
        val[0] = val[1] = val[2] = val[3] = 0;
    }
    Scalar_(T v0, T v1, T v2=0, T v3=0) {
        val[0] = v0;
        val[1] = v1;
        val[2] = v2;
        val[3] = v3;
    }
    Scalar_(T v0) {
        val[0] = v0;
        val[1] = val[2] = val[3] = 0;
    }

    Scalar_(const Scalar_& s) {
        val[0] = s.val[0];
        val[1] = s.val[1];
        val[2] = s.val[2];
        val[3] = s.val[3];
    }
    Scalar_& operator=(const Scalar_& s) {
        if (this!=&s) {
            val[0] = s.val[0];
            val[1] = s.val[1];
            val[2] = s.val[2];
            val[3] = s.val[3];
        }
        return *this;
    }

public:
    T val[4];
};

typedef Scalar_<double> Scalar;


template<class T> class Rect_
{
public:
    Rect_(): x(0), y(0), width(0), height(0) {}
    Rect_(T _x, T _y, T _width, T _height): x(_x), y(_y), width(_width), height(_height) {}
    Rect_(const Rect_& r): x(r.x), y(r.y), width(r.width), height(r.height) {}
    Rect_& operator=(const Rect_& r) {
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

public:
    T x;      // top-left, x coordinate
    T y;      // top-left, y coordinate
    T width;  // rect width
    T height; // rect height
};

typedef Rect_<int> Rect2i;
typedef Rect_<float> Rect2f;
typedef Rect_<double> Rect2d;
typedef Rect2i Rect;


Mat imread(const char* image_path);
Mat imread(const std::string& image_path);
void imwrite(const char* filename, const cv::Mat& image);
void imwrite(const std::string& save_path, const cv::Mat& image);


enum {
    COLOR_BGR2GRAY = 0,
    COLOR_BGR2RGB = 1,
    COLOR_RGB2BGR = 1
};

void cvtColor(Mat src, Mat dst, int flag);
 
} // namespace cv


#endif // SMALLCV_MAT_HPP
