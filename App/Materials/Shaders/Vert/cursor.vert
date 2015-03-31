#version 130
in vec4 in_Position;
in vec4 in_Color;
out vec4 ex_Color;

uniform mat4x4 ModelView, Projection;

void main()
{
	gl_Position = Projection * ModelView * in_Position;
	ex_Color = in_Color;
}