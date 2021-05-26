
#ifdef VLD // for memory leak detection
#include <vld.h>
#endif

#include <string>
#include "smallcv/api/smallcv.hpp"

int main() {
    {
        std::string image_path = "mingren.jpg";
        cv::Mat image = cv::imread(image_path);
        //cv::rgb_bgr_swap_inplace(image);
        cv::Rect rect(100, 100, 233, 233);
        cv::rectangle(image, rect, cv::Scalar(0,0,255), 2);
        cv::imwrite("mingren_swap.jpg", image);
        cv::imshow("mingren", image);
        cv::waitKey(0);
        cv::imwrite("mingren_swap.bmp", image);
    }

#ifdef VLD
    VLDReportLeaks();
#endif

    return 0;
}
