#include "Ship.h"

void Ship::LoadGameObject() 
{
	LoadGameObjectFromFile(m_modelPath, m_textureDir);
	//Find wheel mesh
	for (int i = 0; i < m_meshVec.size(); i++)
	{
		if (m_meshVec[i].m_name == "Wheel")
		{
			m_wheelIndex = i;
			//break;
		}
		m_meshVec[i].m_material.diffuse = vec4(1.0f);
	}
	//reset origin of the wheel
	m_meshVec[m_wheelIndex].m_childTransform.SetOrigin(vec3(0.0f, -14.95f, 0.0f));
	//Generate Depth texture
	GenerateDepthTex();
}

//Generate Depth Texture
void Ship::GenerateDepthTex()
{
	m_depthTex.GenerateDepthTexture(32, 32, GL_DEPTH_COMPONENT32F, GL_READ_WRITE);
	m_oceanDepthTex.GenerateDepthTexture(32, 32, GL_DEPTH_COMPONENT32F, GL_READ_WRITE);
	m_shipDepthTex.GenerateDepthTexture(32, 32, GL_DEPTH_COMPONENT32F, GL_READ_WRITE);
}

void Ship::Draw()
{
	this->GetTransform().ComputeLocalToWorldMatrix();
	for (int i = 0; i < m_meshVec.size(); i++)
	{
		//Bind effect
		m_meshVec[i].m_pEffect->BindEffect();
		//Bind textures
		for (int j = 0; j < m_meshVec[i].m_TextureVec.size(); j++)
		{
			std::vector<GLuint> texIDVec;
			std::vector<const char*> texParaNameVec;
			std::vector<GLenum> texTarget;
			if (m_meshVec[i].m_TextureVec[j].type == "diffuse")
			{
				//m_meshVec[i].m_pEffect->BindTexture(m_meshVec[i].m_TextureVec[j].texID, "texture1");
				texIDVec.push_back(m_meshVec[i].m_TextureVec[j].texID);
				texIDVec.push_back(m_shadowMapID);
				texParaNameVec.push_back("texture1");
				texParaNameVec.push_back("shadowMap");
				texTarget.push_back(GL_TEXTURE_2D);
				texTarget.push_back(GL_TEXTURE_2D);
				m_meshVec[i].m_pEffect->BindTexture(texIDVec, texParaNameVec, texTarget);
			}
		}

		//Update Constant Buffer(change every draw)
		//World matrix
		Effect::cbDraw.world = GetMeshWorld(i);
		//World inverse matrix
		Effect::cbDraw.worldInvTranspose = GetMeshInverseTranspose(i);
		//Material
		Effect::cbDraw.material = m_meshVec[i].m_material;
		glBindBuffer(GL_UNIFORM_BUFFER, m_meshVec[i].m_pEffect->m_UBOid[0]);
		glBufferData(GL_UNIFORM_BUFFER, sizeof(CBChangesEveryDrawing), &Effect::cbDraw, GL_DYNAMIC_DRAW);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);


		m_meshVec[i].BindVertexArray();
		glDrawElements(GL_TRIANGLES, m_meshVec[i].m_indexVec.size(), GL_UNSIGNED_INT, 0);
	}
}

void Ship::UpdateHor(vec3 wind, float dt)
{
	if (keyboard.keyState.W)
		m_moveAcc = 1.0f;
	if (keyboard.keyState.S)
		m_moveAcc = -1.0f;
	if (keyboard.keyState.A)
	{
		m_turnSpeed = 0.2f;
		m_meshVec[m_wheelIndex].m_childTransform.SetRotation(m_meshVec[m_wheelIndex].m_childTransform.GetRotation() - vec3(0.0f, 0.0f, 2.0 * dt));
		m_turnRight = true;
	}
	else
	{
		m_turnSpeed = 0.0f;
		m_turnRight = false;
	}
	if (!m_turnRight)
	{
		if (keyboard.keyState.D)
		{
			m_turnSpeed = -0.2f;
			m_meshVec[m_wheelIndex].m_childTransform.SetRotation(m_meshVec[m_wheelIndex].m_childTransform.GetRotation() + vec3(0.0f, 0.0f, 2.0f * dt));
		}
		else
			m_turnSpeed = 0.0f;
	}
	m_moveSpeed += (m_moveAcc * dt);
	if (m_moveSpeed > 10.0f)
		m_moveSpeed = 10.0f;
	if (m_moveSpeed < 0.0f)
		m_moveSpeed = 0.0f;
}

void Ship::UpdateVert(GLfloat* shipDepth, GLfloat* oceanDepth, float dt)
{
	GLfloat sum = 0;
	GLfloat sectorSum[4] = {0.0f, 0.0f, 0.0f, 0.0f };
	for (int i = 0; i < 32; i++)
	{
		for (int j = 0; j < 32; j++)
		{
			if (shipDepth[i * 32 + j] < 0.3f)
			{
				sum += (oceanDepth[i * 32 + j] - shipDepth[i * 32 + j]);
			}
			/*if (i < 16 && j < 16)
				sectorSum[0] += oceanDepth[i * 32 + j];
			else if (i < 16 && j >= 16)
				sectorSum[1] += oceanDepth[i * 32 + j];
			else if (i >= 16 && j < 16)
				sectorSum[2] += oceanDepth[i * 32 + j];
			else if (i >= 16 && j >= 16)
				sectorSum[3] += oceanDepth[i * 32 + j];*/
		}
	}
	GetTransform().SetPosition(GetTransform().GetPosition() + vec3(0.0f, m_vertSpeed * dt, 0.0f));
	m_vertAcc = sum - 0.57122f;
	m_vertAcc = min(m_vertAcc, 2.0f);
	m_vertAcc = max(m_vertAcc, -2.0f);
	m_vertSpeed += m_vertAcc * dt;
	m_vertSpeed = min(m_vertSpeed, 0.5f);
	m_vertSpeed = max(m_vertSpeed, -0.5f);
	//std::cout << sum << std::endl;
	//std::cout << 0 << " "<<sectorSum[0] << std::endl;
	//std::cout << 1 << " " <<sectorSum[1] << std::endl;
	//std::cout << 2 << " " <<sectorSum[2] << std::endl;
	//std::cout <<3 << " " << sectorSum[3] << std::endl;
	//Rotation
	//X
	/*float front = sectorSum[2] / 256.0 + sectorSum[3] / 256.0;
	float back = sectorSum[0] / 256.0 + sectorSum[1] / 256.0;
	float left = sectorSum[0] / 256.0 + sectorSum[2] / 256.0;
	float right = sectorSum[1] / 256.0 + sectorSum[3] / 256.0;
	std::cout << 0 << " " << front << std::endl;
	std::cout << 1 << " " << back << std::endl;
	std::cout << 2 << " " << left << std::endl;
	std::cout << 3 << " " << right << std::endl;
	if ((m_forwardCount > -0.2f && m_forwardCount < 0.2f) || 
		(m_forwardCount <= -0.2f && back - front > 0) || 
		(m_forwardCount >= 0.2f && back - front < 0))
	{
		GetTransform().Rotate((back - front) * 0.2f * dt, m_right);
		m_forwardCount += (back - front) * 0.2f * dt;
	}
	if ((m_rightCount > -0.2f && m_rightCount < 0.2f) ||
		(m_rightCount <= -0.2f && right - left > 0) ||
		(m_rightCount >= 0.2f && right - left < 0))
	{
		GetTransform().Rotate((right - left) * 0.2f * dt, m_forward);
		m_rightCount += (right - left) * 0.2f * dt;
	}
	//GetTransform().SetRotation(vec3((back - front) * 0.2f, GetTransform().GetRotation().y, GetTransform().GetRotation().z));
	//GetTransform().SetRotation(vec3(GetTransform().GetRotation().x, GetTransform().GetRotation().y, (left - right) * 0.1f));
	//GetTransform().SetRotation(GetTransform().GetRotation() + vec3(0.0f, 0.0f, 0.1f * dt));
	//if (back > front)
	//{
		//if(abs(back - front) > 1.0f)
			//GetTransform().SetRotation(GetTransform().GetRotation() + vec3(0.2f * dt, 0.0f, 0.0f));
	//}
	//else
	//{
		//if (abs(back - front) > 1.0f)
			//GetTransform().SetRotation(GetTransform().GetRotation() - vec3(0.2f * dt, 0.0f, 0.0f));
	//}*/
}

void Ship::UpdateTransform(float dt)
{
	m_forward = -GetTransform().GetForwardAxis();
	m_up = GetTransform().GetUpAxis();
	m_right = -GetTransform().GetRightAxis();
	GetTransform().SetPosition(GetTransform().GetPosition() + m_forward * m_moveSpeed * dt);
	//GetTransform().SetRotation(GetTransform().GetRotation() + vec3(0.0f, m_turnSpeed * dt, 0.0f));
	GetTransform().Rotate(m_turnSpeed * dt, m_up);
	//GetTransform().SetPosition(GetTransform().GetPosition() + vec3(0.0f, m_vertSpeed * dt, 0.0f));
}

vec3 Ship::Update(GLsizei size, Camera camera, float dt, vec3 wind, GLfloat* shipDepth, GLfloat* oceanDepth)
{
	if (dt <= 0.01)
	{
		if (m_health > 0)
		{
			UpdateTransform(dt);
			if (m_playerTag)
				UpdateHor(wind, dt);
			UpdateVert(shipDepth, oceanDepth, dt);
			Reload(dt);
		}
		else
		{
			Sink(dt);
		}
	}
	return m_forward * m_moveSpeed * dt;
}

void Ship::Reload(float dt)
{
	if (m_reloadTimeLeft <= 0.0f)
		m_reloadTimeLeft = 0.0f;
	else
		m_reloadTimeLeft -= dt;
	if (m_reloadTimeRight <= 0.0f)
		m_reloadTimeRight = 0.0f;
	else
		m_reloadTimeRight -= dt;
}

void Ship::Fire(vec3 viewDir, std::vector<Ball>& rounds)
{
	if (m_playerTag)
	{
		if (dot(viewDir, m_right) > dot(viewDir, -m_right) && m_reloadTimeRight <= 0.0f && mouse.m_mouseState.leftButton == true)
		{
			for (int i = 0; i < 8; i++)
			{
				Ball round = Ball(VertexPosNormalColor, 0.05f, 20.0f, 20.0f, vec4(0.5f, 0.5f, 0.5f, 1.0f));
				round.GetTransform().SetPosition(GetTransform().GetPosition() + m_right * m_cannonHorDistance
					+ m_forward * m_cannonSpacingPos[i] + m_up * m_cannonVertDistance);
				round.SetDirection(m_right);
				rounds.push_back(round);
			}
			m_reloadTimeRight = m_reloadTime;

		}
		else if (dot(viewDir, m_right) <= dot(viewDir, -m_right) && m_reloadTimeLeft <= 0.0f && mouse.m_mouseState.leftButton == true)
		{
			for (int i = 0; i < 8; i++)
			{
				Ball round = Ball(VertexPosNormalColor, 0.05f, 20.0f, 20.0f, vec4(0.5f, 0.5f, 0.5f, 1.0f));
				round.GetTransform().SetPosition(GetTransform().GetPosition() - m_right * m_cannonHorDistance
					+ m_forward * m_cannonSpacingPos[i] + m_up * m_cannonVertDistance);
				round.SetDirection(-m_right);
				rounds.push_back(round);
			}
			m_reloadTimeLeft = m_reloadTime;
		}
	}
}

void Ship::Hit()
{
	m_health -= 1;
}

void Ship::Sink(float dt)
{
	if(m_sinkTime <= 5.0f)
		GetTransform().SetRotation(GetTransform().GetRotation() + vec3(m_sinkTurnSpeed * dt, 0.0f, 0.0f));
	m_sinkTime += dt;
	m_sinkTurnSpeed -= dt * 0.04f;
	if (m_sinkTime >= 5.0f)
	{
		std::cout << "Game Over" << std::endl;
		exit(1);
	}
}