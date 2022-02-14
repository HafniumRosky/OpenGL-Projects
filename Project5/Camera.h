#ifndef CAMERA_H
#define CAMERA_H

#include "Transform.h"
#include "RenderTexture.h"
#include "Mouse.h"
#include "Keyboard.h"

extern Mouse mouse;
extern Keyboard keyboard;

struct ViewPort
{
	GLfloat TopLeftX;
	GLfloat TopLeftY;
	GLfloat Width;
	GLfloat Height;
	GLfloat MinDepth;
	GLfloat MaxDepth;
};

class Camera
{
protected:
	// Camera transform property
	Transform m_transform;

	//Render target
	std::vector<GLuint> m_FBO;
	int m_FBOnum = 1;
	RenderTexture m_renderTexture;
	GLsizei m_width = 32;
	GLsizei m_height = 32;

	// Frustum Property
	GLfloat m_nearZ = 0.0f;
	GLfloat m_farZ = 0.0f;
	GLfloat m_aspect = 0.0f;
	GLfloat m_fovY = 0.0f;

	GLfloat m_left = 0.0f;
	GLfloat m_right = 0.0f;
	GLfloat m_bottom = 0.0f;
	GLfloat m_top = 0.0f;

	// ViewPort of the camera
	ViewPort m_viewPort = {};

	mat4 m_view = mat4(1.0f);
	mat4 m_proj = mat4(1.0f);

	vec3 m_target;
	float m_distance;

	//Camera state
	bool m_isCaptain = true;
	vec3 m_translationBuffer = vec3(0.0f);
	bool m_cameraMoving = false;
	float m_TranslateSpeed = 0.0f;
	vec3 m_targetBuffer = vec3(0.0f);
	float m_targetSpeed = 0.0f;

	bool m_thirdPersonViewState = true;

	void smoothTranslate(Transform target, float dt, vec3 sync);

public:

	Camera() = default;

	//Custom render target
	void GenerateFBO();
	void BindFBO();
	void BindFBO(int index);
	void InitiRenderTexture(GLsizei width, GLsizei height);
	void BindTextureToFBO(int index);
	//Bind a texture which is not camera render texture
	void BindTextureToFBO(RenderTexture rt, int index);
	void BindDepthBufferWithTexToFBO(RenderTexture rt, int index);
	void BindFBOWithRenderBuffer(GLsizei width, GLsizei height, int index);

	void Update(Transform target, vec3 sync, float dt);



	// View and projection matrics accessors

	mat4 GetView() const;
	void LookAt(const vec3& up);
	mat4 GetProj() const;
	void SetProjPerspective();
	void SetProjOrtho();
	mat4 GetViewProj();

	// Get viewport on the screen
	ViewPort GetViewPort() const;



	// Set frustum properties
	void SetFrustum(GLfloat fovY, GLfloat aspect, GLfloat nearZ, GLfloat farZ);
	void SetFrustum(GLfloat left, GLfloat right, GLfloat bottom, GLfloat top, GLfloat nearZ, GLfloat farZ);

	// Set Viewport
	void SetViewPort(GLfloat topLeftX, GLfloat topLeftY, GLfloat width, GLfloat height, GLfloat minDepth = 0.0f, GLfloat maxDepth = 1.0f);

	// This function will acess transform property of the object
	Transform& GetTransform()
	{
		return m_transform;
	}
	// This function will get transform property of the object
	const Transform& GetTransform() const
	{
		return m_transform;
	}

	void RotateX(float rad);
	void RotateY(float rad);

	void SetTarget(vec3 target)
	{
		m_target = target;
	}

	void SetAspect(int width, int height)
	{
		m_aspect = float(width) / height;
	}

	GLfloat GetAspect()
	{
		return m_aspect;
	}

	vec3 GetTarget()
	{
		return m_target;
	}

	void ComputeDistance()
	{
		m_distance = distance(m_target, m_transform.GetPosition());
	}

	GLuint GetRenderTextureID()
	{
		return m_renderTexture.GetTextureID();
	}

	RenderTexture GetRenderTexture()
	{
		return m_renderTexture;
	}

	GLfloat GetNearZ()
	{
		return m_nearZ;
	}

	GLfloat GetFarZ()
	{
		return m_farZ;
	}

	void SetFBONum(int num)
	{
		m_FBOnum = num;
	}

	void SetThirdPersonCam()
	{
		m_thirdPersonViewState = true;
	}

	void SetFirstPersonCam()
	{
		m_thirdPersonViewState = false;
	}

	bool getCameraViewState()
	{
		return m_thirdPersonViewState;
	}
};

#endif