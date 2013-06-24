#version 130

uniform sampler2D tex;

//in  vec2 ex_Texco;
//layout(location = 0) 
out vec4 out_Color;

void main() 
{
    //out_Color = texture2D(tex, ex_Texco);
    out_Color = texture2D(tex, gl_TexCoord[0].st) + vec4(1.0f,0.0f,0.0f,0.0f);
}

