#ifndef HARUHI_LOGGING_H
#define HARUHI_LOGGING_H

    #include <stdio.h>
    #include <stdint.h>

    #define HARU_COLOR_RESET "\033[0m"
    #define HARU_COLOR_TRACE "\033[37m"
    #define HARU_COLOR_DEBUG "\033[36m"
    #define HARU_COLOR_INFO "\033[32m"
    #define HARU_COLOR_WARN "\033[33m"
    #define HARU_COLOR_ERROR "\033[31m"
    #define HARU_COLOR_FATAL "\033[41m\033[37m"

    typedef enum {
        HARU_LOGGING_TRACE,
        HARU_LOGGING_DEBUG,
        HARU_LOGGING_INFO,
        HARU_LOGGING_WARN,
        HARU_LOGGING_ERROR,
        HARU_LOGGING_FATAL
    } HaruLoggingLevel;

    typedef struct HaruLogger {
        FILE *File;

        uint8_t EnableColors;
        uint8_t EnableFile;

        HaruLoggingLevel Level;
    } HaruLogger;

    extern HaruLogger gLogger;

    void HaruLoggerInitialize(HaruLogger *Logger);

    void HaruLoggerSetFile(HaruLogger *Logger, const char *Path);
    void HaruLoggerSetLevel(HaruLogger *Logger, HaruLoggingLevel Level);

    void HaruLog(HaruLogger *Logger, HaruLoggingLevel Level, const char *File, int Line, const char *Format, ...);

    #define HARU_LOG_TRACE(Logger, Format, ...) HaruLog(Logger, HARU_LOGGING_TRACE, __FILE__, __LINE__, Format, ##__VA_ARGS__)
    #define HARU_LOG_DEBUG(Logger, Format, ...) HaruLog(Logger, HARU_LOGGING_DEBUG, __FILE__, __LINE__, Format, ##__VA_ARGS__)
    #define HARU_LOG_INFO(Logger, Format, ...) HaruLog(Logger, HARU_LOGGING_INFO, __FILE__, __LINE__, Format, ##__VA_ARGS__)
    #define HARU_LOG_WARN(Logger, Format, ...) HaruLog(Logger, HARU_LOGGING_WARN, __FILE__, __LINE__, Format, ##__VA_ARGS__)
    #define HARU_LOG_ERROR(Logger, Format, ...) HaruLog(Logger, HARU_LOGGING_ERROR, __FILE__, __LINE__, Format, ##__VA_ARGS__)
    #define HARU_LOG_FATAL(Logger, Format, ...) HaruLog(Logger, HARU_LOGGING_FATAL, __FILE__, __LINE__, Format, ##__VA_ARGS__)

#endif
