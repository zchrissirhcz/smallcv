# SmallCV

Lightweight `Mat` and `imread()`/`imwrite()`/`imshow()`.

Basic usage:
```c++
#include <string>
#include "smallcv.hpp"

int main() {
    std::string image_path = "mingren.jpg";
    sv::Mat image = sv::imread(image_path);
    sv::rgb_bgr_swap_inplace(image);
    sv::imwrite("mingren_swap.jpg", image);
    sv::imshow("mingren", image);
    sv::waitKey(0);
    sv::imwrite("mingren_swap.bmp", image);

    return 0;
}
```

Example screenshot:

![](assets/ncnn_simplepose_result.png)
