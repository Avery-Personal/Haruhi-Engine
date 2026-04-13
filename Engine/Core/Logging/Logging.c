#include "Logging.h"

#include <stdarg.h>
#include <string.h>
#include <time.h>

static const char *HaruLevelToString(HaruLoggingLevel Level) {
    switch (Level) {
        case HARU_LOGGING_TRACE: return "TRACE";
        case HARU_LOGGING_DEBUG: return "DEBUG";
        case HARU_LOGGING_INFO: return "INFO";
        case HARU_LOGGING_WARN: return "WARN";
        case HARU_LOGGING_ERROR: return "ERROR";
        case HARU_LOGGING_FATAL: return "FATAL";

        default: return "UNKNOWN";
    }
}

static const char *HaruLevelToColor(HaruLoggingLevel Level) {
    switch (Level) {
        case HARU_LOGGING_TRACE: return HARU_COLOR_TRACE;
        case HARU_LOGGING_DEBUG: return HARU_COLOR_DEBUG;
        case HARU_LOGGING_INFO: return HARU_COLOR_INFO;
        case HARU_LOGGING_WARN: return HARU_COLOR_WARN;
        case HARU_LOGGING_ERROR: return HARU_COLOR_ERROR;
        case HARU_LOGGING_FATAL: return HARU_COLOR_FATAL;

        default: return HARU_COLOR_RESET;
    }
}

static void HaruGetTime(char *buffer, size_t size) {
    time_t Time = time(NULL);

    struct tm *TimeInformation = localtime(&Time);

    strftime(buffer, size, "%H:%M:%S", TimeInformation);
}

void HaruLoggerInitialize(HaruLogger *Logger) {
    if (!Logger)
        return;

    Logger -> File = NULL;
    Logger -> EnableColors = 1;
    Logger -> EnableFile = 0;
    Logger -> Level = HARU_LOGGING_TRACE;
}

void HaruLoggerSetFile(HaruLogger *Logger, const char *Path) {
    if (!Logger)
        return;

    if (Logger -> File) {
        fclose(Logger -> File);

        Logger -> File = NULL;
    }

    if (Path) {
        Logger -> File = fopen(Path, "a");
        Logger -> EnableFile = (Logger -> File != NULL);
    }
}

void HaruLoggerSetLevel(HaruLogger *Logger, HaruLoggingLevel Level) {
    if (!Logger)
        return;

    Logger -> Level = Level;
}

void HaruLog(HaruLogger *Logger, HaruLoggingLevel Level, const char *File, int Line, const char *Format, ...) {
    if (!Logger)
        return;

    if (Level < Logger -> Level)
        return;

    char TimeBuffer[16];

    HaruGetTime(TimeBuffer, sizeof(TimeBuffer));

    const char *LevelString = HaruLevelToString(Level);
    const char *Color = Logger -> EnableColors ? HaruLevelToColor(Level) : "";

    char Message[1024];

    va_list Arguments;

    va_start(Arguments, Format);

    vsnprintf(Message, sizeof(Message), Format, Arguments);

    va_end(Arguments);

    fprintf(stdout, "%s[%s] [%s] %s (%s:%d)%s\n", Color, TimeBuffer, LevelString, Message, File, Line, HARU_COLOR_RESET);

    if (Logger -> EnableFile && Logger -> File) {
        fprintf(Logger -> File, "[%s] [%s] %s (%s:%d)\n", TimeBuffer, LevelString, Message, File, Line);

        fflush(Logger -> File);
    }
}
