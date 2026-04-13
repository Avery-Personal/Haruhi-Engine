#ifndef HARUHI_LOGGING_H
#define HARUHI_LOGGING_H

    #include <stdio.h>
    #include <stdint.h>

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

    void HaruLoggerInitialize(HaruLogger *Logger);
    
    void HaruLoggerSetFile(HaruLogger *Logger, const char *Path);
    void HaruLoggerSetLevel(HaruLogger *Logger, HaruLoggingLevel Level);

    void HaruLog(HaruLogger *Logger, HaruLoggingLevel Level, const char *File, int Line, const char *Format, ...);

#endif
