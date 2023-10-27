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
	vec3 position;
	vec3 diffuse;
	vec3 direction;
	float intensity;
	float range;
	float innerAngle;
	float outerAngle;
}lights[3];

uniform struct Material
{
	vec3 diffuse;
	vec3 specular;
	float shininess;
	vec2 offset;
	vec2 tiling;
} material;

uniform vec3 ambient = {0.1, 0.1, 0.1};
uniform int numLights = 3;


float attenuation(in vec3 position1, in vec3 position2, in float range)
{
	float distanceSqr = dot(position1 - position2, position1 - position2);
	float rangeSqr = pow(range, 2.0);
	float attenuation = max(0, 1 - pow((distanceSqr / rangeSqr), 2.0));
	attenuation = pow(attenuation, 2.0);
 
	return attenuation;
}

void phong(in Light pLight, in vec3 pPosition, in vec3 pNormal, out vec3 pDiffuse, out vec3 pSpecular)
{
	float attenuation = (pLight.type != DIRECTIONAL) ? attenuation(pLight.position, pPosition, pLight.range) : 1;
//gets the relative direction of the light based off of light position and model position
	vec3 lightDir = (pLight.type == DIRECTIONAL) ? normalize(-pLight.direction) : normalize(pLight.position - pPosition);
	//finds how much light should be hitting the fragment based off of the relative angles of the fragment face and the light direction, with a minimum of no light
	float intensity = max(dot(lightDir, pNormal), 0);
	//if the light is a spot light and the angle is greater than the cutoff, then it sets intensity to 0, else it does nothing
	float outerAngle = (pLight.outerAngle <= pLight.innerAngle) ? pLight.innerAngle + 0.00001f : pLight.outerAngle;
	float angle = acos(dot(pLight.direction, -lightDir));
	float spotIntensity = 1;
	if(pLight.type == SPOT)
	{
		if(angle > outerAngle) spotIntensity = 0;
		else if(angle > pLight.innerAngle) spotIntensity = smoothstep(outerAngle, pLight.innerAngle, angle);
	}
	
	//applies the diffuse lighting to the diffuse material of the model and gets the resulting color
	pDiffuse = material.diffuse * (pLight.diffuse * intensity * spotIntensity * pLight.intensity * attenuation);

	pSpecular = vec3(0);
	if(intensity > 0)
	{
	//gets the reflection of the light for specular calculations
		vec3 reflection = reflect(-lightDir, pNormal);
		//gets which direction the screen is by inverting position
		vec3 viewDir = -pPosition;
		//finds how much light should be reaching the observer based off of how the light would reflect and the direction to the observer, with a minimum of none
		intensity = max(dot(reflection, viewDir), 0);
		//modifies intensity so that shinier things have less of a spread
		intensity = pow(intensity, material.shininess);
		//applies the specular map of the material multiplied by the intensity
		pSpecular = material.specular * (intensity * spotIntensity * pLight.intensity * attenuation);
	}
}
/*
vec3 ads(in vec3 aposition, in vec3 anormal, in int lightNumber)
{

	float attenuation = (light.type != DIRECTIONAL) ? pow(max(0, 1 - pow(light.range*lights[lightNumber].range / dot(light.position - position, light.position - position), 2.0)), 2.0) : 1;
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
*/
void main()
{


	vec4 texColor = texture(tex,(texCoord * material.tiling) + material.offset);
	ocolor = vec4(ambient, 1) * texColor;
	for(int i = 0; i < numLights; i++)
	{
		vec3 diffuse;
		vec3 specular;
		phong(lights[i], position, normal, diffuse, specular);
		ocolor += vec4(diffuse, 1) + vec4(specular, 1);
	}
}
