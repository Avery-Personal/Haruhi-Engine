//
//  Haruhi Engine
//
//  Copyright © 2026 AveriC & Averi
//

#include <string.h>
#include <GLFW/glfw3.h>

#include "Input.h"
#include "../../Platform/Runtime/Window/Window.h"
#include "../../Core/Application/Application.h"
#include "../../Core/Logging/Logging.h"

static HaruInputState *HaruGetInputFromGLFWWindow(GLFWwindow *GLFWWindow) {
    if (!GLFWWindow)
        return NULL;

    HaruWindow *Window = (HaruWindow *) glfwGetWindowUserPointer(GLFWWindow);
    if (!Window || !Window -> Application)
        return NULL;

    return &Window -> Application -> Platform.Input;
}

static void HaruGLFWKeyCallback(GLFWwindow *GLFWWindow, int Key, int Scancode, int Action, int Mods) {
    (void) Scancode;
    (void) Mods;

    HaruInputState *Input = HaruGetInputFromGLFWWindow(GLFWWindow);
    if (!Input)
        return;

    if (Key < 0 || Key >= HARU_MAX_KEYS)
        return;

    if (Action == GLFW_PRESS || Action == GLFW_REPEAT) {
        Input -> Keys[Key].IsPressed = HARU_TRUE;
    } else if (Action == GLFW_RELEASE) {
        Input -> Keys[Key].IsPressed = HARU_FALSE;
    }
}

static void HaruGLFWMouseButtonCallback(GLFWwindow *GLFWWindow, int Button, int Action, int Mods) {
    (void) Mods;

    HaruInputState *Input = HaruGetInputFromGLFWWindow(GLFWWindow);
    if (!Input)
        return;

    if (Button < 0 || Button >= HARU_MAX_MOUSE_BUTTONS)
        return;

    if (Action == GLFW_PRESS || Action == GLFW_REPEAT) {
        Input -> MouseButtons[Button].IsDown = HARU_TRUE;
    } else if (Action == GLFW_RELEASE) {
        Input -> MouseButtons[Button].IsDown = HARU_FALSE;
    }
}

static void HaruGLFWCursorPositionCallback(GLFWwindow *GLFWWindow, double XPosition, double YPosition) {
    HaruInputState *Input = HaruGetInputFromGLFWWindow(GLFWWindow);
    if (!Input)
        return;

    int NewX = (int) XPosition;
    int NewY = (int) YPosition;

    if (!Input -> HasMousePosition) {
        Input -> MouseDeltaX = 0;
        Input -> MouseDeltaY = 0;

        Input -> HasMousePosition = HARU_TRUE;
    } else {
        Input -> MouseDeltaX = NewX - Input -> MouseX;
        Input -> MouseDeltaY = NewY - Input -> MouseY;
    }

    Input -> MouseX = NewX;
    Input -> MouseY = NewY;
}

static void HaruGLFWFocusCallback(GLFWwindow *GLFWWindow, int Focused) {
    HaruInputState *Input = HaruGetInputFromGLFWWindow(GLFWWindow);
    if (!Input)
        return;

    Input -> HasFocus = Focused ? HARU_TRUE : HARU_FALSE;
}

void HaruInputInitialize(HaruPlatform *Platform) {
    if (!Platform)
        return;

    memset(&Platform -> Input, 0, sizeof(Platform -> Input));
}

void HaruInputBeginFrame(HaruPlatform *Platform) {
    if (!Platform)
        return;

    HaruInputState *Input = &Platform -> Input;

    for (int i = 0; i < HARU_MAX_KEYS; ++i) {
        Input -> Keys[i].WasPressed = Input -> Keys[i].IsPressed;
    }

    for (int i = 0; i < HARU_MAX_MOUSE_BUTTONS; ++i) {
        Input -> MouseButtons[i].WasDown = Input -> MouseButtons[i].IsDown;
    }

    Input -> MouseDeltaX = 0;
    Input -> MouseDeltaY = 0;
}

void HaruInputBindWindow(HaruWindow *Window) {
    if (!Window || Window -> Backend != HARU_WINDOW_BACKEND_GLFW)
        return;

    GLFWwindow *GLFWWindow = (GLFWwindow *)Window -> Handle;
    if (!GLFWWindow)
        return;

    glfwSetWindowUserPointer(GLFWWindow, Window);
    glfwSetKeyCallback(GLFWWindow, HaruGLFWKeyCallback);
    glfwSetMouseButtonCallback(GLFWWindow, HaruGLFWMouseButtonCallback);
    glfwSetCursorPosCallback(GLFWWindow, HaruGLFWCursorPositionCallback);
    glfwSetWindowFocusCallback(GLFWWindow, HaruGLFWFocusCallback);
}

int HaruInputKeyDown(const HaruPlatform *Platform, int Key) {
    if (!Platform || Key < 0 || Key >= HARU_MAX_KEYS)
        return 0;

    return Platform -> Input.Keys[Key].IsPressed;
}

int HaruInputKeyPressed(const HaruPlatform *Platform, int Key) {
    if (!Platform || Key < 0 || Key >= HARU_MAX_KEYS)
        return 0;

    return Platform -> Input.Keys[Key].IsPressed && !Platform -> Input.Keys[Key].WasPressed;
}

int HaruInputKeyReleased(const HaruPlatform *Platform, int Key) {
    if (!Platform || Key < 0 || Key >= HARU_MAX_KEYS)
        return 0;

    return !Platform -> Input.Keys[Key].IsPressed && Platform -> Input.Keys[Key].WasPressed;
}

int HaruInputMouseDown(const HaruPlatform *Platform, int Button) {
    if (!Platform || Button < 0 || Button >= HARU_MAX_MOUSE_BUTTONS)
        return 0;

    return Platform -> Input.MouseButtons[Button].IsDown;
}

int HaruInputMousePressed(const HaruPlatform *Platform, int Button) {
    if (!Platform || Button < 0 || Button >= HARU_MAX_MOUSE_BUTTONS)
        return 0;

    return Platform -> Input.MouseButtons[Button].IsDown && !Platform -> Input.MouseButtons[Button].WasDown;
}

int HaruInputMouseReleased(const HaruPlatform *Platform, int Button) {
    if (!Platform || Button < 0 || Button >= HARU_MAX_MOUSE_BUTTONS)
        return 0;

    return !Platform -> Input.MouseButtons[Button].IsDown && Platform -> Input.MouseButtons[Button].WasDown;
}

void HaruInputGetMousePosition(const HaruPlatform *Platform, int *X, int *Y) {
    if (!Platform)
        return;

    if (X)
        *X = Platform -> Input.MouseX;

    if (Y)
        *Y = Platform -> Input.MouseY;
}

void HaruInputGetMouseDelta(const HaruPlatform *Platform, int *DX, int *DY) {
    if (!Platform)
        return;

    if (DX)
        *DX = Platform -> Input.MouseDeltaX;

    if (DY)
        *DY = Platform -> Input.MouseDeltaY;
}
