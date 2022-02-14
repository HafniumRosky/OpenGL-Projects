#version 430

in v2f
{
	vec3 posL;
}pIn;

uniform samplerCube cubeMap;

uniform float face;

out vec4 FragColor;

vec3 SampleCubeMap(vec2 v, float scale)
{
    vec2 unclampedST = (v - 0.5) * 2 * scale;
    vec3 sampleDir;
    sampleDir.xy = unclampedST.xy;
    sampleDir.z = face * (0.5 - 0.5 * (unclampedST.x * unclampedST.x + unclampedST.y * unclampedST.y));
    return normalize(sampleDir);
}

void main()
{		
    vec3 texCoord = SampleCubeMap(pIn.posL.xy + vec2(0.5), 1.2);
    vec3 envColor = texture(cubeMap, texCoord).rgb;
    FragColor = vec4(envColor, 1.0);
}