#version 430
#define POINT 0
#define DIRECTIONAL 1
#define SPOT 2
in layout(location = 0) vec3 position;
in layout(location = 1) vec3 normal;
in layout(location = 2) vec2 texCoord;

out layout(location = 0) vec4 ocolor;


layout(binding = 0) uniform sampler2D tex;

uniform struct Light
{
	int type;
	vec3 ambient;
	vec3 position;
	vec3 diffuse;
	vec3 direction;
	float intensity;
	float range;
	float innerAngle;
	float outerAngle;
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

	float attenuation = (light.type != DIRECTIONAL) ? pow(max(0, 1 - pow(light.range*light.range / dot(light.position - position, light.position - position), 2.0)), 2.0) : 1;
//gets the relative direction of the light based off of light position and model position
	vec3 lightDir = (light.type == DIRECTIONAL) ? normalize(-light.direction) : normalize(light.position - aposition);
	//finds how much light should be hitting the fragment based off of the relative angles of the fragment face and the light direction, with a minimum of no light
	float intensity = max(dot(lightDir, anormal), 0);
	//if the light is a spot light and the angle is greater than the cutoff, then it sets intensity to 0, else it does nothing
	float outerAngle = (light.outerAngle <= light.innerAngle) ? light.innerAngle + 0.00001f : light.outerAngle;
	float angle = acos(dot(light.direction, -lightDir));
	float spotIntensity = (light.type == SPOT && angle > outerAngle) ? 0 : (light.type == SPOT && angle > light.innerAngle) ? smoothstep(outerAngle, light.innerAngle, angle) : 1;
	
	//applies the diffuse lighting to the diffuse material of the model and gets the resulting color
	vec3 diffuse = material.diffuse * (light.diffuse * intensity * spotIntensity * light.intensity * attenuation);

	vec3 specular = vec3(0);
	if(intensity > 0)
	{
	//gets the reflection of the light for specular calculations
		vec3 reflection = reflect(-lightDir, anormal);
		//gets which direction the screen is by inverting position
		vec3 viewDir = -aposition;
		//finds how much light should be reaching the observer based off of how the light would reflect and the direction to the observer, with a minimum of none
		intensity = max(dot(reflection, viewDir), 0);
		//modifies intensity so that shinier things have less of a spread
		intensity = pow(intensity, material.shininess);
		//applies the specular map of the material multiplied by the intensity
		specular = material.specular * (intensity * spotIntensity * light.intensity * attenuation);
	}
	//returns all lighting combined into one form
	return light.ambient + diffuse + specular;
}

void main()
{
	vec4 texColor = texture(tex,(texCoord * material.tiling) + material.offset);
	vec4 color = vec4(ads(position, normal), 1);
	ocolor = texColor * color;
}
