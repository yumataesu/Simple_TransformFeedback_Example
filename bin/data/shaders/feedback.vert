#version 400 core

uniform float u_time;

in vec3 position;
out vec3 o_position;


void main() {
    o_position = position + vec3(sin(u_time*10), 0.0f, 0.0f);
}
