#version 130

uniform sampler2D tex;

in  vec2 ex_Texco;
out vec4 out_Color;

void main() 
{
    out_Color = texture(tex, ex_Texco);
}

