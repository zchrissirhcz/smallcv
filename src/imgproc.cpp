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

} // namespace

void cv::resize(const Mat& src, Mat& dst, Size dsize, double fx, double fy, Interpolation method)
{
    //TODO
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

// template<int bIdx>
// void cvtcolor_bgr_to_gray(const cv::Mat& src, cv::Mat& dst)
// {
//     if (src.depth() != CV_8U)
//     {
//         fprintf(stderr, "only support uchar type");
//     }
//     if (src.channels() != 3)
//     {
//         fprintf(stderr, "src is not 3 channels");
//     }
//     if (bIdx != 0 && bIdx != 2)
//     {
//         fprintf(stderr, "bIdx should be 0 or 2");
//     }

//     const int srcw = src.cols;
//     const int srch = src.rows;
//     const int channels = 3;

//     dst.create(src.size(), CV_8UC1);
//     const uchar* src_line = src.data;
//     const int src_step = src.step1();
//     uchar* dst_line = dst.data;
//     const int dst_step = dst.step1();

//     const uint8_t R2Y_fx_u8 = 77; // 0.299 * (1 >> 8)
//     const uint8_t G2Y_fx_u8 = 150; // 0.587 * (1 >> 8)
//     const uint8_t B2Y_fx_u8 = 29; // 0.114 * (1 >> 8)

//     const uint16_t R2Y_fx = R2Y_fx_u8;
//     const uint16_t G2Y_fx = G2Y_fx_u8;
//     const uint16_t B2Y_fx = B2Y_fx_u8;
//     const uint16_t shift = 8;
//     const uint16_t half_fx = (1 << (shift - 1));

// #if __ARM_NEON
//     uint8x8_t v_R2Y_fx = vdup_n_u8(R2Y_fx_u8);
//     uint8x8_t v_G2Y_fx = vdup_n_u8(G2Y_fx_u8);
//     uint8x8_t v_B2Y_fx = vdup_n_u8(B2Y_fx_u8);
//     uint16x8_t v_half_fx = vdupq_n_u16(half_fx);
// #endif // __ARM_NEON

//     cv::parallel_for_(cv::Range(0, srch), [&](const cv::Range& range) {
//         for (int i = range.start; i < range.end; i++)
//         {
//             const uchar* src_pixel = src_line;
//             uchar* dst_pixel = dst_line;

// #if __ARM_NEON
//             int nn = srcw >> 3;
//             int remain = srcw - (nn << 3);
// #else
//             int remain = srcw;
// #endif // __ARM_NEON


// #if __ARM_NEON
//             for (int j = 0; j < nn; j++)
//             {
//                 uint8x8x3_t v_src = vld3_u8(src_pixel);
//                 uint16x8_t v_b2p = vmull_u8(v_src.val[0], v_B2Y_fx);
//                 uint16x8_t v_g2p = vmull_u8(v_src.val[1], v_G2Y_fx);
//                 uint16x8_t v_r2p = vmull_u8(v_src.val[2], v_R2Y_fx);
//                 uint16x8_t v_gray = vaddq_u16(vaddq_u16(vaddq_u16(v_b2p, v_g2p), v_r2p), v_half_fx);
//                 uint8x8_t v_gray_u8 = vshrn_n_u16(v_gray, 8);
//                 vst1_u8(dst_pixel, v_gray_u8);

//                 src_pixel += 8 * channels;
//                 dst_pixel += 8;
//             }
// #endif // __ARM_NEON

//             for (; remain >= 0; remain--)
//             {
//                 uchar b = src_pixel[0];
//                 uchar g = src_pixel[1];
//                 uchar r = src_pixel[2];
//                 //uint16_t gray = (77 * r + 151 * g + 28 * b + (1 << 7)) >> 8;
//                 uint16_t gray = (R2Y_fx * r + G2Y_fx * g + B2Y_fx * b + half_fx) >> shift;
//                 *dst_pixel++ = cv::saturate_cast<uint8_t>(gray);

//                 src_pixel += channels;
//             }
//             src_line += src_step;
//             dst_line += dst_step;
//         }
//     });
// }


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
    assert(channels == 3);
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