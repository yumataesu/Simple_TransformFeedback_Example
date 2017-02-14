#version 400 core

uniform float u_time;

layout(location = 0) in vec3 in_position;
layout(location = 1) in vec3 in_accel;
layout(location = 2) in vec3 in_velocity;


out vec3 o_position;
out vec3 o_accel;
out vec3 o_velocity;


void main() {
    vec3 position = in_position;
    vec3 accel = in_accel;
    vec3 velocity = in_velocity;

    accel = normalize(vec3(40.0 * cos(u_time * 3.0), 10.0*sin(u_time), 40.0 * cos(u_time * 5.0)) - position);
    velocity += accel;
    position += velocity;
    
    
    o_position = position;
    o_accel = accel;
    o_velocity = velocity;
}
