#version 430

in layout(location = 0) vec2 texCoord;

out layout(location = 0) vec4 ocolor;

uniform struct Material
{
	vec3 albedo;
	vec2 offset;
	vec2 tiling;
} material;
layout(binding = 0) uniform sampler2D tex;

void main()
{
	vec4 texColor = texture(tex, texCoord).rgba;
	float depth = texColor.r;

	if(texColor.a < 0.8) discard;
	ocolor = texColor * vec4(material.albedo, 1);
}
