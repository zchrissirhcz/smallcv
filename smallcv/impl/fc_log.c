#include <stdio.h>

//----- gettime -----
#include <time.h>
#ifdef _MSC_VER
#include <windows.h>
#else
#include <unistd.h>
#include <sys/time.h>
#endif

//----- log -----
#include "fc_log.h"
#include <stdarg.h>

#if ANDROID
#include <android/log.h>
#define LOG_TAG "FC_LOG"
#endif

int fc_gettimeofday(struct timeval* tp, void* tzp) {
#ifdef _MSC_VER
    time_t clock;
    struct tm tm;
    SYSTEMTIME wtm;

    GetLocalTime(&wtm);
    tm.tm_year = wtm.wYear - 1900;
    tm.tm_mon = wtm.wMonth - 1;
    tm.tm_mday = wtm.wDay;
    tm.tm_hour = wtm.wHour;
    tm.tm_min = wtm.wMinute;
    tm.tm_sec = wtm.wSecond;
    tm.tm_isdst = -1;
    clock = mktime(&tm);
    tp->tv_sec = clock;
    tp->tv_usec = wtm.wMilliseconds * 1000;

    return (0);
#else
    return gettimeofday(tp, tzp);
#endif
}

// return current time in milli-second format
long fc_gettime() {
    struct timeval time;
    fc_gettimeofday(&time, NULL);
    return (time.tv_sec * 1000 + time.tv_usec / 1000);
}


void fc_sleep(int milli_secs) {
#ifdef _MSC_VER
    Sleep(milli_secs);
#elif defined(__linux__)
    usleep(milli_secs * 1000); // 1 ms = 1000 us
#endif
}

void fc_echo_platform() {
#ifdef _MSC_VER
    FC_LOGD("MSVC platform\n");
#endif

#ifdef __linux__
    FC_LOGD("linux platform\n");
#endif

#ifdef ANDROID
    FC_LOGD("android platform\n");
#endif

#ifdef __APPLE__
    FC_LOGD("Apple\n");
#endif

#ifdef TI_C66
    FC_LOGD("TI_C66 DSP platform\n");
#endif
}


static const char* level_names[] = {
    //"TRACE", "DEBUG", "INFO", "WARN", "ERROR", "FATAL"
    "DEBUG", "ERROR"
};

void fc_log(FcLogLevel level, const char* file, int line, const char* fmt, ...) {
    if (level <= kFcLogLevel_Begin || level >= kFcLogLevel_End) {
        return;
    }

//print to console for both PC & Android
#if defined(_MSC_VER) || (defined(__linux__) || defined(__APPLE__))
    va_list args;
    va_start(args, fmt);

    FILE* fout = stdout;
    if (level == kFcLogLevel_Debug) {
        fout = stdout;
    }
    else if (level == kFcLogLevel_Error) {
        fout = stderr;
    }

    time_t t = time(NULL);
    struct tm* lt = localtime(&t);
    char now[100];
    now[strftime(now, sizeof(now), "%Y-%m-%d %H:%M:%S", lt)] = '\0';
    fprintf(fout, "%s [%-5s] %s:%d: ", now, level_names[level], file, line);

    vfprintf(fout, fmt, args);
    va_end(args);
    fprintf(fout, "\n");
    fflush(fout);

#elif defined(TI_C66)
    //we could use Vps_printf(__VA_ARGS__) to replace printf(). but here may not.
#endif

//for android, we also print to logcat
#if ANDROID
    va_start(args, fmt);
    if (level == kFcLogLevel_Debug) {
        __android_log_vprint(ANDROID_LOG_DEBUG, LOG_TAG, fmt, args);
    }
    else if (level == kFcLogLevel_Error) {
        __android_log_vprint(ANDROID_LOG_ERROR, LOG_TAG, fmt, args);
    }
    va_end(args);
#endif

}