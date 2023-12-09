#version 430

#define POINT 0
#define DIRECTIONAL 1
#define SPOT 2

#define ALBEDO_TEXTURE_MASK  (1 << 0)
#define SPECULAR_TEXTURE_MASK  (1 << 1)
#define NORMAL_TEXTURE_MASK  (1 << 2)
#define EMISSIVE_TEXTURE_MASK  (1 << 3)
#define CUBEMAP_TEXTURE_MASK (1 << 4)

in layout(location = 0) vec3 position;
in layout(location = 1) vec3 normal;
in layout(location = 2) vec2 texCoord;
in layout(location = 3) vec4 shadowCoord;
in layout(location = 4) vec3 cubeCoord;

out layout(location = 0) vec4 ocolor;


layout(binding = 0) uniform sampler2D albedoTexture;
layout(binding = 1) uniform sampler2D specularTexture;
layout(binding = 2) uniform sampler2D normalTexture;
layout(binding = 3) uniform sampler2D emissiveTexture;
layout(binding = 5) uniform sampler2D shadowTexture;
layout(binding = 4) uniform samplerCube cubemap;
uniform struct Light
{
	int type;
	vec3 position;
	vec3 albedo;
	vec3 direction;
	float intensity;
	float range;
	float innerAngle;
	float outerAngle;
}lights[6];

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

uniform vec3 ambient = {0.1, 0.1, 0.1};
uniform int numLights = 3;
uniform float shadowBias = 0.05f;
uniform int celBands = 0;


float attenuation(in vec3 position1, in vec3 position2, in float range)
{
	float distanceSqr = dot(position1 - position2, position1 - position2);
	float rangeSqr = pow(range, 2.0);
	float attenuation = max(0, 1 - pow((distanceSqr / rangeSqr), 2.0));
	attenuation = pow(attenuation, 2.0);
 
	return attenuation;
}

float celShading(float cIntensity, int celBands)
{
	return celBands == 0 ? cIntensity : round(cIntensity * celBands) / (celBands * 1.0f);
}

void phong(in Light pLight, in vec3 pPosition, in vec3 pNormal, in float attenuation, out vec3 pDiffuse, out vec3 pSpecular)
{
	
//gets the relative direction of the light based off of light position and model position
	vec3 lightDir = (pLight.type == DIRECTIONAL) ? normalize(-pLight.direction) : normalize(pLight.position - pPosition);
	//if the light is a spot light and the angle is greater than the cutoff, then it sets intensity to 0, else it does nothing
	float spotIntensity = 1;
	if(pLight.type == SPOT)
	{
		float outerAngle = (pLight.outerAngle <= pLight.innerAngle) ? pLight.innerAngle + 0.00001f : pLight.outerAngle;
		float angle = acos(dot(pLight.direction, -lightDir));
		spotIntensity = smoothstep(outerAngle, pLight.innerAngle, angle);
	}
	//finds how much light should be hitting the fragment based off of the relative angles of the fragment face and the light direction, with a minimum of no light
	float intensity = max(dot(lightDir, pNormal), 0) * spotIntensity;
	
	//applies the diffuse lighting to the diffuse material of the model and gets the resulting color
	pDiffuse = pLight.albedo * celShading(intensity * pLight.intensity * attenuation, celBands);

	pSpecular = vec3(0);
	if(intensity > 0)
	{
	//gets the reflection of the light for specular calculations
		vec3 viewDir = normalize(-pPosition);
		vec3 h = normalize(viewDir + lightDir);
		intensity = max(dot(h, pNormal), 0) * spotIntensity * attenuation;
		intensity = pow(intensity, material.shininess);
		pSpecular = vec3(celShading(intensity, celBands));
	}
}

float calculateShadow(vec4 iShadowCoord, float bias)
{
	return texture(shadowTexture, iShadowCoord.xy).x < shadowCoord.z - bias ? 0 : 1;
}


void main()
{
	vec4 albedoColor = bool(material.params & ALBEDO_TEXTURE_MASK) ? texture(albedoTexture, texCoord) : vec4(material.albedo, 1);
	vec4 specularColor = bool(material.params & SPECULAR_TEXTURE_MASK) ? texture(specularTexture, texCoord) : vec4(material.specular, 1);
	vec4 emissiveColor = bool(material.params & EMISSIVE_TEXTURE_MASK) ? texture(emissiveTexture, texCoord) : vec4(material.emissive, 1);
	albedoColor = bool(material.params & CUBEMAP_TEXTURE_MASK) ? texture(cubemap, cubeCoord) : albedoColor;
	ocolor = vec4(ambient, 1) * albedoColor + emissiveColor;
	float shadow = calculateShadow(shadowCoord, shadowBias);
	if(bool(CUBEMAP_TEXTURE_MASK & material.params))
	{
		ocolor = albedoColor;
	}
	else if(shadow > 0)
	{
		for(int i = 0; i < numLights; i++)
		{
			vec3 diffuse;
			vec3 specular;

			float attenuation = (lights[i].type == DIRECTIONAL) ? 1 : attenuation(lights[i].position, position, lights[i].range);

			phong(lights[i], position, normal, attenuation, diffuse, specular);
			ocolor += ((vec4(diffuse, 1) * albedoColor) + (vec4(specular, 1) * specularColor)) * shadow;
		}
	}
}
