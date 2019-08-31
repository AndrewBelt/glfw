//========================================================================
// Parent window handle example
//========================================================================

#include <glad/gl.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_X11
#include <GLFW/glfw3native.h>
#include <X11/Xlib.h>

#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>


Display* display = NULL;

Window createForeignWindow()
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

    return handle;
}

void destroyForeignWindow(Window handle)
{
    XDestroyWindow(display, handle);
    XCloseDisplay(display);
}

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
    Window handle = createForeignWindow();

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
    destroyForeignWindow(handle);
    exit(EXIT_SUCCESS);
}

