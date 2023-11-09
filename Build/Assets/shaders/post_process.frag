#version 430

in layout(location = 0) vec2 texCoord;

out layout(location = 0) vec4 ocolor;


layout(binding = 0) uniform sampler2D tex;

void main()
{
	vec4 texColor = texture(tex, texCoord).rgba;
	if(texColor.a < 0.8) discard;
	ocolor = texColor;
}
