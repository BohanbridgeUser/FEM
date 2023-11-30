#version 330

in vec4 vertexfP;
in vec3 vertexfN;
noperspective in vec3 GEdgeDistance;

uniform vec4 color;
uniform vec4 light_pos; 
uniform vec4 light_diff;
uniform vec4 light_spec;
uniform vec4 light_amb; 
uniform float spec_power;
uniform float line_width;
uniform vec4  line_color;
uniform int   flag_edge;

out vec4 fragColor;

void main() 
{
    vec3 L = light_pos.xyz - vertexfP.xyz;
    vec3 V = -vertexfP.xyz;
    vec3 N = normalize(vertexfN);
    L = normalize(L);
    V = normalize(V);
    vec3 R = reflect(-L, N);
    vec4 diffuse = abs(dot(N,L)) * light_diff * color;
    vec4 specular = pow(max(dot(R,V), 0.0), spec_power) * light_spec;
    vec4 surface_color = light_amb * color + diffuse + specular ;

    // Find the smallest distance
    if(flag_edge != 0)
    {
        float d = min( GEdgeDistance.x, min(GEdgeDistance.y, GEdgeDistance.z) );
        float mixVal; 
        if( d < line_width - 1 ) {
            mixVal = 1.0;
        } else if( d > line_width + 1 ) {
            mixVal = 0.0;
        } else {
            float x = d - (line_width - 1);
        mixVal = exp2(-2.0 * (x*x));
        }
        fragColor = mix(surface_color, line_color, mixVal);
    } 
    else
    fragColor = surface_color;
}