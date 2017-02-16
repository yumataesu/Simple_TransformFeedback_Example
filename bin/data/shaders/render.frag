#version 400 core

in vec3 v_color;
out vec4 outputColor;

uniform sampler2D tex;

void main() {
    vec4 spliteTex = texture(tex, gl_PointCoord);
    
    outputColor = spliteTex;
}
