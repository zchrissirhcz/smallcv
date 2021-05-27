# SmallCV

Lightweight `Mat` and `imread()`/`imwrite()`/`imshow()`.

Basic usage:
```c++
#include <string>
#include "smallcv.hpp"

int main() {
    std::string image_path = "mingren.jpg";
    cv::Mat image = cv::imread(image_path);
    cv::Rect rect(100, 100, 233, 233);
    cv::rectangle(image, rect, cv::Scalar(0,0,255), 2);
    cv::imshow("mingren", image);
    cv::waitKey(0);
    cv::imwrite("mingren_swap.bmp", image);

    return 0;
}
```

Example screenshot:

![](assets/ncnn_simplepose_result.png)
