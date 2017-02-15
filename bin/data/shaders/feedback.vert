#version 400 core

uniform float u_time;

layout(location = 0) in vec3 in_position;
layout(location = 1) in vec3 in_accel;
layout(location = 2) in vec3 in_velocity;
layout(location = 3) in float in_lifetime;


out vec3 v_position;
out vec3 v_accel;
out vec3 v_velocity;
out float v_lifetime;


void main() {
    vec3 position = in_position;
    vec3 accel = in_accel;
    vec3 velocity = in_velocity;
    float lifetime = in_lifetime;

    accel = normalize(vec3(40.0 * cos(u_time * 3.0), 5.0*sin(u_time), 90.0 * cos(u_time * 5.0)) - position);
    velocity += accel;
    position += velocity;
    
    
    
    if(lifetime < 0)
    {
        position = vec3(900*sin(u_time), 900*cos(u_time), 0);
        lifetime = 100.0;
    }
    
    lifetime -= 0.5;
    
    v_position = position;
    v_accel = accel;
    v_velocity = velocity;
    v_lifetime = lifetime;
    
}
