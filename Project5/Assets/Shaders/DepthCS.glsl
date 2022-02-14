#version 430

layout (local_size_x = 32, local_size_y = 32, local_size_z = 1) in;
layout (r32f, binding = 0) uniform image2D outPut;
layout (r32f, binding = 1) uniform image2D oceanDepth;
layout (r32f, binding = 2) uniform image2D shipDepth;

/*int ValueMap(float valF)
{
	float valClamp = 100.0;
	float realVal = valF * valClamp;
	int iVal = int(realVal * 10000);
	return iVal;
}*/

void main()
{
	ivec2 uv = ivec2(gl_GlobalInvocationID);
	float oceanVal = imageLoad(oceanDepth, uv).x;
	float shipVal = imageLoad(shipDepth, uv).x;
	//int uOceanVal = ValueMap(oceanVal);
	//int uShipVal = ValueMap(shipVal);
	//Flatten
	float isShip = step(shipVal, 0.3);
	float subVal = (oceanVal - shipVal) * isShip;
	//imageAtomicAdd(outPut, uv / 16, subVal);
	imageStore(outPut, uv, vec4(isShip));
}