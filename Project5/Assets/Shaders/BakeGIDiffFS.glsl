#version 430

in v2f
{
	vec3 posL;
}pIn;

uniform samplerCube environmentMap;

uniform int numAzimuth = 1000;
uniform int numZenith = 1000;

out vec4 FragColor;

float PI = 3.14159265359;

void ComputeDiffuseIrradiance(out vec3 irradiance)
{
	vec3 facingDir = normalize(pIn.posL);
	vec3 tangentUp = vec3(0.0, 1.0, 0.0);
	vec3 tangentRight = normalize(cross(tangentUp, facingDir));
	tangentUp = normalize(cross(facingDir, tangentRight));

	float azimuthOffset = (PI * 2) / numAzimuth;
	float zenithOffset = (PI * 2) / numZenith;
	int sampleNum = 0;
	for(float phi = 0; phi < 2.0 * PI; phi += azimuthOffset)
	{
		for(float theta = 0; theta < 0.5 * PI; theta += zenithOffset)
		{
			vec3 sampleDirTan;
			sampleDirTan.x = cos(phi) * sin(theta);
			sampleDirTan.y = sin(phi) * sin(theta);
			sampleDirTan.z = cos(theta);
			sampleDirTan = normalize(sampleDirTan);
			vec3 sampleDir = sampleDirTan.x * tangentRight + sampleDirTan.y * tangentUp + sampleDirTan.z * facingDir;
			irradiance += texture(environmentMap, sampleDir).rgb * cos(theta) * sin(theta);
			sampleNum++;
		}
	}
	irradiance *= (PI / float(sampleNum));
}

void main()
{
	vec3 irradiance = vec3(0);
	ComputeDiffuseIrradiance(irradiance);
	FragColor = vec4(irradiance, 1.0);
}