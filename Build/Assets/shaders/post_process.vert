#version 430

in layout(location = 0) vec3 position;
in layout(location = 1) vec2 texCoord;

out layout(location = 0) vec2 otexCoord;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;

void main()
{
	otexCoord = texCoord;
	gl_Position =vec4(position, 1.0);
}
