#version 330

out vec4 fragColor;
in vec4 outColor;

void main(void)
{
   fragColor = outColor;
}