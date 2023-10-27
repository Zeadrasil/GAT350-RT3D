#version 430

in layout(location = 0) vec3 position;
in layout(location = 1) vec3 normal;
in layout(location = 2) vec2 texCoord;
flat in layout(location = 3) vec4 icolor;

out layout(location = 0) vec4 ocolor;


layout(binding = 0) uniform sampler2D tex;

void main()
{
	vec4 texColor = texture(tex,texCoord);
	ocolor = texColor * icolor;
}
