#ifndef OCEAN_H
#define OCEAN_H

#include "GameObject.h"

class Ocean : public GameObject
{
private:
	int m_oceanVertWidth = 200;
	int m_oceanHorWidth = 200;
	GLuint m_environmentMapID;
	void GeneratePlane(MeshData& oceanMesh);
	void GenerateTexture(MeshData& oceanMesh, GLsizei width, GLsizei height);

public:
	Ocean() = default;
	Ocean(VertexType type, int vert, int hor)
	{
		m_type = type;
		m_oceanVertWidth = vert;
		m_oceanHorWidth = hor;
	}
	void LoadGameObject(GLsizei texWidth, GLsizei texHeight);
	void Draw();
	void Update(float deltaTime);
	void SetEnvironmentMapID(GLuint cubeID)
	{
		m_environmentMapID = cubeID;
	}
};

#endif