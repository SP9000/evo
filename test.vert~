#version 130
in vec3 in_Position;
in vec4 in_Color;
in vec3 in_Normal;
out vec4 ex_Color;
uniform mat4x4 Model, View, Projection;
void main()
{
   gl_Position = Model * Projection * View * vec4(in_Position, 1.0);
   ex_Color = in_Color - vec4(in_Normal,1.0);
}
