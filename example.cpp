
#ifdef VLD // for memory leak detection
#include <vld.h>
#endif

#include "smallcv.h"
#include <assert.h>

static void rgb_bgr_swap_inplace(sv::Mat& image);


void rgb_bgr_swap_inplace(sv::Mat& image)
{
    size_t h = image.get_height();
    size_t w = image.get_width();
    size_t c = image.get_channels();
    assert(c==3);
    uchar* data = image.data.get();
    for (size_t i=0; i<h; i++) {
        for (size_t j=0; j<w; j++) {
            size_t idx = i*w*3 + j*3;
            unsigned char t = data[idx];
            data[idx] = data[idx+2];
            data[idx+2] = t;
        }
    }
}

int main() {
    {
        std::string image_path = "mingren.jpg";
        sv::Mat image = sv::imread(image_path);
        rgb_bgr_swap_inplace(image);
        sv::imwrite("mingren_swap.jpg", image);
        sv::imwrite("mingren_swap.bmp", image);
    }

#ifdef VLD
    VLDReportLeaks();
#endif

    return 0;
}
