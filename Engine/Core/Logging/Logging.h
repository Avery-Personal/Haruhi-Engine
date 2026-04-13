#ifndef HARUHI_LOGGING_H
#define HARUHI_LOGGING_H

    #include <stdint.h>

    typedef enum {
        HARU_LOGGING_TRACE,
        HARU_LOGGING_DEBUG,
        HARU_LOGGING_INFO,
        HARU_LOGGING_WARN,
        HARU_LOGGING_ERROR,
        HARU_LOGGING_FATAL
    } HaruLoggingLevel;

#endif
