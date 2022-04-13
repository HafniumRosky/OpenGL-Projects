#include "Face.h"
#include <io.h>

void Face::LoadGameObject()
{
	LoadGameObjectFromFile(m_modelPath, m_textureDir);
}

void Face::GetFiles(std::string path, std::vector<std::string>& files)
{
	long  hFile = 0;
	struct _finddata_t fileinfo;
	std::string p;
	if ((hFile = _findfirst(p.assign(path).append("\\*").c_str(), &fileinfo)) != -1)
	{
		do
		{
			if ((fileinfo.attrib & _A_SUBDIR))
			{
				if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)
					GetFiles(p.assign(path).append("\\").append(fileinfo.name), files);
			}
			else
			{
				files.push_back(p.assign(path).append("\\").append(fileinfo.name));
			}
		} while (_findnext(hFile, &fileinfo) == 0);
		_findclose(hFile);
	}
}

void Face::LoadPBRGameObject(vec3 albedo, float metallic, float roughness)
{
	/*LoadGameObjectFromFile(m_modelPath, m_textureDir);
	for (int i = 0; i < m_meshVec.size(); i++)
	{
		m_meshVec[i].m_PBRMaterial.albedo = albedo;
		m_meshVec[i].m_PBRMaterial.metallic = metallic;
		m_meshVec[i].m_PBRMaterial.roughness = roughness;
	}*/

	//With blendshapes
	GetFiles(m_modelFolder, m_blendShapeFiles);
	for (int i = 14; i < m_blendShapeFiles.size(); i++)
	{
		LoadGameObjectFromFile(m_blendShapeFiles[i], m_textureDir, m_meshVec.size());
	}
	m_blendWeights.resize(m_meshVec.size());
	for (int i = 0; i < m_meshVec.size(); i++)
	{
		m_blendWeights[i] = 0;
		//Reset name to file name
		int namePos = m_blendShapeFiles[i + 14].find_last_of('\\');
		std::string name = (m_blendShapeFiles[i + 14].substr(namePos + 1));
		m_meshVec[i].m_name = name;
		if (name == m_neutralName)
		{
			m_neutralIndex = i;
			m_blendWeights[i] = 1;
		}
		m_meshVec[i].m_PBRMaterial.albedo = albedo;
		m_meshVec[i].m_PBRMaterial.metallic = metallic;
		m_meshVec[i].m_PBRMaterial.roughness = roughness;
	}
	GenerateBuffers();
}

void Face::SetEffect(Effect* pEffect)
{
	m_meshVec[0].m_pEffect = pEffect;
}

void Face::GenerateBuffers()
{
	//Generate buffers
	glGenVertexArrays(1, &m_VAO);
	glGenBuffers(1, &m_VBO);
	glGenBuffers(1, &m_EBO);
}

void Face::Assemble()
{
	glBindVertexArray(m_VAO);

	unsigned int offset = 3 * m_meshVec.size() + 3;
	unsigned int alignedOffset = 0;
	//Bind VBO
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	unsigned int bufferSize = m_meshVec[0].m_posL.size() * offset * sizeof(GLfloat);
	glBufferData(GL_ARRAY_BUFFER, bufferSize, NULL, GL_STATIC_DRAW);
	for (int i = 0; i < m_meshVec.size(); i++)
	{
		glBufferSubData(GL_ARRAY_BUFFER, alignedOffset, m_meshVec[i].m_posL.size() * sizeof(vec3), &m_meshVec[i].m_posL[0]);
		alignedOffset += m_meshVec[i].m_posL.size() * sizeof(vec3);
	}
	glBufferSubData(GL_ARRAY_BUFFER, alignedOffset, m_meshVec[m_neutralIndex].m_posL.size() * sizeof(vec3), &m_meshVec[m_neutralIndex].m_normalL[0]);
	//Set EBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_meshVec[m_neutralIndex].m_indexVec.size() * sizeof(GLuint), &m_meshVec[m_neutralIndex].m_indexVec[0], GL_STATIC_DRAW);

	//Bind buffers
	unsigned int attributeCount = 0;
	alignedOffset = 0;
	// vertex positions
	for (int i = 0; i < m_meshVec.size(); i++)
	{
		std::stringstream ss;
		ss << "posL";
		ss << i;
		std::string pos = ss.str();
		GLuint POSITION = glGetAttribLocation(m_meshVec[0].m_pEffect->getShaderProgramID(), pos.c_str());
		glEnableVertexAttribArray(attributeCount);
		glVertexAttribPointer(POSITION, 3, GL_FLOAT, GL_FALSE, 0, (void*)BUFFER_OFFSET(alignedOffset));
		attributeCount++;
		alignedOffset += m_meshVec[i].m_posL.size() * sizeof(vec3);
	}
	//Normal
	GLuint NORMAL = glGetAttribLocation(m_meshVec[0].m_pEffect->getShaderProgramID(), "normalL");
	glEnableVertexAttribArray(attributeCount);
	glVertexAttribPointer(NORMAL, 3, GL_FLOAT, GL_FALSE, 0, (void*)BUFFER_OFFSET(alignedOffset));

	glBindVertexArray(0);
}

void Face::DrawPBRBlend()
{
	this->GetTransform().ComputeLocalToWorldMatrix();
	//Bind effect
	m_meshVec[0].m_pEffect->BindEffect();
	//Bind uniform
	glUniform1fv(glGetUniformLocation(m_meshVec[0].m_pEffect->getShaderProgramID(), "blendWeights"), m_meshVec.size(), &m_blendWeights[0]);

	//Update Constant Buffer(change every draw)
	//World matrix
	Effect::cbDrawPBR.world = GetMeshWorld(m_neutralIndex);
	//World inverse matrix
	Effect::cbDrawPBR.worldInvTranspose = GetMeshInverseTranspose(m_neutralIndex);
	//Materials
	Effect::cbDrawPBR.material = m_meshVec[m_neutralIndex].m_PBRMaterial;
	glBindBuffer(GL_UNIFORM_BUFFER, m_meshVec[0].m_pEffect->m_UBOid[5]);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(CBChangesEveryDrawingPBR), &Effect::cbDrawPBR, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	glBindVertexArray(m_VAO);
	glDrawElements(GL_TRIANGLES, m_meshVec[m_neutralIndex].m_indexVec.size(), GL_UNSIGNED_INT, 0);
}

void Face::DrawPBR()
{
	this->GetTransform().ComputeLocalToWorldMatrix();
	for (int i = 0; i < m_meshVec.size(); i++)
	{
		//Bind effect
		m_meshVec[i].m_pEffect->BindEffect();


		//Update Constant Buffer(change every draw)
		//World matrix
		Effect::cbDrawPBR.world = GetMeshWorld(0);
		//World inverse matrix
		Effect::cbDrawPBR.worldInvTranspose = GetMeshInverseTranspose(0);
		//Materials
		Effect::cbDrawPBR.material = m_meshVec[0].m_PBRMaterial;
		glBindBuffer(GL_UNIFORM_BUFFER, m_meshVec[0].m_pEffect->m_UBOid[5]);
		glBufferData(GL_UNIFORM_BUFFER, sizeof(CBChangesEveryDrawingPBR), &Effect::cbDrawPBR, GL_DYNAMIC_DRAW);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);


		m_meshVec[i].BindVertexArray();
		glDrawElements(GL_TRIANGLES, m_meshVec[i].m_indexVec.size(), GL_UNSIGNED_INT, 0);
	}
}

void Face::Update(float dt)
{
	
}