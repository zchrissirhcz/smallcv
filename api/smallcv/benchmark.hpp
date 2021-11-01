#pragma once

namespace cv {

    /// @brief get now timestamp in ms（毫秒）
    double get_current_time();

    /// @brief sleep for specified milliseconds (10^-3 second)
    /// 等待指定的毫秒数
    void sleep(int milli_secs);
}
