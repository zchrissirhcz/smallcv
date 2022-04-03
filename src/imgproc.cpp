#include "smallcv.hpp"
#include <stdint.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

namespace {

template<int bIdx>
void cvtcolor_bgr_to_gray(const cv::Mat& src, cv::Mat& dst)
{
    if (src.depth() != CV_8U)
    {
        fprintf(stderr, "%s: only support uchar type", __FUNCTION__);
    }
    if (src.channels() != 3)
    {
        fprintf(stderr, "%s: src is not 3 channels", __FUNCTION__);
    }
    if (bIdx != 0 && bIdx != 2)
    {
        fprintf(stderr, "%s: bIdx should be 0 or 2", __FUNCTION__);
    }

    const int srcw = src.cols;
    const int srch = src.rows;
    const int channels = 3;

    dst.create(src.size(), CV_8UC1);
    for (int i = 0; i < srch; i++)
    {
        for (int j = 0; j < srcw; j++)
        {
            uchar b = src.ptr(i, j)[bIdx];
            uchar g = src.ptr(i, j)[1];
            uchar r = src.ptr(i, j)[2-bIdx];
            dst.ptr(i, j)[0] = (0.299*r + 0.587*g + 0.114*b);
        }
    }
}

template<typename T>
static
T clip(T val, T minval, T maxval)
{
    if (minval > maxval) {
        std::swap(minval, maxval);
    }

    T result = val;
    if (val < minval) {
        result = minval;
    }
    if (val > maxval) {
        result = maxval;
    }
    return result;
}

} // namespace

void cv::resize(const Mat& src, Mat& dst, Size dsize, Interpolation method)
{
    if (method == Interpolation::LINEAR)
    {
        cv::resize_linear(src, dst, dsize);
    }
    else if (method == Interpolation::NEAREST)
    {
        cv::resize_nearest(src, dst, dsize);
    }
    else
    {
        SMALLCV_LOGE("not implemented yet!\n");
    }
}


void cv::resize_nearest(const cv::Mat& src, cv::Mat& dst, cv::Size dsize)
{
    int depth = src.depth();
    if (depth != CV_8U) {
        SMALLCV_LOGE("only support uchar type");
    }

    cv::Size ssize = src.size();
    int channels = src.channels();

    dst.create(dsize, src.type());

    const int src_height = ssize.height;
    const int src_width = ssize.width;
    const int dst_height = dsize.height;
    const int dst_width = dsize.width;

    const float scale_w = src_width * 1.0 / dst_width;
    const float scale_h = src_height * 1.0 / dst_height;

    for (int i = 0; i < dst_height; i++)
    {
        int src_i = static_cast<int>(i * scale_h);
        src_i = clip(src_i, 0, src_height-1);

        for (int j = 0; j < dst_width; j++)
        {
            int src_j = static_cast<int>(j * scale_w); 
            src_j = clip(src_j, 0, src_width-1);

            for (int k = 0; k < channels; k++)
            {
                // .ptr(int, int) returns uchar* type
                dst.ptr(i, j)[k] = src.ptr(src_i, src_j)[k];
            }
        }
    }
}

void cv::resize_linear(const cv::Mat& src, cv::Mat& dst, cv::Size dsize)
{
    dst.create(dsize, src.type());

    const int src_width = src.cols;
    const int src_height = src.rows;

    int dst_width = dst.cols;
    int dst_height = dst.rows;

    const int channels = src.channels();

    double fw = double(src_width) / dst_width;
    double fh = double(src_height) / dst_height;

    int y1, y2, x1, x2;
    float fx1, fx2, fy1, fy2;
    for(int y = 0; y < dst_height; y++)
    {
        float y0 = (float)((y+0.5)*fh - 0.5);
        y1 = int(y0);
        if(y1 == src_height-1) y2 = y1;
        else y2 = y1 + 1;

        fy1 = y0 - y1;
        fy2 = 1.0f - fy1;
        for (int x = 0; x < dst_width; x++)
        {
            float x0 = (float)((x+0.5)*fw - 0.5);
            x1 = int(x0);
            if (x1 == src_width-1) x2 = x1;
            else x2 = x1+1;

            fx1 = x0 - x1;
            fx2 = 1.0f - fx1;

            float s1 = fx1*fy1;
            float s2 = fx2*fy1;
            float s3 = fx2*fy2;
            float s4 = fx1*fy2;

            for (int k = 0; k < channels; k++)
            {
                uchar c1 = src.ptr(y1, x1)[k];
                uchar c2 = src.ptr(y1, x2)[k];
                uchar c3 = src.ptr(y2, x1)[k];
                uchar c4 = src.ptr(y2, x2)[k];

                float v = c1*s3 + c2*s4 + c3*s2 + c4*s1;
                dst.ptr(y, x)[k] = v + 0.5;
            }
        }
    }
}

void cv::bgr2gray(const Mat& src, Mat& dst)
{
    cvtcolor_bgr_to_gray<0>(src, dst);
}

void cv::rgb2gray(const Mat& src, Mat& dst)
{
    cvtcolor_bgr_to_gray<2>(src, dst);
}

void cv::bgr2rgb_inplace(Mat& src)
{
    assert(src.type() == CV_8UC3);
    Mat& image = src;
    size_t h = image.rows;
    size_t w = image.cols;
    uchar* data = image.data;
    for (size_t i = 0; i < h; i++)
    {
        for (size_t j = 0; j < w; j++)
        {
            size_t idx = i * w * 3 + j * 3;
            unsigned char t = data[idx];
            data[idx] = data[idx + 2];
            data[idx + 2] = t;
        }
    }
}

void cv::bgr2rgb(const Mat& src, Mat& dst)
{
    assert(src.type() == CV_8UC3);
    dst.create(src.size(), CV_8UC1);

    size_t h = src.rows;
    size_t w = src.cols;
    const uchar* sp = src.data;
    uchar* dp = dst.data;
    for (size_t i = 0; i < h; i++)
    {
        for (size_t j = 0; j < w; j++)
        {
            size_t idx = i * w * 3 + j * 3;
            dp[idx + 0] = sp[idx + 2];
            dp[idx + 1] = sp[idx + 1];
            dp[idx + 2] = sp[idx + 0];
        }
    }
}

void cv::cvtColor(const cv::Mat& src, cv::Mat& dst, int flag)
{
    if (flag == COLOR_BGR2RGB)
    {
        if(src.data == dst.data)
        {
            bgr2rgb_inplace(dst);
        }
        else
        {
            bgr2rgb(src, dst);
        }
    }
    else if (flag == COLOR_BGR2GRAY)
    {
        bgr2gray(src, dst);
    }
    else
    {
        fprintf(stderr, "! error: not supported yet\n");
        exit(1);
    }
}

void cv::image_upside_down(const Mat& im, Mat& im_upsd)
{
    im_upsd.create(im.size(), im.type());

    int height = im.rows;
    int width = im.cols;
    int channels = im.channels();
    int linebytes = im_upsd.cols * channels;

    unsigned char* dst = im_upsd.data + linebytes*(height-1);
    unsigned char* src = im.data;
    unsigned char* outbuf = im_upsd.data;

    unsigned char* input = im.data;

    for (int h = 0; h < height; h++) {
        for (int wc = 0; wc < width*channels; wc++) {
            size_t src_idx = h*linebytes + wc;
            size_t dst_idx = (height-1-h)*linebytes + wc;
            outbuf[dst_idx] = input[src_idx];
        }
        memcpy(dst, src, linebytes);
        dst -= linebytes;
        src += linebytes;
    }
}



void cv::putText(Mat& img, const std::string& text, Point org, int fontFace, double fontScale, Scalar _color, int thickness)
{
    const int fontpixelsize = 20 * fontScale;

    unsigned int color = 0;
    unsigned char* border_color = (unsigned char*)&color;
    const int channels = img.channels();

    if (channels == 1)
    {
        border_color[0] = _color[0];
        draw_text_c1(img.data, img.cols, img.rows, text.c_str(), org.x, org.y - fontpixelsize * 2, fontpixelsize, color);
    }
    else if (channels == 3)
    {
        border_color[0] = _color[0];
        border_color[1] = _color[1];
        border_color[2] = _color[2];
        draw_text_c3(img.data, img.cols, img.rows, text.c_str(), org.x, org.y - fontpixelsize * 2, fontpixelsize, color);
    }
    else
    {
        SMALLCV_LOGE("only support 1 and 3 channels\n");
    }
}
