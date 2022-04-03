#pragma once

#include <stdio.h>
#include <string>
#include <memory>

//------------------------------------------------------------
// logging
//------------------------------------------------------------
#define SMALLCV_STDIO 1

#if SMALLCV_STDIO
#if SMALLCV_PLATFORM_API && __ANDROID_API__ >= 8
#include <android/log.h>
#define SMALLCV_LOGE(...) do { \
    fprintf(stderr, ##__VA_ARGS__); fprintf(stderr, "\n"); \
    __android_log_print(ANDROID_LOG_WARN, "smallcv", ##__VA_ARGS__); } while(0)
#else // SMALLCV_PLATFORM_API && __ANDROID_API__ >= 8
#include <stdio.h>
#define SMALLCV_LOGE(...) do { \
    fprintf(stderr, ##__VA_ARGS__); fprintf(stderr, "\n"); } while(0)
#endif // SMALLCV_PLATFORM_API && __ANDROID_API__ >= 8
#else
#define SMALLCV_LOGE(...)
#endif

//------------------------------------------------------------
// C types
//------------------------------------------------------------
typedef unsigned char uchar;
enum {
    CV_8UC1 =  1 * 10 + 1,
    CV_8UC2 =  1 * 10 + 2,
    CV_8UC3 =  1 * 10 + 3,
    CV_8UC4 =  1 * 10 + 4,
    CV_8U   =  CV_8UC1,
    
    CV_16UC1 = 2 * 10 + 1,
    CV_16U   = CV_16UC1,
    
    CV_32FC1 = 3 * 10 + 1,
    CV_32FC3 = 3 * 10 + 3,
    CV_32F   = CV_32FC1,

    CV_64FC1 = 4 * 10 + 1,
    CV_64FC3 = 4 * 10 + 3,
    CV_64F   = CV_64FC1
};


namespace cv {

//------------------------------------------------------------
// Mat
//------------------------------------------------------------

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
    int step;
    int channels() const;
    bool empty() const;
    Size size() const { Size size; size.height = rows; size.width = cols; return size; }
    void create(Size size, int type);
    void init(int rows, int cols, int type);
    int type() const;
    int depth() const;
    uchar* ptr(int i0=0);
    const uchar* ptr(int i0=0) const; // TODO
    uchar* ptr(int i0, int i1); // TODO
    const uchar* ptr(int i0, int i1) const; // TODO
    int step1() const; // TODO

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

//------------------------------------------------------------
// C++ Types
//------------------------------------------------------------
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

    const T operator[](const int i) const
    {
        return val[i];
    }

    T operator[](const int i)
    {
        return val[i];
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

class Range
{
public:
    int start;
    int end;

public:
    Range(int _start, int _end): start(_start), end(_end) {}
};

// TODO: saturate_cast
// TODO: parallel_for_

//------------------------------------------------------------
// image processing
//------------------------------------------------------------
enum {
    COLOR_BGR2GRAY = 0,
    COLOR_BGR2RGB = 1,
    COLOR_RGB2BGR = 1
};

enum class Interpolation
{
    NEAREST = 0,
    LINEAR = 1,
    AREA = 2,
    CUBIC = 3
};

void resize(const Mat& src, Mat& dst, Size dsize, Interpolation method = Interpolation::LINEAR);
void cvtColor(const Mat& src, Mat& dst, int flag);

void image_upside_down(const Mat& im, Mat& im_upsd);
void bgr2gray(const Mat& src, Mat& dst);
void rgb2gray(const Mat& src, Mat& dst);
void bgr2rgb_inplace(Mat& src);
void bgr2rgb(const Mat& src, Mat& dst);
void resize_linear(const Mat& src, Mat& dst, Size dsize);
void resize_nearest(const Mat& src, Mat& dst, Size dsize);

//------------------------------------------------------------
// imshow
//------------------------------------------------------------
#ifdef SMALLCV_IMSHOW
void imshow(const char* winname, const Mat& im);
void waitKey(int milli_secs);
#endif // SMALLCV_IMSHOW

// drawing
void line(Mat& image, Point pt1, Point pt2, const Scalar& color, int thickness = 1);
void line(Mat& image, Point2f pt1, Point2f pt2, const Scalar& color, int thickness = 1);

void circle(Mat& image, Point center, int radius, const Scalar& color, int thickness = 1);
void circle(Mat& image, Point2f center, int radius, const Scalar& color, int thickness = 1);

void rectangle(Mat& im, const Rect& rect, const Scalar& color, int thickness=1);
void putText(Mat& img, const std::string& text, Point org, int fontFace, double fontScale, Scalar color, int thickness = 1);


// resolve text bounding box size
void get_text_drawing_size(const char* text, int fontpixelsize, int* w, int* h);
// draw ascii printables and newline, the color RGBA is little-endian encoded
void draw_text_c1(unsigned char* pixels, int w, int h, const char* text, int x, int y, int fontpixelsize, unsigned int color);
void draw_text_c2(unsigned char* pixels, int w, int h, const char* text, int x, int y, int fontpixelsize, unsigned int color);
void draw_text_c3(unsigned char* pixels, int w, int h, const char* text, int x, int y, int fontpixelsize, unsigned int color);
void draw_text_c4(unsigned char* pixels, int w, int h, const char* text, int x, int y, int fontpixelsize, unsigned int color);
// draw ascii printables and newline with stride(bytes-per-row) parameter, the color RGBA is little-endian encoded
void draw_text_c1(unsigned char* pixels, int w, int h, int stride, const char* text, int x, int y, int fontpixelsize, unsigned int color);
void draw_text_c2(unsigned char* pixels, int w, int h, int stride, const char* text, int x, int y, int fontpixelsize, unsigned int color);
void draw_text_c3(unsigned char* pixels, int w, int h, int stride, const char* text, int x, int y, int fontpixelsize, unsigned int color);
void draw_text_c4(unsigned char* pixels, int w, int h, int stride, const char* text, int x, int y, int fontpixelsize, unsigned int color);
// draw ascii printables and newline, convenient wrapper for yuv420sp(nv21/nv12), the color YUV_ is little-endian encoded
void draw_text_yuv420sp(unsigned char* yuv420sp, int w, int h, const char* text, int x, int y, int fontpixelsize, unsigned int color);

//------------------------------------------------------------
// load/save
//------------------------------------------------------------
#ifdef SMALLCV_IMAGEIO
Mat imread(const char* image_path);
Mat imread(const std::string& image_path);
void imwrite(const char* filename, const cv::Mat& image);
void imwrite(const std::string& save_path, const cv::Mat& image);
#endif // SMALLCV_IMAGEIO

//------------------------------------------------------------
// timer
//------------------------------------------------------------
/// @brief get now timestamp in ms（毫秒）
double get_current_time();

/// @brief sleep for specified milliseconds (10^-3 second)
/// 等待指定的毫秒数
void sleep(int milli_secs);

} // namespace cv