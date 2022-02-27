#version 430

in v2f
{
	vec3 posL;
}pIn;

const float PI = 3.14159265359;
 
out vec2 FragColor;

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

float GeometrySchlickGGX(float NdotV, float roughness)
{
    float a = roughness;
    float k = (a * a) / 2.0;

    float nom   = NdotV;
    float denom = NdotV * (1.0 - k) + k;

    return nom / denom;
}

float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2 = GeometrySchlickGGX(NdotV, roughness);
    float ggx1 = GeometrySchlickGGX(NdotL, roughness);

    return ggx1 * ggx2;
}  

vec2 IntegrateBRDF(float NdotV, float roughness)
{
    vec3 V;
    V.x = sqrt(1.0 - NdotV*NdotV);
    V.y = 0.0;
    V.z = NdotV;

    float A = 0.0;
    float B = 0.0;

    vec3 N = vec3(0.0, 0.0, 1.0);

    const uint SAMPLE_COUNT = 1024u;
    for(uint i = 0u; i < SAMPLE_COUNT; ++i)
    {
        vec2 Xi = HammersleySeq(i, SAMPLE_COUNT);
        vec3 H  = ImportanceSamplingGGX(Xi, N, roughness);
        vec3 L  = normalize(2.0 * dot(V, H) * H - V);

        float NdotL = max(L.z, 0.0);
        float NdotH = max(H.z, 0.0);
        float VdotH = max(dot(V, H), 0.0);

        if(NdotL > 0.0)
        {
            float G = GeometrySmith(N, V, L, roughness);
            float G_Vis = (G * VdotH) / (NdotH * NdotV);
            float Fc = pow(1.0 - VdotH, 5.0);

            A += (1.0 - Fc) * G_Vis;
            B += Fc * G_Vis;
        }
    }
    A /= float(SAMPLE_COUNT);
    B /= float(SAMPLE_COUNT);
    return vec2(A, B);
}
// ----------------------------------------------------------------------------
void main() 
{
    vec2 integratedBRDF = IntegrateBRDF(pIn.posL.x + 0.5, pIn.posL.y + 0.5);
    FragColor = integratedBRDF;
}