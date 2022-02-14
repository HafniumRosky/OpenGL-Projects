#version 430

layout (local_size_x = 32, local_size_y = 32, local_size_z = 1) in;
layout (r32f, binding = 0) uniform image2D outputImage;

void main()
{
	ivec2 pos = ivec2(gl_GlobalInvocationID);
	vec4 illumination = vec4(pos.x / 1024.0);
	imageStore(outputImage, pos, illumination);
}