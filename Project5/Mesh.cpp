#include "Mesh.h"

void MeshData::SetVertexType(VertexType type, bool hasBone)
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
	this->m_dataState.bone = hasBone;
}

void MeshData::BindVertexArray()
{
	glBindVertexArray(VAO);
}

void MeshData::SetVertexBuffer()
{
	//Compute inputlayout
	ComputeOffset();
	unsigned int alignedOffset = 0;
	//Bind VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	unsigned int bufferSize = m_posL.size() * m_offset * sizeof(GLfloat);
	//if (this->m_dataState.bone)
		//bufferSize += m_posL.size() * sizeof(BoneData);
	// After binding, we now fill our object with data, everything in "Vertices" goes to the GPU
	glBufferData(GL_ARRAY_BUFFER, bufferSize, NULL, GL_STATIC_DRAW);
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
		alignedOffset += m_posL.size() * sizeof(vec2);
	}
	/*if (this->m_dataState.bone)
	{
		glBufferSubData(GL_ARRAY_BUFFER, alignedOffset, m_posL.size() * sizeof(BoneData), &m_boneDataVec[0]);
	}*/
}

void MeshData::SetIndexBuffer()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indexVec.size() * sizeof(GLuint), &m_indexVec[0], GL_STATIC_DRAW);
}

void MeshData::BindBuffers()
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
		attributeCount++;
		alignedOffset += m_posL.size() * sizeof(vec2);
	}
	if (this->m_dataState.bone)
	{
		//Bone index
		/*GLuint BONEINDEX = glGetAttribLocation(this->m_pEffect->getShaderProgramID(), "boneIndex");
		glEnableVertexAttribArray(attributeCount);
		glVertexAttribPointer(BONEINDEX, MAX_NUM_BONES_PER_VERTEX, GL_INT, GL_FALSE, 0, (void*)BUFFER_OFFSET(alignedOffset));
		attributeCount++;
		alignedOffset += m_posL.size() * MAX_NUM_BONES_PER_VERTEX * sizeof(GLint);

		//Bone weight
		GLuint BONEWEIGHT = glGetAttribLocation(this->m_pEffect->getShaderProgramID(), "boneWeight");
		glEnableVertexAttribArray(attributeCount);
		glVertexAttribPointer(BONEWEIGHT, MAX_NUM_BONES_PER_VERTEX, GL_FLOAT, GL_FALSE, 0, (void*)BUFFER_OFFSET(alignedOffset));*/

		glBindBuffer(GL_ARRAY_BUFFER, VBOBone);
		unsigned int bufferSize = m_boneDataVec.size() * (MAX_NUM_BONES_PER_VERTEX * sizeof(GLint) + MAX_NUM_BONES_PER_VERTEX * sizeof(GLfloat));
		glBufferData(GL_ARRAY_BUFFER, bufferSize, &m_boneDataVec[0], GL_STATIC_DRAW);

		GLuint BONEINDEX = glGetAttribLocation(this->m_pEffect->getShaderProgramID(), "boneIndex");
		glEnableVertexAttribArray(BONEINDEX);
		glVertexAttribIPointer(BONEINDEX, MAX_NUM_BONES_PER_VERTEX, GL_INT, sizeof(BoneData), (const GLvoid*)0);
		//alignedOffset = m_boneDataVec.size() * sizeof(GLint) * 4;

		GLuint BONEWEIGHT = glGetAttribLocation(this->m_pEffect->getShaderProgramID(), "boneWeight");
		glEnableVertexAttribArray(BONEWEIGHT);
		glVertexAttribPointer(BONEWEIGHT, MAX_NUM_BONES_PER_VERTEX, GL_FLOAT, GL_FALSE, sizeof(BoneData), (const GLvoid*)16);
	}

	glBindVertexArray(0);
}

void MeshData::ComputeOffset()
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