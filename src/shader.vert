#version 330 core
layout(location = 0) in vec3 aPos;
out vec4 vertex_color;

void main() {
    gl_Position = vec4(aPos, 1.0);
    vertex_color = vec4(0.97, 0.0, 0.64, 1.0);
}