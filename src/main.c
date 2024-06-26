#include <glad.h>
#include <glfw3.h>
#include <stdio.h>

#define true 1
#define false 0

void framebuffer_size_callback(GLFWwindow *window, int w, int h);
void process_input(GLFWwindow *window);
int handle_shader_compile_errors(unsigned int id, const char *name, unsigned int buf_size);
int handle_program_link_errors(unsigned int id, const char *name, unsigned int buf_size);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
const char *vertexShaderSource =
    "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";

const char *fragmentShaderSource =
    "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main () \n"
    "{\n"
    "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\0";

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

    // vertex shader
    // --------------------------------------------
    unsigned int vertex_shader = glCreateShader(GL_VERTEX_SHADER);  // reference the shader by and id
    glShaderSource(vertex_shader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertex_shader);
    if (handle_shader_compile_errors(vertex_shader, "VERTEX", 512) < 0) {
        // shit happened
    }
    // fragment shader
    // --------------------------------------------
    unsigned int fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);  // reference the shader by and id
    glShaderSource(fragment_shader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragment_shader);
    if (handle_shader_compile_errors(fragment_shader, "FRAGMENT", 512) < 0) {
        // shit happend
    }
    // link shaders
    // --------------------------------------------
    unsigned int shader_program = glCreateProgram();
    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, fragment_shader);
    glLinkProgram(shader_program);
    if (handle_program_link_errors(shader_program, "SHADER", 512) < 0) {
        // shit happened
    }
    glDeleteShader(vertex_shader);  // we no longer need the shaders after succesful programing linkage
    glDeleteShader(fragment_shader);

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ---------------------------------------------
    float vertices[] = {
        -0.5f, -0.5f, 0.0f,  // left
        0.5f, -0.5f, 0.0f,   // right
        0.0f, 0.5f, 0.0f};   // top
    unsigned int VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    // bind the vertex array object first, then bind the vertex buffer object, then configure everything
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);                                         // bind the VBO to gl's target
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);  // bind the

    // pos, size, type, normalized, stride, some weird shit
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

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
        glClearColor(0.3f, 0.2f, 0.4f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // draw the thing
        glUseProgram(shader_program);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // instead of directly writing to a buffer on screen
        // gl uses a double-buffer technique, by drawing to a backbuffer in the background
        // and once the drawing is done it swaps the backbuffer and the frontbuffer
        // the stuff that's actually rendered is in the frontbuffer
        // this avoid a weird top-down left-right rendering and instead renders everything at the same time
        // swap buffers and poll IO events (mouse, keyboards, etc...)
        // --------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // unbind and terminate glfw
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shader_program);
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

int handle_shader_compile_errors(unsigned int id, const char *name, unsigned int buf_size) {
    int error = 1;
    int success;
    char info_log[buf_size];
    glGetShaderiv(id, GL_COMPILE_STATUS, &success);

    if (!success) {
        glGetShaderInfoLog(id, buf_size, NULL, info_log);
        printf("ERROR::SHADER::%s::COMPILATION_FAILED\n %s", name, info_log);
        error = -1;
    }
    return error;
}

int handle_program_link_errors(unsigned int id, const char *name, unsigned int buf_size) {
    int error = 1;
    int success;
    char info_log[buf_size];
    glGetProgramiv(id, GL_COMPILE_STATUS, &success);

    if (!success) {
        glGetProgramInfoLog(id, buf_size, NULL, info_log);
        printf("ERROR::PROGRAM::%s::LINKING_FAILED\n %s", name, info_log);
        error = -1;
    }

    return error;
}
