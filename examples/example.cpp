
#if defined(_MSC_VER) && defined(VLD) // for memory leak detection
#include <vld.h>
#endif

#include <string>
#include "smallcv.hpp"

int main() {
    {
        std::string image_path = "mingren.jpg";
        //std::string image_path = "E:/projects/arccv/smallcv/build/vs2019-x64/mingren.jpg";
        cv::Mat image = cv::imread(image_path);
        cv::Rect rect(100, 100, 233, 233);
        cv::rectangle(image, rect, cv::Scalar(0,0,255), 2);
        
        cv::Mat bigger_image;
        cv::Size ssize = image.size();
        cv::Size dsize = ssize * 2;
        cv::resize(image, bigger_image, dsize);

        cv::Mat gray;
        cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);
        

        cv::putText(image, "image", cv::Point(60, 60), 10, 1, cv::Scalar(255, 0, 0), 2);
        cv::putText(bigger_image, "bigger image", cv::Point(60, 60), 10, 1, cv::Scalar(255, 0, 0), 2);
        cv::putText(gray, "gray", cv::Point(60, 60), 10, 1, cv::Scalar(255, 0, 0), 2);

#if !__ANDROID__
        cv::imshow("mingren", image);
        cv::imshow("bigger image", bigger_image, true);
        cv::imshow("gray", gray);
        cv::waitKey(0);
#endif

        cv::imwrite("image.png", image);
        cv::imwrite("bigger_image.png", bigger_image);
        cv::imwrite("gray.png", gray);
    }

#ifdef VLD
    VLDReportLeaks();
#endif

    return 0;
}
