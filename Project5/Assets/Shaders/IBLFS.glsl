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
uniform vec4 emmision = vec4(0.0);
//test
uniform vec3 probePos0;
uniform vec3 probePos1;
uniform float weight0;
uniform float weight1;
uniform samplerCube irradianceMap;
uniform samplerCube irradianceMap0;
uniform samplerCube irradianceMap1;
uniform samplerCube prefilteredMap;
uniform samplerCube prefilteredMap0;
uniform samplerCube prefilteredMap1;
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
    float k = (roughness * roughness) / 2.0;

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

vec3 ParallaxCorrection(vec3 probe, vec3 pos, vec3 v)
{
	if(sign((probe.x + 5.0001 - pos.x) / v.x) == sign((probe.x - 5.0001 - pos.x) / v.x))
		return vec3(0);
	if(sign((probe.y + 5.0001 - pos.y) / v.y) == sign((probe.y - 5.0001 - pos.y) / v.y))
		return vec3(0);
	if(sign((probe.z + 10.0001 - pos.z) / v.z) == sign((probe.z - 10.0001 - pos.z) / v.z))
		return vec3(0);
	float pLength = max((probe.x + 5.0001 - pos.x) / v.x, (probe.x - 5.0001 - pos.x) / v.x);
	float pHeight = max((probe.y + 5.0001 - pos.y) / v.y, (probe.y - 5.0001 - pos.y) / v.y);
	float pWidth = max((probe.z + 10.0001 - pos.z) / v.z, (probe.z - 10.0001 - pos.z) / v.z);
	float dist = min(min(pLength, pHeight), pWidth);
	vec3 mark = pos + v * dist;
	return normalize(mark - probe);
}

float ComputeWeight(vec3 probe, vec3 pos)
{
	vec3 posL = pos - probe;
	vec3 dirL = vec3(abs(posL.x), abs(posL.y), abs(posL.z));
	dirL = dirL * 2.0 / vec3(50, 50, 50);
    float effective = max(max(dirL.x, dirL.y), dirL.z);
    return clamp(1.0f - effective, 0.f, 1.0);
}

void main()
{

	//Compute weight
	float Weight0 = ComputeWeight(probePos0, pIn.posW);
	float Weight1 = ComputeWeight(probePos1, pIn.posW);
	float Weight = Weight0 + Weight1;
	Weight0 = Weight0 / Weight;
	Weight1 = Weight1 / Weight;

	float useAlbedoMap = step(material.albedo.x, -0.1);
	float useMetallicMap = step(material.metallic, -0.1);
	float useRoughnessMap = step(material.roughness, -0.1);
	vec3 albedo = pow(texture(albedoMap, pIn.uv).rgb, vec3(2.2)) * useAlbedoMap + (1 - useAlbedoMap) * material.albedo;
    float metallic = texture(metallicMap, pIn.uv).r * useMetallicMap + (1 - useMetallicMap) * material.metallic;
    float roughness = texture(roughnessMap, pIn.uv).r * useRoughnessMap + (1 - useRoughnessMap) * material.roughness;

	vec3 normalW = normalize(pIn.normalW); 
    vec3 viewDir = normalize(eyePos - pIn.posW);

	vec3 F0 = vec3(0.04); 
	F0 = mix(F0, albedo, metallic);

	vec3 Lo = vec3(0.0);

	vec3 R = reflect(-viewDir, normalW);
	const float MAX_REFLECTION_LOD = 4.0;
	//vec3 prefilteredColor = textureLod(prefilteredMap, R,  roughness * MAX_REFLECTION_LOD).rgb;  
	vec3 correctR0 = ParallaxCorrection(probePos0, pIn.posW, R);
	vec3 correctR1 = ParallaxCorrection(probePos1, pIn.posW, R);
	vec3 pC0, pC1;
	if(correctR0 == vec3(0))
		pC0 = vec3(0);
	else
		pC0 = textureLod(prefilteredMap0, correctR0,  roughness * MAX_REFLECTION_LOD).rgb;
	if(correctR1 == vec3(0))
		pC1 = vec3(0);
	else
		pC1 = textureLod(prefilteredMap1, correctR1,  roughness * MAX_REFLECTION_LOD).rgb * Weight1; 
	vec3 prefilteredColor = pC0;  

	float NdotV = max(dot(normalW, viewDir), 0.0);
	vec3 F = FresnelSchlickRoughness(NdotV, F0, roughness);
	vec2 envBRDF = texture(BRDFLUT, vec2(NdotV, roughness)).rg;
	vec3 specular = prefilteredColor * (F * envBRDF.x + envBRDF.y);

	vec3 kS = F;
	vec3 kD = 1.0 - kS;
	kD *= 1.0 - metallic;	  
  
	//vec3 irradiance = texture(irradianceMap, normalW).rgb;

	vec3 correctNoraml = ParallaxCorrection(probePos0, pIn.posW, normalW);
	vec3 iC;
	if(correctNoraml == vec3(0))
		iC = vec3(0);
	else
		iC = texture(irradianceMap0, correctNoraml).rgb;
	vec3 irradiance = iC;
	//vec3 irradiance = texture(irradianceMap0, normalW).rgb * weight0;
	vec3 diffuse = irradiance * albedo;

	Lo = kS * specular + kD * diffuse;


	//Point light
	for(int i = 0; i < numPointLight; i++)
	{
		vec3 lightVec = normalize(pointLight[i].position - pIn.posW);
		vec3 halfVec = normalize(viewDir + lightVec);
		float lightDistance = length(pointLight[i].position - pIn.posW);
		float atten = 1.0 /(lightDistance, lightDistance);
		vec3 radiance = pointLight[i].color.rgb * atten;
		float NdotH = max(dot(normalW, halfVec), 0.0);
		float HdotV = max(dot(halfVec, viewDir), 0.0);
		float NdotV = max(dot(normalW, viewDir), 0.0);
		float NdotL = max(dot(normalW, lightVec), 0.0);
		float D = DistributionGGX(NdotH, roughness);
		vec3 F = fresnelSchlick(HdotV, F0);
		float G = GeometrySmith(NdotL, NdotV, roughness);
		vec3 DFG = D * F * G;
		float denominator = 4.0 * NdotV * NdotL  + 0.0001;
		vec3 specular = DFG / denominator;
		
		vec3 ks = FresnelSchlickRoughness(NdotV, F0, roughness); 
		vec3 kd = vec3(1.0) - ks;
		kd *= 1.0 - metallic;
		Lo += (kd * albedo / PI + ks * specular) * radiance * NdotL;
	}

	vec3 color = Lo + emmision.rgb;
	color = color / (color + vec3(1.0));
	color = pow(color, vec3(1.0/2.2)); 
	FragColor = vec4(color, 1.0);
}