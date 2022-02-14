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
in vec4 tangent;
in vec2 texcoord;

out v2f
{
	vec4 uv;
	vec4 TtoW0;
	vec4 TtoW1;
	vec4 TtoW2;
}pOut;

void main()
{
	vec3 posW = vec3(world * vec4(posL,1.0));
	pOut.uv.xy = texcoord;
	pOut.uv.zw = texcoord;

	vec3 normalW = vec3(worldInvTranspose * vec4(normalL, 1.0));
	vec3 tagentW = vec3(world * vec4(tangent.xyz, 1.0));
	vec3 biNormalW = cross(normalW, tagentW) * tangent.w;

	// Compute the matrix that transform directions from tangent space to world space
	// Put the world position in w component for optimization
	pOut.TtoW0 = vec4(tagentW.x, biNormalW.x, normalW.x, posW.x);
	pOut.TtoW1 = vec4(tagentW.y, biNormalW.y, normalW.y, posW.y);
	pOut.TtoW2 = vec4(tagentW.z, biNormalW.z, normalW.z, posW.z);

	gl_Position = proj * view * vec4(posW,1.0);
}