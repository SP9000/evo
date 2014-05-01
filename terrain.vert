#version 130
in vec3 in_Position;
in vec4 in_Color;

out vec4 ex_Color;

//the location that is the focus of the rendering
uniform vec3 focus;

uniform mat4x4 Model, View, Projection;
void main()
{
	vec4 col = in_Color;
	gl_Position = Model * Projection * View * vec4(in_Position, 1.0);
	if(distance(focus.y, in_Position.y) < 0.25) {
		col.a = 1.0;
	}
	else {
		col.a = 0.1;
	}
	col.a = 1.0;
	ex_Color = col;
}
