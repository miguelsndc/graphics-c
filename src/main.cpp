#include <glad.h>
#include <glfw3.h>
#include <math.h>
#include <stdio.h>

#include <filesystem>

#include "shader.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

void framebuffer_size_callback(GLFWwindow *window, int w, int h);
void process_input(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// fix this later lol
const char *vshader_path = "C:\\Users\\Paula\\Documents\\cs\\glshit\\src\\shader.vert";
const char *fshader_path = "C:\\Users\\Paula\\Documents\\cs\\glshit\\src\\shader.frag";
const char *container_image_path = "C:\\Users\\Paula\\Documents\\cs\\glshit\\textures\\container.jpg";

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
        // positions          // colors           // texture coords
        0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,    // top right
        0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,   // bottom right
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,  // bottom left
        -0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f    // top left
    };
    unsigned int indices[] = {
        0,
        1,
        3,
        1,
        2,
        3};

    unsigned int VAO,
        VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);                                         // bind the VBO to gl's target
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);  // bind the

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);                                       // bind the VBO to gl's target
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);  // bind the

    // positions
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    // colors
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // texture coords
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
    // texture coordinates start from (0,0) at the lower left and end at (1,1) at top right

    // float texture_coords[] = {
    //     0.0f, 0.0f,   // left bottom
    //     1.0f, 0.0f,   // right bottom
    //     0.5f, 1.0f};  // center top

    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    // Texture filtering can be set for magnifying and minifying operations (when scaling up or downwards)
    // gl nearest pick's the color of the nearest neighbour texel to the center of the texture coordinate
    // GL_LINEAR: (docs) Returns the weighted average of the texture elements that are closest to the specified texture coordinate
    // so it basically interpolates the colors and find's a color
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
    // mipmaps have no effect on upscaling so setting the mag filter to a mipmap doesn't do anything (GL_INVALID_ENUM ERROR)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // mipmaps are a collection of images where each is twice as small compared to the previous one
    // for efficiency's sake, different mipmaps are applied to objects when a certain distance threshold
    // is met, opengl uses the according mipmap to correctly sample the texels
    int width, height, nr_channels;
    unsigned char *data = stbi_load(container_image_path, &width, &height, &nr_channels, 0);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cout << "FAILED TO LOAD TEXTURE: " << std::endl;
    }

    stbi_image_free(data);

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
        glBindTexture(GL_TEXTURE_2D, texture);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

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
