#version 430
#define POINT 0
#define DIRECTIONAL 1
#define SPOT 2
in layout(location = 0) vec3 texCoord;
in layout(location = 1) vec3 normal;
out layout(location = 0) vec4 ocolor;
out layout(location = 1) vec3 onormal;



layout(binding = 0) uniform samplerCube cubemap;
void main()
{
	ocolor = texture(cubemap, texCoord);
	onormal = normal;
}
