#version 430

in v2f
{
	vec3 posL;
}pIn;
  
uniform samplerCube environmentMap;

uniform float roughness;

const float PI = 3.14159265359;
 
out vec4 FragColor;

float RadicalInverseSeq(uint bits)
{
	bits = (bits << 16u) | (bits >> 16u);
    bits = ((bits & 0x55555555u) << 1u) | ((bits & 0xAAAAAAAAu) >> 1u);
    bits = ((bits & 0x33333333u) << 2u) | ((bits & 0xCCCCCCCCu) >> 2u);
    bits = ((bits & 0x0F0F0F0Fu) << 4u) | ((bits & 0xF0F0F0F0u) >> 4u);
    bits = ((bits & 0x00FF00FFu) << 8u) | ((bits & 0xFF00FF00u) >> 8u);
    return float(bits) * 2.3283064365386963e-10; 
}

vec2 HammersleySeq(uint i, uint N)
{
    return vec2(float(i)/float(N), RadicalInverseSeq(i));
}

vec3 ImportanceSamplingGGX(vec2 uniformSample, vec3 viewDir, float rough)
{
    float r = rough * rough;
    //Bias sample vectors to the specular lobes
    float phi = 2.0 * PI * uniformSample.x;
    float cosTheta = sqrt((1.0 - uniformSample.y) / (1.0 + (r*r - 1.0) * uniformSample.y));
    float sinTheta = sqrt(1.0 - cosTheta*cosTheta);

    //transform from sphereical coord to cartesian coord
    vec3 sampleDirT;
    sampleDirT.x = cos(phi) * sinTheta;
    sampleDirT.y = sin(phi) * sinTheta;
    sampleDirT.z = cosTheta;

    //transform from tangent to world
    vec3 up = abs(viewDir.z) < 0.999 ? vec3(0.0, 0.0, 1.0) : vec3(1.0, 0.0, 0.0);
    vec3 tangent = normalize(cross(up, viewDir));
    vec3 bitangent = cross(viewDir, tangent);

    vec3 sampleDirW = tangent * sampleDirT.x + bitangent * sampleDirT.y + viewDir * sampleDirT.z;
    return normalize(sampleDirW);
}

void main()
{
    vec3 outSampleVec = normalize(pIn.posL);
    const uint numSample = 1024;
    float weight = 0.0;
    vec3 prefilteredColor = vec3(0.0);
    for(uint i = 0; i < numSample; i++)
    {
        vec2 uniformSample = HammersleySeq(i, numSample);
        vec3 sampleDir = ImportanceSamplingGGX(uniformSample, outSampleVec, roughness);
        vec3 L = normalize(2.0 * dot(outSampleVec, sampleDir) * sampleDir - outSampleVec);

        float OdotL = max(dot(outSampleVec, L), 0.0);
        if(OdotL > 0.0)
        {
            prefilteredColor += texture(environmentMap, L).rgb * OdotL;
            weight      += OdotL;
        }
    }
    prefilteredColor = prefilteredColor / weight;
    FragColor = vec4(prefilteredColor, 1.0);
}