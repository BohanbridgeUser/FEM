#version 330

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in vec4 fP[];
in vec3 fN[];

out vec4 vertexfP;
out vec3 vertexfN;
noperspective out vec3 GEdgeDistance;

uniform mat4 ViewportMatrix;

void main()
{
   // Transform each vertex into viewport space
   vec3 p0 = vec3(ViewportMatrix * (gl_in[0].gl_Position / gl_in[0].gl_Position.w));
   vec3 p1 = vec3(ViewportMatrix * (gl_in[1].gl_Position / gl_in[1].gl_Position.w));
   vec3 p2 = vec3(ViewportMatrix * (gl_in[2].gl_Position / gl_in[2].gl_Position.w));
   float a = length(p1 - p2);
   float b = length(p2 - p0);
   float c = length(p1 - p0);
   float alpha = acos( (b * b + c * c - a * a) / (2.0 * b * c) );
   float beta = acos( (a * a + c * c - b * b) / (2.0 * a * c) );
   float ha = abs( c * sin( beta ) );
   float hb = abs( c * sin( alpha ) );
   float hc = abs( b * sin( alpha ) );

   gl_Position = gl_in[0].gl_Position;
   vertexfP = fP[0];
   vertexfN = fN[0];
   GEdgeDistance = vec3(ha, 0, 0);
   EmitVertex();

   gl_Position = gl_in[1].gl_Position;
   vertexfP = fP[1];
   vertexfN = fN[1];
   GEdgeDistance = vec3(0, hb, 0);
   EmitVertex();

   gl_Position = gl_in[2].gl_Position;
   vertexfP = fP[2];
   vertexfN = fN[2];
   GEdgeDistance = vec3(0, 0, hc);
   EmitVertex();

   EndPrimitive();
}