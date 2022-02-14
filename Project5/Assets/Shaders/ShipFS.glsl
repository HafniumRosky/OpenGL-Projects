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

in v2f
{
	vec3 posW;
	vec3 normalW;
	vec2 uv;
	vec4 posS;
}pIn;

uniform sampler2D texture1;
uniform sampler2D shadowMap;

out vec4 FragColor;

void ComputeDirectLight(DirectionalLight light, out vec4 ambient, out vec4 diffuse, out vec4 specular)
{
	ambient += material.ambient * light.color;

	vec3 lightVec = - normalize(light.direction);
	vec3 normalW = normalize(pIn.normalW);
	float diffuseFactor = clamp(dot(lightVec, normalW), 0.0, 1.0);
	float halfLambert = diffuseFactor * 0.5 + 0.5;
	diffuse += halfLambert * material.diffuse * light.color;

	vec3 reflectDir = normalize(reflect(light.direction, normalW));
	vec3 viewDir = normalize(eyePos - pIn.posW);
	specular += pow(clamp(dot(reflectDir, viewDir), 0.0, 1.0), material.gloss) * material.specular * light.color;
}

void ComputePointLight(PointLight light, out vec4 ambient, out vec4 diffuse, out vec4 specular)
{
	ambient += material.ambient * light.color;

	vec3 lightVec = light.position - pIn.posW;
	float d = length(lightVec);
	//must not flatten
	if(d > light.range)
	return;
	lightVec = normalize(lightVec);
	vec3 normalW = normalize(pIn.normalW);
	float diffuseFactor = clamp(dot(lightVec, normalW), 0.0, 1.0);
	float halfLambert = diffuseFactor * 0.5 + 0.5;
	vec4 diff;
	diff = halfLambert * material.diffuse * light.color;

	vec3 reflectDir = normalize(reflect(-lightVec, normalW));
	vec3 viewDir = normalize(eyePos - pIn.posW);
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

void RenderShip()
{
	vec4 ambient = vec4(0, 0, 0, 0);
	vec4 diffuse = vec4(0, 0, 0, 0);
	vec4 specular = vec4(0, 0, 0, 0);

	//must not unroll
	int i;
	//Directional Light
	for(i = 0; i < numDirLight; i++)
	{
		ComputeDirectLight(dirLight[i], ambient, diffuse, specular);
	}
	//Point light
	for(i = 0; i < numPointLight; i++)
	{
		ComputePointLight(pointLight[i], ambient, diffuse, specular);
	}

	float shadow;
	float shadowFact;
	for(i = 0; i < numDirLight; i++)
	{
		shadow = max(computeShadow(pIn.posS, dirLight[i], shadowFact), shadow);
	}

	for(i = 0; i < numPointLight; i++)
	{
		shadow = max(computeShadow(pIn.posS, dirLight[i], shadowFact), shadow);
	}

	vec4 texColor = texture(texture1, pIn.uv);
	vec4 litColor = texColor * (ambient + diffuse * (1 - shadow) + diffuse * shadow * 0.8) + specular * (1 - shadow);
	litColor.a = texColor.a * material.diffuse.a;
	FragColor = litColor;
}

void main()
{
	if(!renderDepth)
		RenderShip();
	//else
		//FragColor = vec4(gl_FragDepth);
}