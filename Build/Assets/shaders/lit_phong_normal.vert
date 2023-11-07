#version 430

in layout(location = 0) vec3 position;
in layout(location = 1) vec2 texCoord;
in layout(location = 2) vec3 normal;
in layout(location = 3) vec3 vtangent;

out layout(location = 0) vec3 oposition;
out layout(location = 1) vec2 otexCoord;
out layout(location = 2) mat3 otbn;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;



uniform struct Material
{
	uint params;
	vec3 albedo;
	vec3 specular;
	float shininess;
	vec3 emissive;
	vec2 offset;
	vec2 tiling;
} material;

void main()
{
	mat4 modelView = view * model;
	otexCoord = (texCoord * material.tiling) * material.offset;
	oposition = vec3(modelView * vec4(position, 1));
	vec3 normal = normalize(mat3(modelView) * normal);
	vec3 tangent = normalize(mat3(modelView) * vtangent);
	vec3 biTangent = cross(normal, tangent);
	otbn = mat3(tangent, biTangent, normal);
	mat4 mvp = projection * modelView;
	gl_Position = mvp * vec4(position, 1);
}
