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

out v2f
{
	vec3 posW;
	vec3 normalW;
}pOut;

void main()
{
	pOut.posW = vec3(world * vec4(posL,1.0));
	pOut.normalW = vec3(worldInvTranspose * vec4(normalL, 1.0));
	gl_Position = proj * view * vec4(pOut.posW,1.0);
}