#ifndef CUBE_H
#define CUBE_H

#include "GameObject.h"

class Cube : public GameObject
{
private:
	void GenerateCube(MeshData& cubeMesh);

public:
	Cube() = default;
	Cube(VertexType type)
	{
		m_type = type;
	}
	void LoadGameObject(float gloss);
	void LoadPBRGameObject(vec3 albedo = vec3(-1.0f, -1.0f, -1.0f), float metallic = -1.0f, float roughness = -1.0f);
	void Draw();
	void DrawPBR();
	void Update(float dt);
};

#endif