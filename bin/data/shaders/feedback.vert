#version 400 core

in vec3 position;
out vec3 o_position;


void main() {
    o_position = position + vec3(1.0f, 0.0f, 0.0f);
}
