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
};

struct Texture
{
	GLuint texID = 0;
	std::string type;
	std::string name;
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
	std::vector<GLuint> m_indexVec;
	std::vector<Texture> m_TextureVec;

	DataState m_dataState;
	unsigned int m_offset = 0;
	std::string m_name;

	GLuint VBO, VAO, EBO;
	Effect* m_pEffect = nullptr;
	Material m_material;
	PBRMaterial m_PBRMaterial;

	MeshData()
	{
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);
	}

	void SetVertexType(VertexType type)
	{
		if (type == VertexPos)
		{
			this->m_dataState.pos = true;
		}
		if (type == VertexPosColor)
		{
			this->m_dataState.pos = true;
			this->m_dataState.color = true;
		}
		if (type == VertexPosTex)
		{
			this->m_dataState.pos = true;
			this->m_dataState.tex = true;
		}
		if (type == VertexPosNormal)
		{
			this->m_dataState.pos = true;
			this->m_dataState.normal = true;
		}
		if (type == VertexPosNormalColor)
		{
			this->m_dataState.pos = true;
			this->m_dataState.normal = true;
			this->m_dataState.color = true;
		}
		if (type == VertexPosNormalTex)
		{
			this->m_dataState.pos = true;
			this->m_dataState.normal = true;
			this->m_dataState.tex = true;
		}
		if (type == VertexPosNormalTangentTex)
		{
			this->m_dataState.pos = true;
			this->m_dataState.normal = true;
			this->m_dataState.tangent = true;
			this->m_dataState.tex = true;
		}
	}


	void BindVertexArray()
	{
		glBindVertexArray(VAO);
	}

	void SetVertexBuffer()
	{
		//Compute inputlayout
		ComputeOffset();
		unsigned int alignedOffset = 0;
		//Bind VBO
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		// After binding, we now fill our object with data, everything in "Vertices" goes to the GPU
		glBufferData(GL_ARRAY_BUFFER, m_posL.size() * m_offset * sizeof(GLfloat), NULL, GL_STATIC_DRAW);
		// if you have more data besides vertices (e.g., vertex colours or normals), use glBufferSubData to tell the buffer when the vertices array ends and when the colors start
		glBufferSubData(GL_ARRAY_BUFFER, 0, m_posL.size() * sizeof(vec3), &m_posL[0]);
		alignedOffset += m_posL.size() * sizeof(vec3);
		if (this->m_dataState.normal)
		{
			glBufferSubData(GL_ARRAY_BUFFER, alignedOffset, m_posL.size() * sizeof(vec3), &m_normalL[0]);
			alignedOffset += m_posL.size() * sizeof(vec3);
		}
		if (this->m_dataState.tangent)
		{
			glBufferSubData(GL_ARRAY_BUFFER, alignedOffset, m_posL.size() * sizeof(vec4), &m_tangent[0]);
			alignedOffset += m_posL.size() * sizeof(vec4);
		}
		if (this->m_dataState.color)
		{
			glBufferSubData(GL_ARRAY_BUFFER, alignedOffset, m_posL.size() * sizeof(vec4), &m_color[0]);
			alignedOffset += m_posL.size() * sizeof(vec4);
		}
		if (this->m_dataState.tex)
		{
			glBufferSubData(GL_ARRAY_BUFFER, alignedOffset, m_posL.size() * sizeof(vec2), &m_texcoord[0]);
		}
	}

	void SetIndexBuffer()
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indexVec.size() * sizeof(GLuint), &m_indexVec[0], GL_STATIC_DRAW);
	}

	void BindBuffers()
	{
		unsigned int attributeCount = 0;
		unsigned int alignedOffset = 0;

		// vertex positions
		GLuint POSITION = glGetAttribLocation(this->m_pEffect->getShaderProgramID(), "posL");
		glEnableVertexAttribArray(attributeCount);
		glVertexAttribPointer(POSITION, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
		attributeCount++;
		alignedOffset += m_posL.size() * sizeof(vec3);
		// vertex normals
		if (this->m_dataState.normal)
		{
			GLuint NORMAL = glGetAttribLocation(this->m_pEffect->getShaderProgramID(), "normalL");
			glEnableVertexAttribArray(attributeCount);
			glVertexAttribPointer(NORMAL, 3, GL_FLOAT, GL_FALSE, 0, (void*)BUFFER_OFFSET(alignedOffset));
			attributeCount++;
			alignedOffset += m_posL.size() * sizeof(vec3);
		}
		if (this->m_dataState.tangent)
		{
			GLuint TANGENT = glGetAttribLocation(this->m_pEffect->getShaderProgramID(), "tangent");
			glEnableVertexAttribArray(attributeCount);
			glVertexAttribPointer(TANGENT, 4, GL_FLOAT, GL_FALSE, 0, (void*)BUFFER_OFFSET(alignedOffset));
			attributeCount++;
			alignedOffset += m_posL.size() * sizeof(vec4);
		}
		if (this->m_dataState.color)
		{
			GLuint COLOR = glGetAttribLocation(this->m_pEffect->getShaderProgramID(), "color");
			glEnableVertexAttribArray(attributeCount);
			glVertexAttribPointer(COLOR, 4, GL_FLOAT, GL_FALSE, 0, (void*)BUFFER_OFFSET(alignedOffset));
			attributeCount++;
			alignedOffset += m_posL.size() * sizeof(vec4);
		}
		if (this->m_dataState.tex)
		{
			GLuint TEXCOORD = glGetAttribLocation(this->m_pEffect->getShaderProgramID(), "texcoord");
			glEnableVertexAttribArray(attributeCount);
			glVertexAttribPointer(TEXCOORD, 2, GL_FLOAT, GL_FALSE, 0, (void*)BUFFER_OFFSET(alignedOffset));
		}

		glBindVertexArray(0);
	}

private:
	void ComputeOffset()
	{
		if (this->m_dataState.pos)
			this->m_offset += 3;
		if (this->m_dataState.normal)
			this->m_offset += 3;
		if (this->m_dataState.tangent)
			this->m_offset += 4;
		if (this->m_dataState.color)
			this->m_offset += 4;
		if (this->m_dataState.tex)
			this->m_offset += 2;
	}

	
};

#endif