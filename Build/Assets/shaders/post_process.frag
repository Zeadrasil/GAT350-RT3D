#version 430
#define INVERT_MASK 1<<0
#define GRAYSCALE_MASK 1<<1
#define COLOR_TINT_MASK 1<<2
#define GRAIN_MASK 1 << 3
#define SCANLINE_MASK 1 << 4
#define MIRROR_MASK 1 << 5


in layout(location = 0) vec2 texCoord;

out layout(location = 0) vec4 ocolor;

uniform float blend = 1;
uniform uint params = 0;
uniform vec4 tintColor = vec4(0);
uniform float height = 100;
layout(binding = 0) uniform sampler2D tex;

vec4 invert(in vec4 color)
{
	return vec4(vec3(1) - color.rgb, color.a);
}

vec4 grayscale(in vec4 color)
{
	return vec4(vec3((color.r + color.g + color.b) / 3), color.a);
}
vec4 grain(in vec4 color)
{
	return vec4(color.rgb - ((fract(sin(dot(texCoord, vec2(12.98,78.3)*2.0)) * 437548.3)) * 0.15), color.a);
}
vec4 tint(in vec4 color)
{
	return color * tintColor;
}

vec4 scanline(in vec4 color)
{
	return vec4(color.rgb - mod(texCoord.y * height * 100, 1.5), color.a);
}

void main()
{
	vec4 baseColor = (bool(params & MIRROR_MASK)) ? texture(tex, vec2(texCoord.x * -1, texCoord.y)) : texture(tex, texCoord).rgba;
	vec4 postProcess = baseColor;
	if(bool(params & INVERT_MASK))
	{
		postProcess = invert(postProcess);
	}
	if(bool(params & GRAYSCALE_MASK))
	{
		postProcess = grayscale(postProcess);
	}
	if(bool(params & COLOR_TINT_MASK))
	{
		postProcess = tint(postProcess);
	}
	if(bool(params & GRAIN_MASK))
	{
		postProcess = grain(postProcess);
	}
	if(bool(params & SCANLINE_MASK))
	{
		postProcess = scanline(postProcess);
	}
	//if(texColor.a < 0.8) discard;
	ocolor = mix(baseColor, postProcess, blend);
}

