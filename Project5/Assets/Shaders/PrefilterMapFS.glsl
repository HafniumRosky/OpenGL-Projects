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
	uint inversedBits = 0;
    uint bitNumValue = 1;
    for(;bits > 0; bits /= 2)
    {
        inversedBits = inversedBits * 2 + bits % 2;
        bitNumValue = bitNumValue * 2;
    }
    return inversedBits / float(bitNumValue);
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