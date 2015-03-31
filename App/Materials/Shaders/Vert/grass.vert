#version 140

const int NUM_BLADES = 100000

in vec3 in_Position;
in vec3 in_Color;
out vec4 ex_Color;

uniform mat4x4 ModelView, Projection;

layout (std140) uniform locArray {
	uniform vec2 Blades[NUM_BLADES];
};

void main()
{
   gl_Position = Projection * ModelView * vec4(Blades[glInstanceID], 1.0, 1.0);
   ex_Color = vec4(in_Normal, max(1.0,in_Color.z));
}
