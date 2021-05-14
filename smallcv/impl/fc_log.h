#ifndef FC_LOG_H
#define FC_LOG_H

#include <time.h>

#ifdef __cplusplus
extern "C" {
#endif

    //--------------------------------------------------
    // platform name test
    //--------------------------------------------------
    void fc_platform_print();

    //--------------------------------------------------
    // get time functions
    //--------------------------------------------------
    long fc_gettime();
    int fc_gettimeofday(struct timeval *tp, void *tzp);

    //--------------------------------------------------
    // sleep function
    //--------------------------------------------------
    void fc_sleep(int milli_secs);

    //--------------------------------------------------
    // log (print) macro
    //--------------------------------------------------
#define FC_DEBUG

    //we must define `FC_LOGD` macro for any platform.
    //it is a replacement for printf when printing log info

    typedef enum FcLogLevel {
        kFcLogLevel_Begin = -1,

        kFcLogLevel_Debug,
        kFcLogLevel_Error,

        kFcLogLevel_End
    }FcLogLevel;

#ifdef FC_DEBUG

#define FC_LOG(level, ...)	fc_log(level, __FILE__, __LINE__, __VA_ARGS__)
#define FC_LOGD(...)		fc_log(kFcLogLevel_Debug, __FILE__, __LINE__, __VA_ARGS__)
#define FC_LOGE(...)		fc_log(kFcLogLevel_Error, __FILE__, __LINE__, __VA_ARGS__)

#else

#define FC_LOG(level, ...)	fc_log(level, __FILE__, __LINE__, __VA_ARGS__)
#define FC_LOGD(...)
#define FC_LOGE(...)		fc_log(kFcLogLevel_Error, __FILE__, __LINE__, __VA_ARGS__)

#endif // FC_DEBUG

    void fc_log(FcLogLevel level, const char* file, int line, const char* fmt, ...);





#ifdef FC_DEBUG_OLD // debug mode
    // -- begin of windows/linux printf
#if defined(_MSC_VER) || (defined(__linux__)&&!ANDROID)
#define FC_PRINTF(...) printf(__VA_ARGS__)
#define FC_LOGD(...) printf(__VA_ARGS__)
#define FC_LOGE(...) fprintf(stderr, __VA_ARGS__)
    // -- end of window/linux printf

    // -- begin of TI_C66 DSP printf
#elif defined(TI_C66)
#define FC_PRINTF(...) Vps_printf(__VA_ARGS__)
    // -- end of TI_C66 DSP printf

    // -- begin of android printf
#endif

#if ANDROID
#include <android/log.h>
#define LOG_TAG "FC_LOG"
#define FC_PRINTF(...)	__android_log_print(ANDROID_LOG_VERBOSE, LOG_TAG, __VA_ARGS__)
#define FC_LOGV(...)	__android_log_print(ANDROID_LOG_VERBOSE, LOG_TAG, __VA_ARGS__)
#define FC_LOGD(...)	__android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)
#define FC_LOGI(...)	__android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define FC_LOGW(...)	__android_log_print(ANDROID_LOG_WARN, LOG_TAG, __VA_ARGS__)
#define FC_LOGE(...)	__android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
    // -- end of android printf
#endif

#else // release mode
#define FC_PRINTF(...) 
#endif


#ifdef __cplusplus
}
#endif

#endif