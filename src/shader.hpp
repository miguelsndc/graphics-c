#ifndef SHADER_H
#define SHADER_H

#include <glad.h>
#include <glfw3.h>
#include <stdio.h>

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

class Shader {
   public:
    unsigned int ID;
    // reads and builds shaders
    Shader(const char* vertex_path, const char* fragment_path) {
        std::string vertex_code;
        std::string fragment_code;
        std::ifstream vshader_file;
        std::ifstream fshader_file;
        // ensure ifstream objects can throw exceptions:
        vshader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        fshader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        try {
            // open files
            vshader_file.open(vertex_path);
            fshader_file.open(fragment_path);
            std::stringstream vShaderStream, fShaderStream;
            // read file's buffer contents into streams
            vShaderStream << vshader_file.rdbuf();
            fShaderStream << fshader_file.rdbuf();
            // close file handlers
            vshader_file.close();
            fshader_file.close();
            // convert stream into string
            vertex_code = vShaderStream.str();
            fragment_code = fShaderStream.str();
        } catch (std::ifstream::failure& e) {
            std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: " << e.what() << std::endl;
        }

        const char* vshader_code = vertex_code.c_str();
        const char* fshader_code = fragment_code.c_str();

        // 2. compile shaders
        unsigned int vertex, fragment;

        vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &vshader_code, NULL);
        glCompileShader(vertex);
        check_compilation_errors(vertex, "VERTEX");

        fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &fshader_code, NULL);
        glCompileShader(fragment);
        check_compilation_errors(fragment, "FRAGMENT");

        ID = glCreateProgram();
        glAttachShader(ID, vertex);
        glAttachShader(ID, fragment);
        glLinkProgram(ID);
        check_compilation_errors(ID, "PROGRAM");

        // shaders are already bound to our program and are no longer of use
        glDeleteShader(vertex);
        glDeleteShader(fragment);
    }

    // use/activate the shader
    void use() {
        glUseProgram(ID);
    }

    void set_bool(const std::string& name, bool value) const {
        glUniform1i(loc(name), (int)value);
    }
    void set_int(const std::string& name, int value) const {
        glUniform1i(loc(name), value);
    }
    void set_float(const std::string& name, float value) const {
        glUniform1f(loc(name), value);
    }

    int loc(const std::string& name) const {
        return glGetUniformLocation(ID, name.c_str());
    }

    void destroy() {
        glDeleteProgram(ID);
    }

   private:
    void check_compilation_errors(unsigned int id, std::string const& type) {
        int success;
        unsigned int BUF_SIZE = 1024;

        if (type != "PROGRAM") {
            glGetShaderiv(id, GL_COMPILE_STATUS, &success);

            if (!success) {
                char info_log[BUF_SIZE];
                glGetShaderInfoLog(id, BUF_SIZE, NULL, info_log);
                std::cout << "ERROR::SHADER::" << type << "::COMPILATION_FAILED" << info_log << "\n ---------------------------------------------------------------------------";
            }
        } else {
            glGetProgramiv(ID, GL_LINK_STATUS, &success);

            if (!success) {
                char info_log[BUF_SIZE];
                glGetProgramInfoLog(ID, BUF_SIZE, NULL, info_log);
                std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED" << info_log << "\n ---------------------------------------------------------------------------";
            }
        }
    };
};
#endif