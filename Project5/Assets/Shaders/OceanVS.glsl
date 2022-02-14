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

layout (std140, binding = 4) uniform CBRenderState
{
	bool renderDepth;
};

in vec3 posL;
in vec3 normalL;
in vec2 texcoord;

out vec3 posW_HS;
out vec3 normalW_HS;
out vec2 uv_HS;

void main()
{
	posW_HS = vec3(world * vec4(posL,1.0));
	normalW_HS = vec3(worldInvTranspose * vec4(normalL, 1.0));
	if(!renderDepth)
	{
		uv_HS = texcoord;
	}
}