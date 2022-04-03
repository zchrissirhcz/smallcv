#include "smallcv.hpp"


#include <assert.h>
#include <cmath>
#include <limits>


#ifdef SMALLCV_IMAGEIO
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_STATIC
#define STBI_NO_PSD
#define STBI_NO_TGA
#define STBI_NO_GIF
#define STBI_NO_HDR
#define STBI_NO_PIC
#include "stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#define STB_IMAGE_WRITE_STATIC
#include "stb_image_write.h"
#endif // SMALLCV_IMAGEIO


#ifdef _WIN32
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#else // _WIN32
#include <sys/time.h>
#include <unistd.h>
#endif // _WIN32

#include <stddef.h>


#define CV_DbgAssert assert

namespace cv {

void Mat::init(int _rows, int _cols, int type)
{
    rows = _rows;
    cols = _cols;
    data_type_ = static_cast<Mat::DataType>(type / 10);
    channels_ = type % 10;
    step = cols * channels_;

    size_t buf_size = rows * cols * channels_;
    data_.reset(new uchar[buf_size], [](uchar* p) {
        delete[] p;
    });
    
    data = data_.get();
}

void Mat::create(Size size, int type)
{
    if (rows == size.height && cols == size.width && data!=nullptr) return;
    init(size.height, size.width, type);
}

int Mat::type() const
{
    return data_type_ * 10 + channels_;
}

int Mat::depth() const
{
    return data_type_ * 10 + 1;
}

Mat::Mat()
{
    init(0, 0, kEmpty);
}

Mat::Mat(int _rows, int _cols, int type)
{
    init(_rows, _cols, type);
}

Mat::Mat(int _rows, int _cols, int type, void* _data)
{
    init(_rows, _cols, type);

    data_.reset((uchar*)_data, [](uchar* p) { });
    data = data_.get();
}

Mat::Mat(Size size, int type)
{
    init(size.height, size.width, type);
}

Mat::Mat(Size size, int type, void* _data)
    : rows(size.height), cols(size.width)
{
    init(size.height, size.width, type);

    data_.reset((uchar*)_data, [](uchar* p) { });
    data = data_.get();
}

Mat::Mat(const Mat& mat)
    : rows(mat.rows), cols(mat.cols), data_type_(mat.data_type_), channels_(mat.channels_), data_(mat.data_), data(mat.data)
{

}

Mat& Mat::operator=(const Mat& mat)
{
    if (this!=&mat) {
        data_ = mat.data_;
        data = mat.data;
        rows = mat.rows;
        cols = mat.cols;
        data_type_ = mat.data_type_;
        channels_ = mat.channels_;
    }
    return *this;
}

Mat::~Mat()
{

}

int Mat::channels() const
{
    return channels_;
}

bool Mat::empty() const
{
    if (rows == 0 || cols == 0) {
        return true;
    }
    return false;
}

uchar* Mat::ptr(int y)
{
    //CV_DbgAssert( y == 0 || (data && dims >= 1 && (unsigned)y < (unsigned)size.p[0]) );
    return data + step * y;
}

const uchar* Mat::ptr(int y) const
{
    //CV_DbgAssert( y == 0 || (data && dims >= 1 && (unsigned)y < (unsigned)size.p[0]) );
    return data + step * y;
}


uchar* Mat::ptr(int i0, int i1)
{
    //CV_DbgAssert(dims >= 2);
    CV_DbgAssert(data);
    // CV_DbgAssert((unsigned)i0 < (unsigned)size.p[0]);
    // CV_DbgAssert((unsigned)i1 < (unsigned)size.p[1]);
    //return data + i0 * step.p[0] + i1 * step.p[1];
    return data + i0 * step + i1 * channels_;
}

const uchar* Mat::ptr(int i0, int i1) const
{
    //CV_DbgAssert(dims >= 2);
    CV_DbgAssert(data);
    // CV_DbgAssert((unsigned)i0 < (unsigned)size.p[0]);
    // CV_DbgAssert((unsigned)i1 < (unsigned)size.p[1]);
    //return data + i0 * step.p[0] + i1 * step.p[1];
    return data + i0 * step + i1 * channels_;
}

int Mat::step1() const
{
    return cols * channels_;
}

}





cv::Mat cv::imread(const char* image_path)
{
    // TODO: check if file image_path exist
    int height;
    int width;
    int channels;
    unsigned char* raw_data = stbi_load(image_path, &width, &height, &channels, 0);
    assert(raw_data != NULL);
    // TODO: detect 4-channel RGBA image
    Size size(width, height);
    Mat image(size, CV_8UC(channels));
    uchar* image_data = image.data;
    size_t buf_size = static_cast<size_t>(height * width * channels) * sizeof(unsigned char);
    memcpy(image_data, raw_data, buf_size);
    free(raw_data);
    if (channels == 3)
    {
        cvtColor(image, image, COLOR_RGB2BGR);
    }

    return image;
}

cv::Mat cv::imread(const std::string& image_path)
{
    return imread(image_path.c_str());
}

void cv::imwrite(const char* filename, const cv::Mat& image)
{
    if (strlen(filename) < 5) {
        fprintf(stderr, "filename too short\n");
        return;
    }
    const char* ext = filename + strlen(filename) - 4;
    int width = static_cast<int>(image.cols);
    int height = static_cast<int>(image.rows);
    const int comp = image.channels();
    
    assert(image.data != NULL);

    Size size = image.size();
    int type;
    const int channels = image.channels();
    switch (channels)
    {
    case 1:
        type = CV_8UC1;
        break;
    case 3:
        type = CV_8UC3;
        break;
    default:
        fprintf(stderr, "%s only support 1 or 3 channels\n", __FUNCTION__);
        abort();
    }
    Mat shadow;
    if (channels == 1)
    {
        shadow = image;
    }
    else
    {
        shadow.create(size, type);
        memcpy(shadow.data, image.data, width*height*channels);
        cvtColor(shadow, shadow, COLOR_BGR2RGB);
    }

    uchar* data = shadow.data;

    if (0 == strcmp(ext, ".jpg")) {
        int quality = 100;
        stbi_write_jpg(filename, width, height, comp, data, quality);
    }
    else if (0 == strcmp(ext, ".png")) {
        int stride_in_bytes = width * comp; // TODO: Mat may use line alignment
        stbi_write_png(filename, width, height, comp, data, stride_in_bytes);
    }
    else if (0 == strcmp(ext, ".bmp")) {
        stbi_write_bmp(filename, width, height, comp, data);
    }
    else {
        fprintf(stderr, "%s format is not supported yet!\n", ext);
    }
    // TODO: more types checking required
}

void cv::imwrite(const std::string& save_path, const cv::Mat& image)
{
    imwrite(save_path.c_str(), image);
}




double cv::get_current_time()
{
#ifdef _WIN32
    LARGE_INTEGER freq;
    LARGE_INTEGER pc;
    QueryPerformanceFrequency(&freq);
    QueryPerformanceCounter(&pc);

    return pc.QuadPart * 1000.0 / freq.QuadPart;
#else
    struct timeval tv;
    gettimeofday(&tv, NULL);

    return tv.tv_sec * 1000.0 + tv.tv_usec / 1000.0;
#endif // _WIN32
}

void cv::sleep(int milli_secs) {
#ifdef _MSC_VER
    Sleep(milli_secs);
#elif defined(__linux__)
    usleep(milli_secs * 1000); // 1 ms = 1000 us
#endif
}