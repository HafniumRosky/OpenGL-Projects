#version 430

layout(triangles, equal_spacing, ccw) in;

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

in vec3 posW_DS[];
in vec3 normalW_DS[];
in vec2 uv_DS[];

out vec3 posW;
out vec3 normalW;
out vec2 uv;
out vec4 posS;
out vec3 OposW;
out float heightNoise;

uniform sampler2D heightMap;
uniform float currTime;

vec2 Interpolate2D(vec2 v0, vec2 v1, vec2 v2)
{
    return vec2(gl_TessCoord.x) * v0 + vec2(gl_TessCoord.y) * v1 + vec2(gl_TessCoord.z) * v2;
}

vec3 Interpolate3D(vec3 v0, vec3 v1, vec3 v2)
{
    return vec3(gl_TessCoord.x) * v0 + vec3(gl_TessCoord.y) * v1 + vec3(gl_TessCoord.z) * v2;
}

float fade(float t)
{
	return t * t * t * (t * (t * 6 - 15) + 10);
}

float lerp(float x, float y, float z)
{
	return x + z * (y - x);
}

float noise(vec2 xy)
{
	float dt = currTime * 500.0;
	vec2 gradUL = vec2(sin(123.0 * floor(xy.x) + .4567 * floor(xy.y) + dt), sin(123.0 * floor(xy.y) + .4567 * floor(xy.x) + dt));
	vec2 gradUR = vec2(sin(123.0 * floor(xy.x + 1) + .4567 * floor(xy.y) + dt), sin(123.0 * floor(xy.y) + .4567 * floor(xy.x + 1) + dt));
	vec2 gradBL = vec2(sin(123.0 * floor(xy.x) + .4567 * floor(xy.y + 1) + dt), sin(123.0 * floor(xy.y + 1) + .4567 * floor(xy.x) + dt));
	vec2 gradBR = vec2(sin(123.0 * floor(xy.x + 1) + .4567 * floor(xy.y + 1) + dt), sin(123.0 * floor(xy.y + 1) + .4567 * floor(xy.x + 1) + dt));

	vec2 pointVecUL = xy - floor(xy);
	vec2 pointVecUR = xy - floor(vec2(xy.x + 1, xy.y));
	vec2 pointVecBL = xy - floor(vec2(xy.x, xy.y + 1));
	vec2 pointVecBR = xy - floor(vec2(xy.x + 1, xy.y + 1));

	float dotUL = dot(gradUL, pointVecUL);
	float dotUR = dot(gradUR, pointVecUR);
	float dotBL = dot(gradBL, pointVecBL);
	float dotBR = dot(gradBR, pointVecBR);

	vec2 XY = vec2(fade(xy.x - floor(xy.x)), fade(xy.y - floor(xy.y)));

	return lerp(lerp(dotUL, dotUR, XY.x), lerp(dotBL, dotBR, XY.x), XY.y);
}

void main()
{
	posW = Interpolate3D(posW_DS[0], posW_DS[1], posW_DS[2]);
	OposW = posW;
	normalW = normalize(Interpolate3D(normalW_DS[0], normalW_DS[1], normalW_DS[2]));
	if(!renderDepth)
	{
		uv = Interpolate2D(uv_DS[0], uv_DS[1], uv_DS[2]);
	}
	float displace = texture(heightMap, vec2(uv.x + currTime, uv.y) * 5).x;
	//displace += texture(heightMap, vec2(uv.x, uv.y * 2 + currTime * 0.25) * 5).x;
	//displace += texture(heightMap, vec2(uv.x / 2.0 + currTime * 0.5, uv.y) * 5).x;
	//displace /= 3.0;
	posW += normalW * displace;
	vec2 gridUV = uv * 300;
	float heightNoise = noise(gridUV);
	posW += normalW * heightNoise;

	posS = shadowMatrix * vec4(posW, 1.0);
	gl_Position = proj * view * vec4(posW, 1.0);
}