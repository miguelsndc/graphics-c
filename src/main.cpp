#include <glad.h>
#include <glfw3.h>
#include <math.h>
#include <stdio.h>

#include <filesystem>

#include "shader.hpp"

void framebuffer_size_callback(GLFWwindow *window, int w, int h);
void process_input(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// fix this later lol
const char *vshader_path = "C:\\Users\\Paula\\Documents\\cs\\glshit\\src\\shader.vert";
const char *fshader_path = "C:\\Users\\Paula\\Documents\\cs\\glshit\\src\\shader.frag";

int main() {
    // glfw initialize and configure
    // ---------------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // window creation
    // ---------------------------------------
    GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "yay", NULL, NULL);
    if (window == NULL) {
        printf("Failed to create GLFW Window");
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad load opengl function pointers
    // -----------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        printf("Failed to initialize GLAD");
        return -1;
    }

    Shader sh(vshader_path, fshader_path);
    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ---------------------------------------------
    float vertices[] = {
        0.0f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
        -0.5, -0.5f, 0.0f, 0.0, 1.0f, 0.0f,
        0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f};

    unsigned int VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);                                         // bind the VBO to gl's target
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);  // bind the
    // positions
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    // colors
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // render loop
    // ------------------------------------------
    while (!glfwWindowShouldClose(window)) {
        // media input
        // -------------------------------------
        process_input(window);

        // render
        // ------------------------------------
        // set specific red/green/blue/alpha values to the color buffer
        // call functions that uses the state set by the setter before
        glClearColor(0.05f, 0.05f, 0.03f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // draw the thing

        sh.use();
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // unbind and terminate glfw
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    sh.destroy();
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
