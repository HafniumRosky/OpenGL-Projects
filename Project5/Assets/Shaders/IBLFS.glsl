#version 430

struct PBRMaterial
{
	vec3  albedo;
	float metallic;
	float roughness;
	float ao;
};

struct DirectionalLight
{
	vec4 color;
	vec3 direction;
};

struct PointLight
{
	vec4 color;
	vec3 position;
	float range;
	vec3 att;
	//pad
};

layout (std140, binding = 1) uniform CBChangesEveryFrame
{
	mat4 view;
	mat4 shadowMatrix;
	vec3 eyePos;
	//pad
};

layout (std140, binding = 3) uniform CBChangesRarely
{
	DirectionalLight dirLight[1];
	//pad
	PointLight pointLight[4];
	//pad
	int numDirLight;
	int numPointLight;
	//pad2
};

layout (std140, binding = 4) uniform CBRenderState
{
	bool renderDepth;
};

layout (std140, binding = 5) uniform CBChangeEveryDrawingPBR
{
	mat4 world;
	mat4 worldInvTranspose;
	PBRMaterial material;
	//pad * 2
};

in v2f
{
	vec3 posW;
	vec3 normalW;
	vec2 uv;
}pIn;

uniform sampler2D albedoMap;
uniform sampler2D metallicMap;
uniform sampler2D roughnessMap;
//test
uniform samplerCube irradianceMap;
uniform samplerCube prefilteredMap;
uniform sampler2D BRDFLUT;

float PI = 3.14159265359;

out vec4 FragColor;

vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}  

vec3 FresnelSchlickRoughness(float cosTheta, vec3 F0, float roughness)
{
    return F0 + (max(vec3(1.0 - roughness), F0) - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}

float DistributionGGX(float NdotH, float roughness)
{
    float a      = roughness * roughness;
    float a2     = a * a;
    float NdotH2 = NdotH*NdotH;
	
    float num   = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;
	
    return num / denom;
}

float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r*r) / 8.0;

    float num   = NdotV;
    float denom = NdotV * (1.0 - k) + k;
	
    return num / denom;
}

float GeometrySmith(float NdotL, float NdotV, float roughness)
{
    float ggx2  = GeometrySchlickGGX(NdotV, roughness);
    float ggx1  = GeometrySchlickGGX(NdotL, roughness);
	
    return ggx1 * ggx2;
}

void main()
{
	float useAlbedoMap = step(material.albedo.x, 0.0);
	float useMetallicMap = step(material.metallic, 0.0);
	float useRoughnessMap = step(material.roughness, 0.0);
	vec3 albedo = pow(texture(albedoMap, pIn.uv).rgb * useAlbedoMap + (1 - useAlbedoMap) * material.albedo, vec3(2.2));
    float metallic = texture(metallicMap, pIn.uv).r * useMetallicMap + (1 - useMetallicMap) * material.metallic;
    float roughness = texture(roughnessMap, pIn.uv).r * useRoughnessMap + (1 - useRoughnessMap) * material.roughness;

	vec3 normalW = normalize(pIn.normalW); 
    vec3 viewDir = normalize(eyePos - pIn.posW);

	vec3 F0 = vec3(0.04); 
	F0 = mix(F0, albedo, metallic);

	vec3 Lo = vec3(0.0);

	vec3 R = reflect(-viewDir, normalW);
	const float MAX_REFLECTION_LOD = 4.0;
	vec3 prefilteredColor = textureLod(prefilteredMap, R,  roughness * MAX_REFLECTION_LOD).rgb;  

	float NdotV = max(dot(normalW, viewDir), 0.0);
	vec3 F = FresnelSchlickRoughness(NdotV, F0, roughness);
	vec2 envBRDF = texture(BRDFLUT, vec2(NdotV, roughness)).rg;
	vec3 specular = prefilteredColor * (F * envBRDF.x + envBRDF.y);

	vec3 kS = F;
	vec3 kD = 1.0 - kS;
	kD *= 1.0 - metallic;	  
  
	vec3 irradiance = texture(irradianceMap, normalW).rgb;
	vec3 diffuse = irradiance * albedo;

	Lo = kS * specular + kD * diffuse;
	vec3 color = Lo;
	color = pow(color, vec3(1.0/2.2)); 
	FragColor = vec4(color, 1.0);
}