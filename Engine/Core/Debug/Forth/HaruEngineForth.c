#include "HaruEngineForth.h"
#include "../../Application/Application.h"
#include "../../Logging/Logging.h"

static int WordLogTrace(HaruForthContext *Context) {
    HaruForthValue Value;

    if (!HaruForthPop(Context, &Value))
        return 0;

    if (Value.Type == HARU_FORTH_TYPE_NUMBER) {
        HARU_LOG_TRACE(&gLogger, "Forth: %g", Value.As.Number);
    } else if (Value.Type == HARU_FORTH_TYPE_STRING) {
        HARU_LOG_TRACE(&gLogger, "%s", Value.As.String);
    }
    
    return 1;
}

static int WordLogDebug(HaruForthContext *Context) {
    HaruForthValue Value;

    if (!HaruForthPop(Context, &Value))
        return 0;

    if (Value.Type == HARU_FORTH_TYPE_NUMBER) {
        HARU_LOG_DEBUG(&gLogger, "Forth: %g", Value.As.Number);
    } else if (Value.Type == HARU_FORTH_TYPE_STRING) {
        HARU_LOG_DEBUG(&gLogger, "%s", Value.As.String);
    }
    
    return 1;
}

static int WordLogInfo(HaruForthContext *Context) {
    HaruForthValue Value;

    if (!HaruForthPop(Context, &Value))
        return 0;

    if (Value.Type == HARU_FORTH_TYPE_NUMBER) {
        HARU_LOG_INFO(&gLogger, "Forth: %g", Value.As.Number);
    } else if (Value.Type == HARU_FORTH_TYPE_STRING) {
        HARU_LOG_INFO(&gLogger, "%s", Value.As.String);
    }
    
    return 1;
}

static int WordLogWarn(HaruForthContext *Context) {
    HaruForthValue Value;

    if (!HaruForthPop(Context, &Value))
        return 0;

    if (Value.Type == HARU_FORTH_TYPE_NUMBER) {
        HARU_LOG_WARN(&gLogger, "Forth: %g", Value.As.Number);
    } else if (Value.Type == HARU_FORTH_TYPE_STRING) {
        HARU_LOG_WARN(&gLogger, "%s", Value.As.String);
    }
    
    return 1;
}

static int WordLogError(HaruForthContext *Context) {
    HaruForthValue Value;

    if (!HaruForthPop(Context, &Value))
        return 0;

    if (Value.Type == HARU_FORTH_TYPE_NUMBER) {
        HARU_LOG_ERROR(&gLogger, "Forth: %g", Value.As.Number);
    } else if (Value.Type == HARU_FORTH_TYPE_STRING) {
        HARU_LOG_ERROR(&gLogger, "%s", Value.As.String);
    }
    
    return 1;
}

static int WordTimeDelta(HaruForthContext *Context) {
    HaruApplication *Application = (HaruApplication *) Context -> UserData;
    if (!Application)
        return 0;

    return HaruForthPushNumber(Context, Application -> Time.DeltaTime);
}

static int WordTimeTotal(HaruForthContext *Context) {
    HaruApplication *Application = (HaruApplication *) Context -> UserData;
    if (!Application)
        return 0;

    return HaruForthPushNumber(Context, Application -> Time.TotalTime);
}

static int WordWindowCount(HaruForthContext *Context) {
    HaruApplication *Application = (HaruApplication *) Context -> UserData;
    if (!Application)
        return 0;

    return HaruForthPushNumber(Context, (double) Application -> WindowCount);
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
