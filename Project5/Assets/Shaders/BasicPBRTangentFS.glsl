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
	vec4 uv;
	vec4 TtoW0;
	vec4 TtoW1;
	vec4 TtoW2;
}pIn;

uniform sampler2D albedoMap;
uniform sampler2D metallicMap;
uniform sampler2D roughnessMap;
uniform sampler2D normalMap;

float PI = 3.14159265359;

out vec4 FragColor;

vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
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
	vec3 albedo = pow(texture(albedoMap, pIn.uv.xy).rgb * useAlbedoMap + (1 - useAlbedoMap) * material.albedo, vec3(2.2));
    float metallic = texture(metallicMap, pIn.uv.xy).r * useMetallicMap + (1 - useMetallicMap) * material.metallic;
    float roughness = texture(roughnessMap, pIn.uv.xy).r * useRoughnessMap + (1 - useRoughnessMap) * material.roughness;

	vec3 posW = vec3(pIn.TtoW0.w, pIn.TtoW1.w, pIn.TtoW2.w);

	vec3 normalW = texture(normalMap, pIn.uv.zw).rgb; 
	normalW = normalize(normalW * 2.0 - 1.0);
	normalW = normalize(vec3(dot(pIn.TtoW0.xyz, normalW), dot(pIn.TtoW1.xyz, normalW), dot(pIn.TtoW2.xyz, normalW)));


    vec3 viewDir = normalize(eyePos - posW);

	vec3 F0 = vec3(0.04); 
	F0 = mix(F0, albedo, metallic);

	vec3 Lo = vec3(0.0);
	//Point light
	for(int i = 0; i < numPointLight; i++)
	{
		vec3 lightVec = normalize(pointLight[i].position - posW);
		vec3 halfVec = normalize(viewDir + lightVec);
		float lightDistance = length(pointLight[i].position - posW);
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
		
		vec3 ks = F;
		vec3 kd = vec3(1.0) - ks;
		kd *= 1.0 - metallic;
		Lo += (kd * albedo / PI + ks * specular) * radiance * NdotL;
	}
	//Analytical light
	for(int i = 0; i < numDirLight; i++)
	{
		vec3 lightVec = normalize(-dirLight[i].direction);
		vec3 halfVec = normalize(viewDir + lightVec);
		vec3 radiance = dirLight[i].color.rgb;
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
		
		vec3 ks = F;
		vec3 kd = vec3(1.0) - ks;
		kd *= 1.0 - metallic;
		Lo += (kd * albedo / PI + ks * specular) * radiance * NdotL;
	}

	vec3 ambient = vec3(0.03) * albedo;
	vec3 color = ambient + Lo;

	//color = color / (color + vec3(1.0));
	color = pow(color, vec3(1.0/2.2)); 

	FragColor = vec4(color, 1.0);
}