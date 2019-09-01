//========================================================================
// Parent window handle test
//========================================================================

#include <glad/gl.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>


void* createNativeWindow();
void destroyNativeWindow(void* handle);


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
    if (!handle)
        exit(EXIT_FAILURE);

    // Initialize GLFW
    glfwSetErrorCallback(errorCallback);

    if (!glfwInit())
        exit(EXIT_FAILURE);

    glfwWindowHintPointer(GLFW_NATIVE_PARENT_HANDLE, (void*) handle);

    // Create GLFW window from foreign window
    GLFWwindow* window = glfwCreateWindow(600, 400, "Parent window handle", NULL, NULL);
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
        glfwPollEvents();

        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        const float ratio = width / (float) height;

        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT);

        glfwSwapBuffers(window);
    }

    glfwDestroyWindow(window);

    glfwTerminate();

    // Destroy window after GLFW has terminated
    destroyNativeWindow(handle);
    exit(EXIT_SUCCESS);
}

