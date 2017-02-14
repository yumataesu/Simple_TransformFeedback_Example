#version 400 core

uniform mat4 modelViewProjectionMatrix;

layout (location = 0) in vec3 in_position;


void main() {
    gl_Position = modelViewProjectionMatrix * vec4(in_position, 1.0);
}
