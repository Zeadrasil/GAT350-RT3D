#version 430

in layout(location = 1) vec2 texCoord;

out layout(location = 0) vec4 ocolor;

uniform vec4 color;
uniform vec2 offset;
uniform vec2 tiling;

layout(binding = 0) uniform sampler2D tex1;
layout(binding = 1) uniform sampler2D tex2;

void main()
{
	vec4 texColor1 = texture(tex1, (texCoord * tiling) + offset).rgba;
	vec4 texColor2 = texture(tex2, (texCoord * tiling) + offset).rgba;
	vec4 texColor = mix(texColor1, texColor2, 0.1);
	//if(texColor.a < 0.8) discard;
	ocolor = texColor * color;
}
