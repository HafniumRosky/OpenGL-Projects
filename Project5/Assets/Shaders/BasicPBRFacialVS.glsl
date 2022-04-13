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

in vec3 posL0;
in vec3 posL1;
in vec3 posL2;
in vec3 posL3;
in vec3 posL4;
in vec3 posL5;
in vec3 posL6;
in vec3 posL7;
in vec3 posL8;
in vec3 posL9;
in vec3 posL10;
in vec3 normalL;
in vec2 texcoord;

uniform float blendWeights[11];

out v2f
{
	vec3 posW;
	vec3 normalW;
	vec2 uv;
}pOut;

void main()
{
	float neutralWeight = 1.0;
	float sumWeight = 0.0;
	for(int i = 0; i < 10; i++)
	{
		neutralWeight -= blendWeights[i];
		sumWeight += blendWeights[i];
	}
	clamp(neutralWeight, 0.0, 1.0);
	sumWeight += neutralWeight;

	vec3 posL = vec3(0.0);
	posL += posL0 * blendWeights[0] / sumWeight;
	posL += posL1 * blendWeights[1] / sumWeight;
	posL += posL2 * blendWeights[2] / sumWeight;
	posL += posL3 * blendWeights[3] / sumWeight;
	posL += posL4 * blendWeights[4] / sumWeight;
	posL += posL5 * blendWeights[5] / sumWeight;
	posL += posL6 * blendWeights[6] / sumWeight;
	posL += posL7 * blendWeights[7] / sumWeight;
	posL += posL8 * blendWeights[8] / sumWeight;
	posL += posL9 * blendWeights[9] / sumWeight;
	posL += posL10 * neutralWeight / sumWeight;

	pOut.posW = vec3(world * vec4(posL, 1.0));
	if(!renderDepth)
	{
		pOut.normalW = vec3(worldInvTranspose * vec4(normalL, 1.0));
		pOut.uv = texcoord;
	}

	gl_Position = proj * view * vec4(pOut.posW, 1.0);
}