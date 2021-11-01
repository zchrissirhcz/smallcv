#include "smallcv/benchmark.hpp"
#include <stdio.h>

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#else // _WIN32
#include <sys/time.h>
#include <unistd.h>
#endif // _WIN32

#include <stddef.h>


double cv::get_current_time()
{
#ifdef _WIN32
    LARGE_INTEGER freq;
    LARGE_INTEGER pc;
    QueryPerformanceFrequency(&freq);
    QueryPerformanceCounter(&pc);

    return pc.QuadPart * 1000.0 / freq.QuadPart;
#else
    struct timeval tv;
    gettimeofday(&tv, NULL);

    return tv.tv_sec * 1000.0 + tv.tv_usec / 1000.0;
#endif // _WIN32
}

void cv::sleep(int milli_secs) {
#ifdef _MSC_VER
    Sleep(milli_secs);
#elif defined(__linux__)
    usleep(milli_secs * 1000); // 1 ms = 1000 us
#endif
}