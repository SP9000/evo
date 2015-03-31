#version 130
in vec3 in_Position;
in vec3 in_Normal;
in vec3 in_Color;

out vec4 ex_Color;

uniform mat4x4 ModelView, Projection;
void main()
{
   gl_Position = Projection * ModelView * vec4(in_Position, 1.0);
   ex_Color = vec4(in_Normal, max(1.0,in_Color.z));
}
