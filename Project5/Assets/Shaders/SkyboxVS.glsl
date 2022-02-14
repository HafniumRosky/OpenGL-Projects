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

out v2f
{
	vec3 texcoord;
}pOut;

void main()
{
	vec3 posW = vec3(world * vec4(posL,1.0));
	pOut.texcoord = normalize(posL.xyz);
	gl_Position = proj * view * vec4(posW,1.0);
}