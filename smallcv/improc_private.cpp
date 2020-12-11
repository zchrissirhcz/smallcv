#include "mat.hpp"
#include "improc_private.hpp"
#include <assert.h>

namespace sv {

    void image_upside_down(const Mat& im, Mat& im_upsd)
    {
        int height = im.get_height();
        int width = im.get_width();
        int channels = im.get_channels();
        assert(channels == 3);
        int linebytes = im_upsd.get_width() * channels;

        unsigned char* dst = im_upsd.data.get() + linebytes*(height-1);
        unsigned char* src = im.data.get();
        unsigned char* outbuf = im_upsd.data.get();

        unsigned char* input = im.data.get();

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

}