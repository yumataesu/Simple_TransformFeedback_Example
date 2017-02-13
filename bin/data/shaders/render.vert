#version 400 core

uniform mat4 modelViewProjectionMatrix;

layout (location = 0) in vec3 position;

//in vec3 color;

//in vec3 color;
//out vec3 vcolor;


void main() {
    gl_Position = modelViewProjectionMatrix * vec4(position, 1.0);
    //vcolor = color;
}
