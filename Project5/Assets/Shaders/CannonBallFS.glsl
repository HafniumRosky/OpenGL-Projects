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
	vec4 color;
	vec4 posS;
}pIn;

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
	float halfLambert = diffuseFactor;
	vec4 diff;
	diff = halfLambert * material.diffuse * light.color;

	vec3 reflectDir = normalize(reflect(-lightVec, normalW));
	vec3 viewDir = normalize(eyePos - pIn.posW);
	vec4 spec;
	vec3 halfDir = normalize(lightVec + viewDir);
	spec = pow(clamp(dot(normalW, halfDir), 0.0, 1.0), material.gloss) * material.specular * light.color;

	float atten = 1.0 / dot(light.att, vec3(1.0, d, d * d));
	diffuse += diff * atten;
	ambient += ambient * atten;
	specular += spec;
}

void RenderBall()
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
	ambient = vec4(0.1, 0.1, 0.1, 1.0);
	vec4 texColor = vec4(pow(pIn.color.rgb, vec3(2.2)), 1.0f);
	vec4 litColor = texColor * (ambient + diffuse) + specular;
	litColor.a = texColor.a * material.diffuse.a;
	vec3 color = litColor.rgb / (litColor.rgb + vec3(1.0));
	color = pow(color, vec3(1.0/2.2)); 
	FragColor = vec4(color, 1.0);
}

void main()
{
	if(!renderDepth)
		RenderBall();
	//else
		//FragColor = vec4(gl_FragDepth);
}