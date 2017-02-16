#version 400 core

uniform mat4 modelViewProjectionMatrix;

layout (location = 0) in vec3 in_position;
layout (location = 4) in vec3 in_color;

out vec3 v_color;

void main() {
    gl_Position = modelViewProjectionMatrix * vec4(in_position, 1.0);
    v_color = in_color;
    gl_PointSize = 4.0;
}
