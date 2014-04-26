#version 130

uniform mat4x4 Model, View, Projection;

in vec3 in_Position;
in vec3 in_Texco;
out vec2 ex_Texco;

void main()
{
   ex_Texco = in_Texco.xy;
   gl_Position = Model * Projection * View * vec4(in_Position, 1.0);
}
