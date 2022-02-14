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
	float azimuthOffset = (PI * 2) / numAzimuth;
	float zenithOffset = (PI * 2) / numZenith;
	for(float i = 0; i < numAzimuth; i += azimuthOffset)
	{
		for(float j = 0; j < numZenith; j += zenithOffset)
		{
			vec3 sampleDir;
			sampleDir.x = cos(azimuthOffset * i) * sin(zenithOffset * i);
			sampleDir.y = sin(azimuthOffset * i) * sin(zenithOffset * i);
			sampleDir.z = cos(zenithOffset * i);
			sampleDir = normalize(sampleDir);
			irradiance += texture(environmentMap, sampleDir).rgb * cos(zenithOffset * i) * sin(zenithOffset * i);
		}
	}
	irradiance *= (PI / (numAzimuth * numZenith));
}

void main()
{
	vec3 irradiance = vec3(0);
	ComputeDiffuseIrradiance(irradiance);
	FragColor = vec4(irradiance, 1.0);
}