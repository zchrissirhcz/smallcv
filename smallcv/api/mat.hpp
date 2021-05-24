#ifndef SMALLCV_API_MAT_HPP
#define SMALLCV_API_MAT_HPP

typedef unsigned char uchar;
#include <memory>


enum {
    CV_8UC1 =  1*10 + 1,
    CV_8UC3 =  1*10 + 3,
    CV_32FC1 = 2*10 + 1,
    CV_32FC3 = 2*10 + 3,
    CV_64FC1 = 3*10 + 1,
    CV_64FC3 = 3*10 + 3
};

namespace cv
{

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


Mat imread(const char* image_path);
Mat imread(const std::string& image_path);
void imwrite(const char* filename, const cv::Mat& image);
void imwrite(const std::string& save_path, const cv::Mat& image);


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
    int x;
    int y;
};

typedef Point_<int> Point2i;
typedef Point_<float> Point2f;
typedef Point2i Point;

enum {
    COLOR_BGR2GRAY = 0,
    COLOR_BGR2RGB = 1,
    COLOR_RGB2BGR = 1
};

void cvtColor(Mat src, Mat dst, int flag);

}


#endif // SMALLCV_API_MAT_HPP
