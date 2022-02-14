#version 430

struct Material
{
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
	float gloss;
};

layout (std140, binding = 0) uniform CBChangeEveryDrawing
{
	mat4 world;
	mat4 worldInvTranspose;
	Material material;
	//pad * 3
};

layout (std140, binding = 1) uniform CBChangesEveryFrame
{
	mat4 view;
	mat4 shadowMatrix;
	vec3 eyePos;
	//pad
};

layout (std140, binding = 2) uniform CBChangesOnResize
{
	mat4 proj;
};

in vec3 posL;
in vec3 normalL;

void main()
{
	vec4 posV = view * world * vec4(posL, 1.0);
	vec3 normalV = vec3(transpose(inverse(view * world))  * vec4(normalL, 1.0));
	normalV.z = -0.5;
	posV = posV + vec4(normalize(normalV), 0.0) * 0.016;
	gl_Position = proj * posV;
}