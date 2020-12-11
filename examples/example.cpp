
#ifdef VLD // for memory leak detection
#include <vld.h>
#endif

#include <string>
#include "smallcv.hpp"

int main() {
    {
        std::string image_path = "mingren.jpg";
        sv::Mat image = sv::imread(image_path);
        sv::rgb_bgr_swap_inplace(image);
        sv::imwrite("mingren_swap.jpg", image);
        sv::imshow("mingren", image);
        sv::waitKey(0);
        sv::imwrite("mingren_swap.bmp", image);
    }

#ifdef VLD
    VLDReportLeaks();
#endif

    return 0;
}
