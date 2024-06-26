#include <glad.h>
#include <glfw3.h>
#include <stdio.h>

#define true 1
#define false 0

void framebuffer_size_callback(GLFWwindow *window, int w, int h);
void process_input(GLFWwindow *window);

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "yay", NULL, NULL);

    if (window == NULL) {
        printf("Failed to create GLFW Window");
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        printf("Failed to initialize GLAD");
        return -1;
    }

    while (!glfwWindowShouldClose(window)) {
        // input
        process_input(window);

        // rendering commands here
        // ...

        // set specific red/green/blue/alpha values to the color buffer
        glClearColor(0.3f, 0.2f, 0.4f, 1.0f);
        // call functions that uses the state set by the setter before
        glClear(GL_COLOR_BUFFER_BIT);

        glfwPollEvents();
        // instead of directly writing to a buffer on screen
        // gl uses a double-buffer technique, by drawing to a backbuffer in the background
        // and once the drawing is done it swaps the backbuffer and the frontbuffer
        // the stuff that's actually rendered is in the frontbuffer
        // this avoid a weird top-down left-right rendering and instead renders everything at the same time
        glfwSwapBuffers(window);
    }

    glfwTerminate();

    return 0;
}

void framebuffer_size_callback(GLFWwindow *window, int w, int h) {
    glViewport(0, 0, w, h);
}
void process_input(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}