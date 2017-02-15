#version 400 core


layout(points) in;
layout(points, max_vertices = 1) out;


in vec3 v_position[];
in vec3 v_accel[];
in vec3 v_velocity[];
in float v_lifetime[];


out vec3 o_position;
out vec3 o_accel;
out vec3 o_velocity;
out float o_lifetime;


void main() {
    
    o_position = v_position[0];
    o_accel = v_accel[0];
    o_velocity = v_velocity[0];
    o_lifetime = v_lifetime[0];
    EmitVertex();
    
    
    
    EndPrimitive();
}
