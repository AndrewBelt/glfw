//========================================================================
// Parent window handle example
//========================================================================

#include <glad/gl.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#if 0
    #define GLFW_EXPOSE_NATIVE_X11
#else
    #define GLFW_EXPOSE_NATIVE_WIN32
#endif
#include <GLFW/glfw3native.h>
#if 0
    #include <X11/Xlib.h>
#else
    #include <windows.h>
#endif

#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>


#if 0
Display* display = NULL;

void* createNativeWindow()
{
    display = XOpenDisplay(NULL);
    if (!display)
        exit(EXIT_FAILURE);

    int screen = DefaultScreen(display);
    Window handle = XCreateSimpleWindow(display, RootWindow(display, screen), 0, 0, 400, 600, 20, BlackPixel(display, screen), WhitePixel(display, screen));

    XSelectInput(display, handle, ExposureMask | KeyPressMask);
    XMapWindow(display, handle);

    // Consume events until the window is exposed (shown).
    while (1) {
        XEvent event;
        XNextEvent(display, &event);

        if (event.type == Expose)
            break;
    }

    return (void*) handle;
}

void destroyNativeWindow(void* handle)
{
    XDestroyWindow(display, (Window) handle);
    XCloseDisplay(display);
}
#else
void* createNativeWindow()
{
    HWND handle = CreateWindowExW(0, L"PARENT", L"Parent window", WS_OVERLAPPEDWINDOW, 0, 0, 400, 600, NULL, NULL, GetModuleHandleW(NULL), NULL);
    ShowWindow(handle, SW_SHOWNORMAL);
    return (void*) handle;
}

void destroyNativeWindow(void* handle)
{
    DestroyWindow((HWND) handle);
}
#endif

static void errorCallback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}

int main(void)
{
    // In this scenario, create a foreign window before GLFW is even initialized.
    void* handle = createNativeWindow();

    // Initialize GLFW
    glfwSetErrorCallback(errorCallback);

    if (!glfwInit())
        exit(EXIT_FAILURE);

    glfwWindowHintPointer(GLFW_NATIVE_PARENT_HANDLE, (void*) handle);

    // Create GLFW window from foreign window
    GLFWwindow* window = glfwCreateWindow(400, 600, "Parent window handle", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwSetKeyCallback(window, keyCallback);

    glfwMakeContextCurrent(window);
    gladLoadGL(glfwGetProcAddress);
    glfwSwapInterval(1);

    while (!glfwWindowShouldClose(window))
    {
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        const float ratio = width / (float) height;

        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);

    glfwTerminate();

    // Destroy window after GLFW has terminated
    destroyNativeWindow(handle);
    exit(EXIT_SUCCESS);
}

