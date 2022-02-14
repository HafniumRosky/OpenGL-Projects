#version 430

struct DirectionalLight
{
	vec4 color;
	vec3 direction;
};

struct PointLight
{
	vec4 color;
	vec3 position;
	float range;
	vec3 att;
	//pad
};

layout (std140, binding = 3) uniform CBChangesRarely
{
	DirectionalLight dirLight[1];
	//pad
	PointLight pointLight[4];
	//pad
	int numDirLight;
	int numPointLight;
	//pad2
};

in v2f
{
	vec3 texcoord;
}pIn;

uniform samplerCube EnvMap;

out vec4 FragColor;

void main()
{
	FragColor = texture(EnvMap, pIn.texcoord);
}