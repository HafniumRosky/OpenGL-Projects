#ifndef LIGHT_PROBE_H
#define LIGHT_PROBE_H

#include "Skybox.h"

class LightProbe : public Skybox
{
private:
	std::string m_lightPorbeName;
	std::string m_sceneName;
	bool m_isGlobal;
	bool m_hasDualParaMap = false;

	//Dual-prarboloid map
	Texture m_dualParaMap[2];
	Texture m_irradianceMap;

	void GeneratePlane(MeshData& texturePlane);
	void GenerateDualParaMap(GLsizei& width, GLsizei& height, MeshData* texturePlane = nullptr);
public:
	LightProbe() = default;
	LightProbe(VertexType type, std::string lightProbeName, std::string sceneName, bool isGlobal, std::string skyboxName = "")
	{
		m_type = type;
		m_lightPorbeName = lightProbeName;
		m_sceneName = sceneName;
		m_skyboxName = skyboxName;
		m_isGlobal = isGlobal;
	}
	void LoadLightProbe(GLsizei width, GLsizei height);
	void SampleCube(float face);
	void GenerateIrradianceMap(int numAzimuth, int numZenith);

	std::string getProbeName()
	{
		return m_lightPorbeName;
	}

	bool HasDualParaMap()
	{
		return m_hasDualParaMap;
	}

	void SetDualParaMap(int index, Texture tex)
	{
		m_dualParaMap[index] = tex;
	}

	void SetIrradianceMap(Texture tex)
	{
		m_irradianceMap = tex;
	}
};

#endif