# SmallCV

[![unittest](https://github.com/zchrissirhcz/smallcv/actions/workflows/unit_test.yml/badge.svg)](https://github.com/zchrissirhcz/smallcv/actions/workflows/unit_test.yml)
![Widows](https://img.shields.io/badge/Windows-gray?logo=windows&logoColor=blue)
![Ubuntu](https://img.shields.io/badge/Ubuntu-gray?logo=ubuntu)
![macOS](https://img.shields.io/badge/-macOS-333333?style=flat&logo=apple)

Small Computer Vision Library.

## Features
- Lightweight
    - [x] C++11 based
    - [x] Not including optimization
- Conformance
    - [x] API like OpenCV
    - [x] Same result on PC and Android


## APIs
- `Mat`
- `imread()`/`imwrite()`/`imshow()`
- `cvtColor()`/`resize()`

## Example Usage
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

![](assets/ncnn_simplepose_result.png)

```c++
#include <string>
#include "smallcv.hpp"

int main()
{
    std::string image_path = "mingren.jpg";
    cv::Mat image = cv::imread(image_path);
    cv::Rect rect(100, 100, 233, 233);
    cv::rectangle(image, rect, cv::Scalar(0,0,255), 2);
    cv::imshow("mingren", image);
    
    cv::Mat bigger_image;
    cv::Size ssize = image.size();
    cv::Size dsize;
    dsize.height = ssize.height * 2;
    dsize.width = ssize.width * 2;
    cv::resize(image, bigger_image, dsize);
    cv::imshow("bigger image", bigger_image);

    cv::imwrite("mingren-copy.png", image);

    cv::Mat gray;
    cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);
    cv::imwrite("gray.png", gray);
    cv::imshow("gray", gray);
    cv::waitKey(0);

    return 0;
}
```
![](assets/smallcv_imshow_multiple_win_result.png)


## Acknowledgement

SmallCV referenced the following projects：

- [OpenCV](https://github.com/opencv/opencv)
- [ncnn](https://github.com/tencent/ncnn)
- [shufaCV](https://github.com/scarsty/shufaCV)

