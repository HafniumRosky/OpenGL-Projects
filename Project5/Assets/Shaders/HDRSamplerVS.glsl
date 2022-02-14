#version 430

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
	vec3 posL;
}pOut;

void main()
{
	pOut.posL = posL;
	gl_Position = proj * view * vec4(posL, 1.0);
}