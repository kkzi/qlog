#pragma once

#if defined(_WIN32)
    #if defined(QLOG_EXPORTS)
        #define QLOG_API __declspec(dllexport)
    #else
        #define QLOG_API __declspec(dllimport)
    #endif
#else
    #define QLOG_API
#endif
