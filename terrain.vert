#version 130

in vec4 in_Position;
in vec4 in_Color;
in vec4 in_Flags;

out vec4 ex_Color;

//the location that is the focus of the rendering
uniform vec3 focus;
uniform mat4x4 ModelView, Projection;

//uniform vec4 Lights[2*10];
//uniform float NumLights;

void main()
{
	float NdotL;
	vec4 col = in_Color;
	if(in_Flags.x > focus.y) {
		gl_Position = vec4(-1000,-1000,-1000,-1000);
	}
	else {
		gl_Position = Projection * ModelView * in_Position;
	}
	ex_Color = col;
}
