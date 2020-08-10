#version 330 core

layout (location = 0) in vec3 obj_spc_vertex_pos;
layout (location = 1) in vec3 obj_spc_N;
layout (location = 2) in vec3 k_d;

uniform mat4 model_mat;
uniform mat4 view_mat;
uniform mat4 proj_mat;

out vec3 I; 

void main() {
    vec3 cam_pos = vec3(0.0f, 0.0f, 1.5f);

    vec3 I_a = vec3(0.2f, 0.2f, 0.2f);
    vec3 k_a = vec3(0.0f, 0.0f, 1.0f);	
    vec3 k_s = vec3(1.0f, 1.0f, 1.0f);	
    vec3 I_p_pos = vec3(-2.0f, 2.0f, 1.5f);
    vec3 I_p = vec3(0.8f, 0.8f, 0.8f);

    I = I_a * k_a;

    gl_Position = proj_mat * view_mat * model_mat * vec4(obj_spc_vertex_pos, 1.0);
}