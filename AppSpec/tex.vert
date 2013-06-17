#version 130
in vec3 in_Position;
in vec4 in_Color;
uniform mat4x4 Model, View, Projection;
void main()
{
   gl_Position = Model * Projection * View * vec4(in_Position, 1.0);
}
