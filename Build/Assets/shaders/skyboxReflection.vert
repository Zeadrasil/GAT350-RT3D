#version 430

in layout(location = 0) vec3 position;
in layout(location = 1) vec3 normal;
out layout(location = 0) vec3 otexCoord;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;

vec3 vreflect(vec3 i, vec3 n)
{
	return i - (n * dot(i, n) * 2);
}

void main()
{
	//transform vertex position and normal to world space
	vec3 mposition = vec3(model * vec4(position, 1));
	vec3 mnormal = normalize(vec3(mat3(model) * normal));

	//inverse of view space -> world space
	//last column of mat4 is position
	vec3 viewPosition = inverse(view)[3].xyz;
	vec3 viewDir = normalize(mposition - viewPosition);

	//reflect view direction about vector normal
	otexCoord = vreflect(viewDir, mnormal);
	mat4 vp = projection * mat4(mat3(view)) * model;
	gl_Position = vp * vec4(position, 1.0);
}
