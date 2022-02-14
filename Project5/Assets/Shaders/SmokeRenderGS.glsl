#version 430

layout (points) in;
layout (triangle_strip) out;
layout (max_vertices = 4) out;

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

out vec2 uv;

void main()
{
    vec3 posW = gl_in[0].gl_Position.xyz;
	vec3 toCamera = normalize(eyePos - posW);
    vec3 up = vec3(0.0, 1.0, 0.0);
    vec3 right = cross(toCamera, up);
	posW -= (right * 0.5);
    gl_Position = proj * view * vec4(posW, 1.0);
    uv = vec2(0.0, 0.0);
    EmitVertex();

    posW.y += 1.0;
    gl_Position = proj * view * vec4(posW, 1.0);
    uv = vec2(0.0, 1.0);
    EmitVertex();

    posW.y -= 1.0;
    posW += right;
    gl_Position = proj * view * vec4(posW, 1.0);
    uv = vec2(1.0, 0.0);
    EmitVertex();

    posW.y += 1.0;
    gl_Position = proj * view * vec4(posW, 1.0);
    uv = vec2(1.0, 1.0);
    EmitVertex();

    EndPrimitive();
}