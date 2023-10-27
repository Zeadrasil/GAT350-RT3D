#version 430

in layout(location = 0) vec3 position;
in layout(location = 1) vec2 texCoord;
in layout(location = 2) vec3 normal;

out layout(location = 0) vec3 oposition;
out layout(location = 1) vec3 onormal;
out layout(location = 2) vec2 otexCoord;
out layout(location = 3) vec4 ocolor;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;


uniform struct Light
{
	vec3 ambient;
	vec3 position;
	vec3 diffuse;
}light;

uniform struct Material
{
	vec3 diffuse;
	vec3 specular;
	float shininess;
	vec2 offset;
	vec2 tiling;
} material;

vec3 ads(in vec3 aposition, in vec3 anormal)
{
	vec3 lightDir = normalize(light.position - aposition);
	float intensity = max(dot(lightDir, anormal), 0);
	vec3 diffuse = material.diffuse * (light.diffuse * intensity);

	vec3 specular = vec3(0);
	if(intensity > 0)
	{
		vec3 reflection = reflect(-lightDir, anormal);
		vec3 viewDir = -aposition;
		intensity = max(dot(reflection, viewDir), 0);
		intensity = pow(intensity, material.shininess);
		specular = material.specular * intensity;
	}

	return light.ambient + diffuse + specular;
}

void main()
{
	mat4 modelView = view * model;
	otexCoord = (texCoord * material.tiling) + material.offset;
	oposition = vec3(modelView * vec4(position, 1));
	onormal = normalize(mat3(modelView) * normal);

	ocolor = vec4(ads(position, onormal), 1);

	mat4 mvp = projection * modelView;
	gl_Position = mvp * vec4(oposition, 1);
}
