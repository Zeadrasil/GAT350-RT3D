#version 430

in layout(location = 0) vec3 position;
out layout(location = 0) vec3 otexCoord;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;



void main()
{
	otexCoord = position;
	mat4 vp = projection * mat4(mat3(view));
	gl_Position = vp * vec4(position, 1.0);
}
