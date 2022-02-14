#include "Camera.h"

void Camera::GenerateFBO()
{
	GLuint temp;
	for (int i = 0; i < m_FBOnum; i++)
	{
		glGenFramebuffers(1, &temp);
		m_FBO.push_back(temp);
	}
}

void Camera::InitiRenderTexture(GLsizei width, GLsizei height)
{
	m_width = width;
	m_height = height;
	m_renderTexture.SetFBO(m_FBO[0]);
	m_renderTexture.GenerateTexture(m_width, m_height, GL_R32F, GL_READ_WRITE);
}

void Camera::BindFBO()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Camera::BindFBO(int index)
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_FBO[index]);
}

void Camera::BindTextureToFBO(int index)
{
	BindFBO(index);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_renderTexture.GetTextureID(), 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Camera::BindTextureToFBO(RenderTexture rt, int index)
{
	BindFBO(index);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, rt.GetTextureID(), 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Camera::BindDepthBufferWithTexToFBO(RenderTexture rt, int index)
{
	BindFBO(index);
	//attach tex to buffer
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, rt.GetTextureID(), 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Camera::BindFBOWithRenderBuffer(GLsizei width, GLsizei height, int index)
{
	BindFBO(index);
	unsigned int captureRBO;
	glGenRenderbuffers(1, &captureRBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, captureRBO);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Camera::smoothTranslate(Transform target, float dt, vec3 sync)
{
	vec3 CaptainPos = target.GetPosition() + vec3(0.0f, 1.8f, 0.0f) + 4.2f * target.GetForwardAxis();
	vec3 targetCent = target.GetPosition() + vec3(0.0f, 3.0f, 0.0f);
	if (m_cameraMoving)
	{
		if (glm::length(m_translationBuffer) > 5.0f)
		{
			m_TranslateSpeed = 10.0f;
			m_targetSpeed = 5.0f;
		}
		else if (glm::length(m_translationBuffer) < 1.0f)
		{
			m_TranslateSpeed = 1.5f;
			m_targetSpeed = 1.5f;
		}
		else
		{
			m_TranslateSpeed = glm::length(m_translationBuffer) * 3.0f;
			m_targetSpeed = glm::length(m_translationBuffer);
		}

		if (m_isCaptain)
		{
			if (m_TranslateSpeed * dt > glm::length(m_translationBuffer))
			{
				GetTransform().SetPosition(CaptainPos);
				m_cameraMoving = false;
			}
			else
			{
				GetTransform().SetPosition(GetTransform().GetPosition() + m_TranslateSpeed * dt * glm::normalize(m_translationBuffer) + sync);
				m_translationBuffer = CaptainPos - GetTransform().GetPosition();
			}
			SetTarget(GetTransform().GetPosition() + GetTransform().GetForwardAxis());
		}
		else
		{
			if (m_TranslateSpeed * dt > glm::length(m_translationBuffer))
			{
				GetTransform().SetPosition(GetTransform().GetPosition() + m_translationBuffer.x * (-GetTransform().GetForwardAxis()) + sync);
				m_cameraMoving = false;
			}
			else
			{
				GetTransform().SetPosition(GetTransform().GetPosition() + m_TranslateSpeed * dt * (-GetTransform().GetForwardAxis()) + sync);
				m_translationBuffer.x -= m_TranslateSpeed * dt;
			}
			if (m_targetSpeed * dt > glm::length(m_targetBuffer))
			{
				SetTarget(targetCent);
			}
			else
			{
				SetTarget(targetCent - m_targetBuffer + m_targetSpeed * dt * glm::normalize(m_targetBuffer));
				m_targetBuffer -= m_targetSpeed * dt * glm::normalize(m_targetBuffer);
			}
		}
	}
	else
	{
		if (m_isCaptain)
		{
			GetTransform().SetPosition(CaptainPos);
			SetTarget(GetTransform().GetPosition() + GetTransform().GetForwardAxis());
		}
		else
		{
			GetTransform().SetPosition(GetTransform().GetPosition() + sync);
			SetTarget(targetCent);
		}
	}
}

void Camera::Update(Transform target, vec3 sync, float dt)
{
	vec3 CaptainPos = target.GetPosition() + vec3(0.0f, 1.8f, 0.0f) + 4.2f * target.GetForwardAxis();
	if (!m_cameraMoving)
	{
		if (keyboard.keyState.V)
		{
			m_cameraMoving = true;
			m_isCaptain = !m_isCaptain;
			if (m_isCaptain)
				m_translationBuffer = CaptainPos - GetTransform().GetPosition();
			else
			{
				m_translationBuffer = vec3(15.0f, 0.0f, 0.0f);
				m_targetBuffer = target.GetPosition() - (GetTransform().GetPosition() + GetTransform().GetForwardAxis()) + vec3(0.0f, 3.0f, 0.0f);
			}
		}
	}
	smoothTranslate(target, dt, sync);
	if (m_isCaptain)
	{
		//Update camera transform
		//GetTransform().SetPosition(CaptainPos);
		GetTransform().SetRotation(GetTransform().GetRotation() - vec3(mouse.m_mouseState.deltaY * dt, 0.0f, 0.0f));
		GetTransform().SetRotation(GetTransform().GetRotation() - vec3(0.0f, mouse.m_mouseState.deltaX * dt, 0.0f));
		if (GetTransform().GetRotation().x > radians(30.0f))
			GetTransform().SetRotation(vec3(radians(30.0f), GetTransform().GetRotation().y, GetTransform().GetRotation().z));
		else if (GetTransform().GetRotation().x < -radians(30.0f))
			GetTransform().SetRotation(vec3(-radians(30.0f), GetTransform().GetRotation().y, GetTransform().GetRotation().z));
		//Set camera target
		//SetTarget(GetTransform().GetPosition() + GetTransform().GetForwardAxis());
		//Update view
		LookAt(vec3(0.0f, 1.0f, 0.0f));
	}
	else
	{
		//Update camera transform
		//GetTransform().SetPosition(GetTransform().GetPosition() + sync);
		//Compute distance to target
		ComputeDistance();
		RotateX(mouse.m_mouseState.deltaY * dt);
		RotateY(mouse.m_mouseState.deltaX * dt);
		//Set camera target
		//SetTarget(target.GetPosition() + vec3(0.0f, 3.0f, 0.0f));
		//Update view
		LookAt(vec3(0.0f, 1.0f, 0.0f));
	}
}

mat4 Camera::GetView() const
{
	return m_view;
}

void Camera::LookAt(const vec3& up)
{
	m_view = this->GetTransform().LookAt(m_target, up);
}

mat4 Camera::GetProj() const
{
	return m_proj;
}

void Camera::SetProjPerspective()
{
	m_proj = perspective(glm::radians(m_fovY), m_aspect, m_nearZ, m_farZ);
}

void Camera::SetProjOrtho()
{
	m_proj = glm::ortho(m_left, m_right, m_bottom, m_top, m_nearZ, m_farZ);
}

mat4 Camera::GetViewProj()
{
	return GetProj() * GetView();
}

ViewPort Camera::GetViewPort() const
{
	return m_viewPort;
}

void Camera::SetFrustum(GLfloat fovY, GLfloat aspect, GLfloat nearZ, GLfloat farZ)
{
	m_fovY = fovY;
	m_aspect = aspect;
	m_nearZ = nearZ;
	m_farZ = farZ;
}

void Camera::SetFrustum(GLfloat left, GLfloat right, GLfloat bottom, GLfloat top, GLfloat nearZ, GLfloat farZ)
{
	m_left = left;
	m_right = right;
	m_bottom = bottom;
	m_top = top;
	m_nearZ = nearZ;
	m_farZ = farZ;
}

void Camera::SetViewPort(GLfloat topLeftX, GLfloat topLeftY, GLfloat width, GLfloat height, GLfloat minDepth, GLfloat maxDepth)
{
	m_viewPort.TopLeftX = topLeftX;
	m_viewPort.TopLeftY = topLeftY;
	m_viewPort.Width = width;
	m_viewPort.Height = height;
	m_viewPort.MinDepth = minDepth;
	m_viewPort.MaxDepth = maxDepth;
}

void Camera::RotateX(float rad)
{
	vec3 rotation = m_transform.GetRotation();
	rotation.x -= rad;
	if (rotation.x > 0.0f)
		rotation.x = 0.0f;
	else if (rotation.x < -radians(30.0f))
		rotation.x = -radians(30.0f);
	m_transform.SetRotation(rotation);
	m_transform.SetPosition(m_target);
	m_transform.Translate(m_transform.GetForwardAxis(), -m_distance);
	m_transform.UpdateEularRotationMatrix();
}

void Camera::RotateY(float rad)
{
	vec3 rotation = m_transform.GetRotation();
	rotation.y -= rad;
	m_transform.SetRotation(rotation);
	m_transform.SetPosition(m_target);
	m_transform.Translate(m_transform.GetForwardAxis(), -m_distance);
	m_transform.UpdateEularRotationMatrix();
}