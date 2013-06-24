#version 130

uniform mat4x4 Model, View, Projection;

in vec3 in_Position;
//in vec2 in_Texco;
//out vec2 ex_Texco;


void main()
{
   //ex_Texco = in_Texco;
   gl_TexCoord[0] = gl_MultiTexCoord0;
   gl_Position = Model * Projection * View * vec4(in_Position, 1.0);
}
