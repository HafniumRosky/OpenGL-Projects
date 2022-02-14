#include "Character.h"

void Character::LoadGameObject()
{
	LoadGameObjectFromFile(m_modelPath, m_textureDir);
}

void Character::LoadPBRGameObject(vec3 albedo, float metallic, float roughness)
{
	LoadGameObjectFromFile(m_modelPath, m_textureDir);
	for (int i = 0; i < m_meshVec.size(); i++)
	{
		m_meshVec[i].m_PBRMaterial.albedo = albedo;
		m_meshVec[i].m_PBRMaterial.metallic = metallic;
		m_meshVec[i].m_PBRMaterial.roughness = roughness;
	}
}

void Character::DrawPBR()
{
	this->GetTransform().ComputeLocalToWorldMatrix();
	for (int i = 0; i < m_meshVec.size(); i++)
	{
		//Bind effect
		m_meshVec[i].m_pEffect->BindEffect();
		//Bind textures
		std::vector<GLuint> texIDVec;
		std::vector<const char*> texParaNameVec;
		std::vector<GLenum> texTarget;
		for (int j = 0; j < m_meshVec[i].m_TextureVec.size(); j++)
		{
			if (m_meshVec[i].m_TextureVec[j].type == "diffuse")
			{
				texIDVec.push_back(m_meshVec[i].m_TextureVec[j].texID);
				texParaNameVec.push_back("albedoMap");
				texTarget.push_back(GL_TEXTURE_2D);
			}
			if (m_meshVec[i].m_TextureVec[j].type == "specular")
			{
				texIDVec.push_back(m_meshVec[i].m_TextureVec[j].texID);
				texParaNameVec.push_back("metallicMap");
				texTarget.push_back(GL_TEXTURE_2D);
			}
			if (m_meshVec[i].m_TextureVec[j].type == "normal")
			{
				texIDVec.push_back(m_meshVec[i].m_TextureVec[j].texID);
				texParaNameVec.push_back("normalMap");
				texTarget.push_back(GL_TEXTURE_2D);
			}
		}
		m_meshVec[i].m_pEffect->BindTexture(texIDVec, texParaNameVec, texTarget);


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

void Character::Update(float dt)
{
	GetTransform().SetRotation(GetTransform().GetRotation() + vec3(0.0f, 0.2f * dt, 0.0f));
}