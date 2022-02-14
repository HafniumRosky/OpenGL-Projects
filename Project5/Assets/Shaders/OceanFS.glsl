#version 430

struct Material
{
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
	float gloss;
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

layout (std140, binding = 0) uniform CBChangeEveryDrawing
{
	mat4 world;
	mat4 worldInvTranspose;
	Material material;
	//pad * 3
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

in vec3 posW;
in vec3 normalW;
in vec2 uv;
in vec4 posS;
in vec3 OposW;
in float heightNoise;

uniform sampler2D heightMap;
uniform sampler2D normalMap;
uniform sampler2D shadowMap;
uniform samplerCube EnvMap;

uniform float currTime;

out vec4 FragColor;

//vec3 shallowOceanColor = vec3(0.1960, 0.5134, 0.6784);
//vec3 shallowOceanColor = vec3(0.2745, 0.7882, 0.8667);
//vec3 deepOceanColor = vec3(0.2666, 0.3176, 0.5372);
//vec3 deepOceanColor = vec3(0.3647, 0.4196, 0.6471);
vec3 shallowOceanColor = vec3(0.0667, 0.5882, 0.6471);
vec3 deepOceanColor = vec3(0.0, 0.0941, 0.1608);

void ComputeDirectLight(DirectionalLight light, out vec4 ambient, out vec4 diffuse, out vec4 specular)
{
	ambient += material.ambient * light.color;

	vec3 lightVec = - normalize(light.direction);
	vec3 normalW = normalize(normalW);
	float diffuseFactor = clamp(dot(lightVec, normalW), 0.0, 1.0);
	float halfLambert = diffuseFactor * 0.5 + 0.5;
	diffuse += halfLambert * material.diffuse * light.color;

	vec3 reflectDir = normalize(reflect(light.direction, normalW));
	vec3 viewDir = normalize(eyePos - posW);
	specular += pow(clamp(dot(reflectDir, viewDir), 0.0, 1.0), material.gloss) * material.specular * light.color;
}

void ComputePointLight(PointLight light, out vec4 ambient, out vec4 diffuse, out vec4 specular)
{
	ambient += material.ambient * light.color;

	vec3 lightVec = light.position - posW;
	float d = length(lightVec);
	//must not flatten
	if(d > light.range)
	return;
	lightVec = normalize(lightVec);
	vec3 normalW = normalize(normalW);
	float diffuseFactor = clamp(dot(lightVec, normalW), 0.0, 1.0);
	float halfLambert = diffuseFactor * 0.5 + 0.5;
	vec4 diff;
	diff = halfLambert * material.diffuse * light.color;

	vec3 reflectDir = normalize(reflect(-lightVec, normalW));
	vec3 viewDir = normalize(eyePos - posW);
	vec4 spec;
	spec = pow(clamp(dot(reflectDir, viewDir), 0.0, 1.0), material.gloss) * material.specular * light.color;

	float atten = 1.0 / dot(light.att, vec3(1.0, d, d * d));
	diffuse += diff * atten;
	specular += spec * atten;
}

float computeShadow(vec4 posS, DirectionalLight light, out float shadowFact)
{
	float shadow = 0.0;
	vec3 projCoord = posS.xyz / posS.w;
	projCoord = projCoord * 0.5 + 0.5; 
	if(projCoord.z <= 1.0)
	{
		vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
		float currentDepth = projCoord.z;  
		for(int x = -1; x <= 1; ++x)
		{
			for(int y = -1; y <= 1; ++y)
			{
				float pcfDepth = texture(shadowMap, projCoord.xy + vec2(x, y) * texelSize).r; 
				shadow += step(pcfDepth, currentDepth);        
			}    
		}
		shadow /= 9.0;
	}
	return shadow;
}

vec3 ReconstructNormal()
{
	vec2 texelSize = 1.0 / textureSize(heightMap, 5);
	vec2 UV = vec2(uv.x + currTime, uv.y) * 5;
	float height = texture(heightMap, UV).x * 1.5;
	float heightR = texture(heightMap, UV + vec2(1, 0) * texelSize * 5).x * 1.5;
	float heightU = texture(heightMap, UV + vec2(0, 1) * texelSize * 5).x * 1.5;
	float dhdu = (heightR - height) / (texelSize.x * 5);
	float dhdv = (heightU - height) / (texelSize.y * 5);
	vec3 dpdx = dFdx(OposW);
	vec3 dpdy = dFdy(OposW);
	float dhdx = dhdu * dFdx(UV.x) + dhdv * dFdx(UV.y);
	float dhdy = dhdu * dFdy(UV.x) + dhdv * dFdy(UV.y);

	vec3 r1 = cross(dpdy, normalW);
	vec3 r2 = cross(normalW, dpdx);

	return normalize(normalW - (r1 * dhdx + r2 * dhdy) / dot(dpdx, r1));
}

vec3 PerturbNormal(vec3 normal)
{
	//vec2 UV = vec2(uv.x + currTime, uv.y) * 20;
	vec3 normalP;
	normalP = vec3(texture(normalMap, vec2(uv.x + currTime, uv.y) * 0.05).x);
	normalP += vec3(texture(normalMap, vec2(uv.x, uv.y + currTime) * 0.05).x);
	return normalize(normal + normalP);
}

void RenderOcean()
{
	vec3 RNormalW = ReconstructNormal();
	vec3 eyeVec = normalize(eyePos - posW);
	float facing = clamp(dot(eyeVec, RNormalW), 0.0, 1.0);
	vec3 oceanColor = shallowOceanColor + facing * (deepOceanColor - shallowOceanColor);
	vec3 reflectDir = normalize(reflect(-eyeVec, RNormalW));
	float r = 0.02037;
	float fastFresnel = r + (1 - r) * pow(1.0 - dot(eyeVec, RNormalW), 5.0);
	oceanColor = (oceanColor * 0.5 + texture(EnvMap, reflectDir).xyz * fastFresnel * 0.5);
	float shadow;
	float shadowFact;
	int i;
	for(i = 0; i < numDirLight; i++)
	{
		shadow = max(computeShadow(posS, dirLight[i], shadowFact), shadow);
	}

	for(i = 0; i < numPointLight; i++)
	{
		shadow = max(computeShadow(posS, dirLight[i], shadowFact), shadow);
	}
	FragColor = vec4(oceanColor, 0.5) * (1 - shadow) + vec4(oceanColor * shadow * 0.8, 0.7 * shadow);
	//FragColor = vec4(RNormalW, 1.0);
}

void main()
{
	if(!renderDepth)
		RenderOcean();
		//FragColor = texture(heightMap, vec2(uv.x + currTime, uv.y) * 5);
	//else
		//FragColor = vec4(gl_FragDepth);
}