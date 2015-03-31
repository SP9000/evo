#version 130
in vec4 in_Position;
in vec3 in_Normal;
in vec2 in_TexCo;
in vec2 in_Index;
in vec2 in_Weight;

out vec2 out_TexCo;
out vec3 out_EyeNormal;

uniform vec4 BoneOffsets[10];
uniform vec4 BoneRotations[10];
uniform mat4x4 Modelview, ProjectionModelview;

void main()
{
	vec4 newVertex;
	vec4 newNormal;
	int index;

	index=int(in_Index.x);    //Cast to int
	newVertex = (Bone[index] * in_Position) * in_Weight.x;
	newNormal = (Bone[index] * vec4(in_Normal, 0.0)) * in_Weight.x;
	index=int(in_Index.y);    //Cast to int
	newVertex = (Bone[index] * in_Position) * in_Weight.y + newVertex;
	newNormal = (Bone[index] * vec4(in_Normal, 0.0)) * in_Weight.y + newNormal;

	out_EyeNormal = vec3(Modelview * newNormal);
	gl_Position = ProjectionModelview * newVertex;
	out_TexCo = in_TexCo;
}
