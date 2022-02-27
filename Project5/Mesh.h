#ifndef MESH_H
#define MESH_H

#include <glm.hpp>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <vector>
#include <string>
#include <stb_image.h>
#include "Effect.h"
#include "LightHelper.h"
#include "Transform.h"



// Macro for indexing vertex buffer
#define BUFFER_OFFSET(i) ((char *)NULL + (i))
#define MAX_NUM_BONES_PER_VERTEX 4

using namespace glm;

enum VertexType
{
	VertexPos,
	VertexPosColor,
	VertexPosTex,
	VertexPosNormal,
	VertexPosNormalColor,
	VertexPosNormalTex,
	VertexPosNormalTangentTex
};

struct DataState
{
	bool pos = false;
	bool normal = false;
	bool tangent = false;
	bool color = false;
	bool tex = false;
	bool bone = false;
};

struct Texture
{
	GLuint texID = 0;
	std::string type;
	std::string name;
};

struct BoneData
{
	GLint boneIndex[MAX_NUM_BONES_PER_VERTEX] = {0};
	GLfloat boneWeight[MAX_NUM_BONES_PER_VERTEX] = {0.0f};
};


class MeshData
{
public:
	Transform m_childTransform;

	std::vector<vec3> m_posL;
	std::vector<vec3> m_normalL;
	std::vector<vec4> m_tangent;
	std::vector<vec4> m_color;
	std::vector<vec2> m_texcoord;
	std::vector<BoneData> m_boneDataVec;
	std::vector<GLuint> m_indexVec;
	std::vector<Texture> m_TextureVec;
	std::vector<unsigned int> m_boneNum;

	DataState m_dataState;
	unsigned int m_offset = 0;
	std::string m_name;

	GLuint VBO, VAO, EBO, VBOBone;
	Effect* m_pEffect = nullptr;
	Material m_material;
	PBRMaterial m_PBRMaterial;

	MeshData()
	{
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);
		glGenBuffers(1, &VBOBone);
	}

	void SetVertexType(VertexType type, bool m_hasBone = false);
	void BindVertexArray();
	void SetVertexBuffer();
	void SetIndexBuffer();
	void BindBuffers();

private:
	void ComputeOffset();

	
};

#endif