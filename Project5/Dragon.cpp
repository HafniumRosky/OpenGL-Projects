#include "Dragon.h"

void Dragon::LoadPBRGameObject(vec3 albedo, float metallic, float roughness)
{
	LoadGameObjectFromFile(m_modelPath, m_textureDir);
}

void Dragon::DrawPerfect(GLuint cubeTexID, int renderState, float* etas)
{
	this->GetTransform().ComputeLocalToWorldMatrix();
	for (int i = 0; i < m_meshVec.size(); i++)
	{
		//Bind effect
		m_meshVec[i].m_pEffect->BindEffect();
		//Bind uniform
		glUniform1i(glGetUniformLocation(m_meshVec[i].m_pEffect->getShaderProgramID(), "renderState"), renderState);
		glUniform1f(glGetUniformLocation(m_meshVec[i].m_pEffect->getShaderProgramID(), "eta"), etas[0]);
		glUniform1f(glGetUniformLocation(m_meshVec[i].m_pEffect->getShaderProgramID(), "etaR"), etas[1]);
		glUniform1f(glGetUniformLocation(m_meshVec[i].m_pEffect->getShaderProgramID(), "etaG"), etas[2]);
		glUniform1f(glGetUniformLocation(m_meshVec[i].m_pEffect->getShaderProgramID(), "etaB"), etas[3]);
		//Bind texture
		std::vector<GLuint> texIDVec;
		std::vector<const char*> texParaNameVec;
		std::vector<GLenum> texTarget;
		texIDVec.push_back(cubeTexID);
		texParaNameVec.push_back("cubeMap");
		texTarget.push_back(GL_TEXTURE_CUBE_MAP);
		m_meshVec[i].m_pEffect->BindTexture(texIDVec, texParaNameVec, texTarget);
		//Update Constant Buffer(change every draw)
		//World matrix
		Effect::cbDrawPBR.world = GetMeshWorld(i);
		//World inverse matrix
		Effect::cbDrawPBR.worldInvTranspose = GetMeshInverseTranspose(i);
		//Materials
		Effect::cbDrawPBR.material = m_meshVec[i].m_PBRMaterial;
		glBindBuffer(GL_UNIFORM_BUFFER, m_meshVec[i].m_pEffect->m_UBOid[5]);
		glBufferData(GL_UNIFORM_BUFFER, sizeof(CBChangesEveryDrawingPBR), &Effect::cbDrawPBR, GL_DYNAMIC_DRAW);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
		m_meshVec[i].BindVertexArray();
		glDrawElements(GL_TRIANGLES, m_meshVec[i].m_indexVec.size(), GL_UNSIGNED_INT, 0);
	}
}

void Dragon::Update(float dt)
{
	GetTransform().SetRotation(GetTransform().GetRotation() + vec3(0.0f, 0.2f * dt, 0.0f));
}