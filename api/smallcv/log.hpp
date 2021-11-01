#pragma once

#define SMALLCV_STDIO 1

#if SMALLCV_STDIO
#if SMALLCV_PLATFORM_API && __ANDROID_API__ >= 8
#include <android/log.h>
#define SMALLCV_LOGE(...) do { \
    fprintf(stderr, ##__VA_ARGS__); fprintf(stderr, "\n"); \
    __android_log_print(ANDROID_LOG_WARN, "smallcv", ##__VA_ARGS__); } while(0)
#else // SMALLCV_PLATFORM_API && __ANDROID_API__ >= 8
#include <stdio.h>
#define SMALLCV_LOGE(...) do { \
    fprintf(stderr, ##__VA_ARGS__); fprintf(stderr, "\n"); } while(0)
#endif // SMALLCV_PLATFORM_API && __ANDROID_API__ >= 8
#else
#define NCNN_LOGE(...)
#endif