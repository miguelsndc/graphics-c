#version 330 core

in vec3 our_color;
out vec4 FragColor;

void main() {
    FragColor = vec4(our_color, 1.0);
}