#version 330 core

in vec3 our_color;
in vec2 tex_coord;
out vec4 FragColor;
uniform sampler2D our_texture;

void main() {
    FragColor = texture2D(our_texture, tex_coord) * vec4(our_color, 1.0);
}