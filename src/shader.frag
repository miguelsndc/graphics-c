#version 330 core

in vec3 our_color;
in vec2 tex_coord;
out vec4 FragColor;

uniform sampler2D texture1;
uniform sampler2D texture2;
uniform float mix_value;

void main() {
    // mix(x, y, a) interpolates x and y with some weight a between 0 and 1
    // calculated as follows: mix(x,y,a) = x(1 - a) + y * a
    FragColor = mix(texture2D(texture1, tex_coord), texture2D(texture2, vec2(1.0 - tex_coord.x, tex_coord.y)), mix_value);
}