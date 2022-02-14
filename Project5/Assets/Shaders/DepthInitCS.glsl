#version 430

layout (local_size_x = 2, local_size_y = 2, local_size_z = 1) in;
layout (r32i, binding = 0) uniform iimage2D outPut;

void main()
{
	ivec2 uv = ivec2(gl_GlobalInvocationID);
	imageStore(outPut, uv, ivec4(2));
}