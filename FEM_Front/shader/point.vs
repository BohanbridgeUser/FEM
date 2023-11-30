#version 330
layout(location = 0) in vec4 vertex;
uniform mat4 mvp_matrix;
uniform float point_size;

void main()
{
   gl_PointSize = point_size;
   gl_Position = mvp_matrix * vertex;
}