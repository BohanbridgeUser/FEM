#version 330 
layout(location = 0) in vec4 vertex; 
layout(location = 1) in vec3 normal; 

uniform mat4 mvp_matrix;
uniform mat4 mv_matrix; 
out vec4 fP; 
out vec3 fN; 
void main(void)
{
   fP = mv_matrix * vertex; 
   fN = mat3(mv_matrix) * normal; 
   gl_Position = mvp_matrix * vertex;
}