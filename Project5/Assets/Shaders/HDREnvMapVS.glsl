#version 430

in vec3 posL;

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

out v2f
{
	vec3 texcoord;
}pOut;

void main()
{
    pOut.texcoord = posL;

    mat4 rotView = mat4(mat3(view)); // remove translation from the view matrix
    vec4 clipPos = proj * rotView * vec4(posL, 1.0);

    gl_Position = clipPos.xyww;
}