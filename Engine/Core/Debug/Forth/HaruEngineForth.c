#include "HaruEngineForth.h"
#include "../../Application/Application.h"
#include "../../Logging/Logging.h"

static int WordLogTrace(HaruForthContext *Context) {
    HARU_LOG_TRACE(&gLogger, "Forth trace command");
    
    return 1;
}

static int WordLogDebug(HaruForthContext *Context) {
    HARU_LOG_DEBUG(&gLogger, "Forth debug command");
    
    return 1;
}

static int WordLogInfo(HaruForthContext *Context) {
    HARU_LOG_INFO(&gLogger, "Forth info command");
    
    return 1;
}

static int WordLogWarn(HaruForthContext *Context) {
    HARU_LOG_WARN(&gLogger, "Forth warn command");
    
    return 1;
}

static int WordLogError(HaruForthContext *Context) {
    HARU_LOG_ERROR(&gLogger, "Forth error command");

    return 1;
}

static int WordTimeDelta(HaruForthContext *Context) {
    HaruApplication *Application = (HaruApplication *) Context -> UserData;
    if (!Application)
        return 0;

    return HaruForthPush(Context, Application -> Time.DeltaTime);
}

static int WordTimeTotal(HaruForthContext *Context) {
    HaruApplication *Application = (HaruApplication *) Context -> UserData;
    if (!Application)
        return 0;

    return HaruForthPush(Context, Application -> Time.TotalTime);
}

static int WordWindowCount(HaruForthContext *Context) {
    HaruApplication *Application = (HaruApplication *) Context -> UserData;
    if (!Application)
        return 0;

    return HaruForthPush(Context, (double) Application -> WindowCount);
}

static int WordQuit(HaruForthContext *Context) {
    HaruApplication *Application = (HaruApplication *) Context -> UserData;
    if (!Application)
        return 0;

    Application -> Running = 0;
    Application -> Engine.Running = 0;

    return 1;
}

void HaruForthRegisterEngineWords(HaruForthContext *Context) {
    HaruForthRegisterWord(Context, "log-trace", WordLogTrace);
    HaruForthRegisterWord(Context, "log-debug", WordLogDebug);
    HaruForthRegisterWord(Context, "log-info",  WordLogInfo);
    HaruForthRegisterWord(Context, "log-warn",  WordLogWarn);
    HaruForthRegisterWord(Context, "log-error", WordLogError);

    HaruForthRegisterWord(Context, "time-delta", WordTimeDelta);
    HaruForthRegisterWord(Context, "time-total", WordTimeTotal);
    HaruForthRegisterWord(Context, "window-count", WordWindowCount);

    HaruForthRegisterWord(Context, "quit", WordQuit);
}
