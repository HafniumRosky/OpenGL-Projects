#version 430

layout (vertices = 3) out;

layout (std140, binding = 1) uniform CBChangesEveryFrame
{
	mat4 view;
    mat4 shadowMatrix;
	vec3 eyePos;
	//pad
};

layout (std140, binding = 4) uniform CBRenderState
{
	bool renderDepth;
};

in vec3 posW_HS[];
in vec3 normalW_HS[];
in vec2 uv_HS[];

out vec3 posW_DS[];
out vec3 normalW_DS[];
out vec2 uv_DS[];

float GetTessLevel(float distance0, float distance1)
{
    float avgDistance = (distance0 + distance1) / 2.0;

    if (avgDistance <= 50.0) 
    {
        return 8.0;
    }
    else if (avgDistance <= 100.0) 
    {
        return 4.0;
    }
    else if (avgDistance <= 200.0) 
    {
        return 2.0;
    }
    else 
    {
        return 1.0;
    }
}

void main()
{
	posW_DS[gl_InvocationID] = posW_HS[gl_InvocationID];
    normalW_DS[gl_InvocationID] = normalW_HS[gl_InvocationID];
    if(!renderDepth)
    {
        uv_DS[gl_InvocationID] = uv_HS[gl_InvocationID];
    }

	float eyeToVertexDistance0 = distance(eyePos, posW_DS[0]);
    float eyeToVertexDistance1 = distance(eyePos, posW_DS[1]);
    float eyeToVertexDistance2 = distance(eyePos, posW_DS[2]);

    gl_TessLevelOuter[0] = GetTessLevel(eyeToVertexDistance1, eyeToVertexDistance2);
    gl_TessLevelOuter[1] = GetTessLevel(eyeToVertexDistance2, eyeToVertexDistance0);
    gl_TessLevelOuter[2] = GetTessLevel(eyeToVertexDistance0, eyeToVertexDistance1);
    gl_TessLevelInner[0] = gl_TessLevelOuter[2];
}