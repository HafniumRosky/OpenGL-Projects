#version 430

layout (std140, binding = 1) uniform CBChangesEveryFrame
{
	mat4 view;
	mat4 shadowMatrix;
	vec3 eyePos;
	//pad
};

layout (std140, binding = 4) uniform CBRenderState
{
	bool renderDepth;
};

in v2f
{
	vec3 posW;
	vec3 normalW;
}pIn;

uniform float eta = 0.66;
uniform float etaR = 0.62;
uniform float etaG = 0.67;
uniform float etaB = 0.73;

uniform int renderState = 0;

uniform samplerCube cubeMap;

out vec4 FragColor;

vec3 fresnelSchlick(float HdotV, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(clamp(1.0 - HdotV, 0.0, 1.0), 5.0);
}  

void main()
{
	vec3 viewDir = normalize(eyePos - pIn.posW);
	vec3 normalW = pIn.normalW;

	//Compute reflectance
	vec3 reflectDir = reflect(-viewDir, normalW);
	vec3 reflectColor = texture(cubeMap, reflectDir).rgb;
	reflectColor = pow(reflectColor, vec3(1.0/2.2)); 

	//Compute refractance
	vec3 refractDir = refract(-viewDir, normalW, eta);
	vec3 refractColor = texture(cubeMap, refractDir).rgb;
	refractColor = pow(refractColor, vec3(1.0/2.2)); 

	float F0 = ((1 - 1.0 / eta) / (1 + 1.0 / eta));
	F0 *= F0;
	vec3 halfVec = normalize(viewDir + reflectDir);
	float HdotV = max(dot(halfVec, viewDir), 0.0);
	vec3 F = fresnelSchlick(HdotV, vec3(F0));

	vec3 color = mix(refractColor, reflectColor, F);

	//Chromatic dispersion
	vec3 refractColorD;
	vec3 refractDirR = refract(-viewDir, normalW, etaR);
	vec3 refractDirG = refract(-viewDir, normalW, etaG);
	vec3 refractDirB = refract(-viewDir, normalW, etaB);
	refractColorD.r = texture(cubeMap, refractDirR).r;
	refractColorD.g = texture(cubeMap, refractDirG).g;
	refractColorD.b = texture(cubeMap, refractDirB).b;

	if(renderState == 0)
		color = reflectColor;
	else if(renderState == 1)
		color = refractColor;
	else if(renderState == 2)
		color = mix(refractColor, reflectColor, F);
	else if(renderState == 3)
		color = mix(refractColorD, reflectColor, F);

	FragColor = vec4(color, 1.0);
}