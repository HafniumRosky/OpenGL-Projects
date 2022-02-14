#version 430

struct PBRMaterial
{
	vec3  albedo;
	float metallic;
	float roughness;
	float ao;
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

layout (std140, binding = 4) uniform CBRenderState
{
	bool renderDepth;
};

layout (std140, binding = 5) uniform CBChangeEveryDrawingPBR
{
	mat4 world;
	mat4 worldInvTranspose;
	PBRMaterial material;
	//pad * 2
};

in vec3 posL;
in vec3 normalL;
in vec4 color;

uniform float eta = 0.8;

out v2f
{
	vec3 posW;
	vec3 normalW;
}pOut;

void main()
{
	pOut.posW = vec3(world * vec4(posL,1.0));
	if(!renderDepth)
	{
		vec3 viewDir = eyePos - pOut.posW;
		pOut.normalW = normalize(vec3(worldInvTranspose * vec4(normalL, 1.0)));
	}
	gl_Position = proj * view * vec4(pOut.posW,1.0);
}