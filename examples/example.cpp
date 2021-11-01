
#if defined(_MSC_VER) && defined(VLD) // for memory leak detection
#include <vld.h>
#endif

#include <string>
#include "smallcv/smallcv.hpp"

int main() {
    {
        std::string image_path = "mingren.jpg";
        //std::string image_path = "E:/projects/arccv/smallcv/build/vs2019-x64/mingren.jpg";
        cv::Mat image = cv::imread(image_path);
        cv::Rect rect(100, 100, 233, 233);
        cv::rectangle(image, rect, cv::Scalar(0,0,255), 2);
        cv::imshow("mingren", image);
        cv::waitKey(0);
        cv::imwrite("mingren-copy.png", image);
    }

#ifdef VLD
    VLDReportLeaks();
#endif

    return 0;
}
