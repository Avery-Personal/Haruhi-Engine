//
//  Haruhi Engine
//
//  Original source code derived from Kunamo Engine:
//    Copyright © 2026 Kunamo Entertainment. All rights reserved.
//
//  Modifications & Integration:
//    Copyright © 2026 AveriC & Averi
//

#include <stdio.h>
#include <stdlib.h>

#include <GLFW/glfw3.h>
#include <SDL3/SDL.h>
#include <OpenGL/OpenGL.h>

#include "Window.h"
#include "../../Platform.h"
#include "../../../Core/Logging/Logging.h"

HaruWindowBackend gWindowBackend = HARU_WINDOW_BACKEND_NONE;

static int WindowingInitialized = 0;

static void HaruGLFWErrorCallback(int Code, const char *Description) {
    HARU_LOG_ERROR(&gLogger, "GLFW Error (%d): %s\n", Code, Description);
}

void HaruSetWindowBackend(HaruWindowBackend BACKEND) {
    gWindowBackend = BACKEND;
}

HaruResult HaruInitializeWindowing() {
    if (WindowingInitialized) {
        HARU_LOG_WARN(&gLogger, "Haruhi windowing ALREADY initialized, short passing...\n");

        return HARU_RESULT_SUCCESS;
    }

    if (gWindowBackend == HARU_WINDOW_BACKEND_GLFW) {
        glfwSetErrorCallback(HaruGLFWErrorCallback);

        if (!glfwInit()) {
            HARU_LOG_ERROR(&gLogger, "Couldn't initialize GLFW\n");

            return HARU_RESULT_ERROR;
        }

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

        WindowingInitialized = 1;

        return HARU_RESULT_SUCCESS;
    } else if (gWindowBackend == HARU_WINDOW_BACKEND_SDL) {
        if ((SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO)) == -1) {
            HARU_LOG_ERROR(&gLogger, "Couldn't initialize SDL: %s.\n", SDL_GetError());

            return HARU_RESULT_ERROR;
        }

        WindowingInitialized = 1;

        return HARU_RESULT_SUCCESS;
    } else if (gWindowBackend == HARU_WINDOW_BACKEND_NONE) {
        HARU_LOG_WARN(&gLogger, "Haruhi windowing backend empty or not created.\n");
        HARU_LOG_INFO(&gLogger, "Rollback - Reverting windowing backend to GLFW...\n");

        HaruSetWindowBackend(HARU_WINDOW_BACKEND_GLFW);

        return HaruInitializeWindowing();
    }

    return HARU_RESULT_FAILURE;
}

HaruWindow *HaruCreateWindow(const char *Title, int WIDTH, int HEIGHT) {
    HaruException Exception;

    HaruWindow *WINDOW = malloc(sizeof(HaruWindow));
    if (!WINDOW) {
        HARU_LOG_ERROR(&gLogger, "Failed to allocate Haruhi Window.\n");

        return NULL;
    }

    TRY(Exception) {
        if (gWindowBackend != HARU_WINDOW_BACKEND_GLFW && gWindowBackend != HARU_WINDOW_BACKEND_SDL) {
            HARU_LOG_ERROR(&gLogger, "Haruhi windowing backend empty or not created.\n");
            HARU_LOG_INFO(&gLogger, "Rollback - Reverting windowing backend to GLFW...\n");

            HaruSetWindowBackend(HARU_WINDOW_BACKEND_GLFW);

            if (HARUHI_RESOLVE)
                HaruCreateWindow(Title, WIDTH, HEIGHT);
            else
                THROW(Exception, 31);
        }

        if (!WindowingInitialized) {
            if (HaruInitializeWindowing() != HARU_RESULT_SUCCESS){
                HARU_LOG_ERROR(&gLogger, "Haruhi windowing initialization failed.\n");

                THROW(Exception, 32);
            }

            WindowingInitialized = 1;
        }

        if (gWindowBackend == HARU_WINDOW_BACKEND_GLFW) {
            GLFWwindow *GLFWHandle = glfwCreateWindow(WIDTH, HEIGHT, Title, NULL, NULL);
            if (!GLFWHandle) {
                HARU_LOG_ERROR(&gLogger, "GLFW Window creation failed.\n");

                glfwTerminate();

                THROW(Exception, 34);
            }

            *WINDOW = (HaruWindow){0};

            WINDOW -> Handle = GLFWHandle;
            WINDOW -> Application = NULL;

            WINDOW -> WIDTH = WIDTH;
            WINDOW -> HEIGHT = HEIGHT;

            WINDOW -> ShouldClose = 0;
            WINDOW -> Backend = HARU_WINDOW_BACKEND_GLFW;

            glfwSetWindowUserPointer(GLFWHandle, WINDOW);

            HaruInputBindWindow(WINDOW);

            return WINDOW;
        } else if (gWindowBackend == HARU_WINDOW_BACKEND_SDL) {
            // TODO: Create Furi to a testable state of rendering API detection
            SDL_Window *SDLHandle = SDL_CreateWindow(Title, WIDTH, HEIGHT, SDL_WINDOW_METAL);
            if (!SDLHandle) {
                HARU_LOG_ERROR(&gLogger, "SDL Window creation failed.\n");
                HARU_LOG_ERROR(&gLogger, "  - %s\n", SDL_GetError());

                SDL_Quit();

                THROW(Exception, 35);
            }

            *WINDOW = (HaruWindow){0};

            WINDOW -> Handle = SDLHandle;
            WINDOW -> Application = NULL;

            WINDOW -> WIDTH = WIDTH;
            WINDOW -> HEIGHT = HEIGHT;

            WINDOW -> ShouldClose = 0;

            return WINDOW;
        }

        THROW(Exception, 190226);
    } CATCH(Exception) {
        HARU_LOG_INFO(&gLogger, "Haruhi Exception failed with code: %d\n", Exception.Code);

        if (WINDOW)
            free(WINDOW);

        return NULL;
    }
}

void HaruDestroyWindow(HaruWindow *WINDOW) {
    if (gWindowBackend == HARU_WINDOW_BACKEND_GLFW) {
        glfwDestroyWindow(WINDOW -> Handle);
    } else if (gWindowBackend == HARU_WINDOW_BACKEND_SDL) {
        SDL_DestroyWindow(WINDOW -> Handle);
    }

    free(WINDOW);
}

void HaruWindowPollEvents(HaruWindow *WINDOW) {
    static int StatedDeprecation = 0;
    if (!StatedDeprecation) {
        HARU_LOG_INFO(&gLogger, "Haruhi usage of windowing poll events contains ONLY GLFW support as of now.\n");

        StatedDeprecation = 1;
    }

    if (gWindowBackend == HARU_WINDOW_BACKEND_GLFW) {
        if (glfwWindowShouldClose((GLFWwindow *) WINDOW -> Handle)) {
            WINDOW -> ShouldClose = 1;
        }
    }
}

void HaruPlatformPollEvents(HaruPlatform *Platform) {
    (void) Platform;
    
    static int StatedDeprecation = 0;
    if (!StatedDeprecation) {
        HARU_LOG_INFO(&gLogger, "Haruhi usage of platforming poll events contains ONLY GLFW support as of now.\n");

        StatedDeprecation = 1;
    }
    
    if (gWindowBackend == HARU_WINDOW_BACKEND_GLFW) {
        glfwPollEvents();
    } else if (gWindowBackend == HARU_WINDOW_BACKEND_SDL) {

    }
}

int HaruWindowShouldClose(HaruWindow *WINDOW) {
    return WINDOW -> ShouldClose;
}
