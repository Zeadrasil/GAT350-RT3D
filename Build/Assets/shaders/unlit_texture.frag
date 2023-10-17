#version 430

in layout(location = 1) vec2 texCoord;

out layout(location = 0) vec4 ocolor;

uniform vec4 color;
uniform vec2 offset;
uniform vec2 tiling;

layout(binding = 0) uniform sampler2D tex;

void main()
{
	vec4 texColor = texture(tex, (texCoord * tiling) + offset).rgba;
	//if(texColor.a < 0.8) discard;
	ocolor = texColor * color;
}
