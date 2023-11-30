#version 330

in vec4 vertex;
in vec4 color;

uniform mat4 mvp_matrix;
uniform float point_size;

out vec4 outColor;

void main()
{
   gl_PointSize = point_size;
   gl_Position = mvp_matrix * vertex;
   outColor = color;
}