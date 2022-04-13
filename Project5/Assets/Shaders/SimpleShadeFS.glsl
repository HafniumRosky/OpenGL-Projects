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

out vec4 FragColor;

void main()
{
	FragColor = material.diffuse;
}