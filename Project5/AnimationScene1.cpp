#include "AnimationScene1.h"

void AnimationScene1::Start()
{
	//Load shaders
	//Basic phong shdaer
	m_phongVertexShader = Shader(GL_VERTEX_SHADER, "../Project5\\Assets\\Shaders\\ShipVS.glsl");
	m_phongFragShader = Shader(GL_FRAGMENT_SHADER, "../Project5\\Assets\\Shaders\\ShipFS.glsl");
	//Skybox shader
	m_skyboxVertexShader = Shader(GL_VERTEX_SHADER, "../Project5\\Assets\\Shaders\\SkyboxVS.glsl");
	m_skyboxFragShader = Shader(GL_FRAGMENT_SHADER, "../Project5\\Assets\\Shaders\\SkyboxFS.glsl");

	//Load effects
	//Basic phong lighting effect
	m_phongEffect.AddShader(&m_phongVertexShader, &m_phongFragShader);
	m_phongEffect.BindShaders();
	//Skybox rendering effect
	m_skyboxEffect.AddShader(&m_skyboxVertexShader, &m_skyboxFragShader);
	m_skyboxEffect.BindShaders();

	//Load lights
	//Directional light
	DirectionalLight directLight;
	directLight.direction = vec3(1.0f, -1.0f, 1.0f);
	directLight.lightColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
	m_directLight.push_back(directLight);

	//Load GameObjects
	m_airplane = AirPlane(VertexPosNormalTex, "LightPlane", "LightPlane.fbx");
	m_airplane.GetTransform().SetScale(vec3(0.01f, 0.01f, 0.01f));
	m_airplane.GetTransform().SetPosition(vec3(0.0f, 0.0f, 0.0f));
	m_airplane.LoadGameObject();
	for (int i = 0; i < m_airplane.GetMeshCount(); i++)
		m_airplane.SetMeshEffectWithIndex(&m_phongEffect, i);

	//Camera

	//Main Camera
	//Set frustum
	m_camera.SetFrustum(60.0f, 16.0f / 9.0f, 0.1f, 1000.0f);
	//Set transform
	//Set camera position
	m_camera.GetTransform().SetPosition(0.0f, 0.0f, 10.0f);
	//Set camera target
	//m_camera.SetTarget(m_playerShip.GetTransform().GetPosition() + vec3(0.0f, 3.0f, 0.0f));
	m_camera.SetTarget(vec3(0.0f, 0.0f, 0.0f));
	//Compute distance to target
	m_camera.ComputeDistance();
	//Set view
	m_camera.LookAt(vec3(0.0f, 1.0f, 0.0f));
	//Set camera projection
	m_camera.SetProjPerspective();

	//Skybox
	m_skybox = Skybox(VertexPos, "Sunny1", "png");
	m_skybox.GetTransform().SetScale(vec3(1000.0f, 1000.0f, 1000.0f));
	m_skybox.GetTransform().SetPosition(m_camera.GetTransform().GetPosition());
	m_skybox.LoadGameObject();
	m_skybox.SetMeshEffectWithIndex(&m_skyboxEffect, 0);
}

void AnimationScene1::InitiScene()
{
	//Game object
	//Load buffers of meshes
	m_skybox.InputAssemble();
	m_airplane.InputAssemble();
	//Initialize constant buffer

	//
	//Set constant buffer(change every draw)
	//

	//Bind constant buffer
	Effect::BindConstantBuffer(&Effect::cbDraw, 0);

	//
	//Set constant buffer(change every frame)
	//

	//View matrix
	Effect::cbFrame.view = m_camera.GetView();
	//Eye position
	Effect::cbFrame.eyePos = m_camera.GetTransform().GetPosition();
	//Bind constant buffer
	Effect::BindConstantBuffer(&Effect::cbFrame, 1);

	//
	//Set constant buffer(change on resize)
	//

	//Project matrix
	Effect::cbResize.proj = m_camera.GetProj();
	//Bind constant buffer
	Effect::BindConstantBuffer(&Effect::cbResize, 2);

	//
	//Set constant buffer(change rarely)
	//

	//Directional light number
	Effect::cbRare.numDirLight = m_directLight.size();
	//Point light number
	Effect::cbRare.numPointLight = m_pointLight.size();
	//Directional light
	for (int j = 0; j < m_directLight.size(); j++)
		Effect::cbRare.dirLight[j] = m_directLight[j];
	//Point light
	for (int j = 0; j < m_pointLight.size(); j++)
		Effect::cbRare.pointLight[j] = m_pointLight[j];
	//Bind constant buffer
	Effect::BindConstantBuffer(&Effect::cbRare, 3);

	//
	//Set constant buffer(RenderState)
	//
	//Bind constant buffer
	Effect::BindConstantBuffer(&Effect::cbState, 4);

	//
	//Set constant buffer(change every draw PBR)
	//
	Effect::BindConstantBuffer(&Effect::cbDrawPBR, 5);

}

void AnimationScene1::RenderObjects()
{
	//Set camera proj view
	Effect::cbFrame.view = m_camera.GetView();
	Effect::cbResize.proj = m_camera.GetProj();
	glBindBuffer(GL_UNIFORM_BUFFER, Effect::m_UBOid[1]);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(CBChangesEveryFrame), &Effect::cbFrame, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	glBindBuffer(GL_UNIFORM_BUFFER, Effect::m_UBOid[2]);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(CBChangesOnResize), &Effect::cbResize, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	//Enable depth test
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	//Set clear flags
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//Set render state
	Effect::cbState.renderDepth = false;
	glBindBuffer(GL_UNIFORM_BUFFER, Effect::m_UBOid[4]);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(CBRenderState), &Effect::cbState, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	glDepthMask(GL_TRUE);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	m_airplane.Draw();
	//Disable depth write
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	m_skybox.Draw(0);
}

void AnimationScene1::DisplayScene()
{
	RenderObjects();
}

void AnimationScene1::UpdateScene()
{
	m_curTime = glutGet(GLUT_ELAPSED_TIME);
	float deltaTime = m_curTime - m_preTime;

	//Update Airplane
	m_airplane.Update(deltaTime * 0.001f);

	//Update camera transform
		//GetTransform().SetPosition(GetTransform().GetPosition() + sync);
		//Compute distance to target
	if (m_camera.getCameraViewState())
	{
		m_camera.ComputeDistance();
		m_camera.RotateX(mouse.m_mouseState.deltaY * deltaTime * 0.001f);
		m_camera.RotateY(mouse.m_mouseState.deltaX * deltaTime * 0.001f);
		//Update view
		m_camera.LookAt(vec3(0.0f, 1.0f, 0.0f));
	}
	else
	{
		m_camera.GetTransform().SetPosition(m_airplane.GetTransform().GetPosition() + m_airplane.GetTransform().GetRightAxis() * 1.45f);
		m_camera.SetTarget(m_camera.GetTransform().GetPosition() + m_airplane.GetTransform().GetRightAxis());
		//Update view
		m_camera.LookAt(m_airplane.GetTransform().GetUpAxis());
	}
	Effect::cbFrame.eyePos = m_camera.GetTransform().GetPosition();

	//Set camera view
	if (keyboard.keyState.V && m_camStateTimer > 1.0f)
	{
		m_camStateTimer = 0.0f;
		if (m_camera.getCameraViewState())
			m_camera.SetFirstPersonCam();
		else
		{
			m_camera.SetThirdPersonCam();
			m_camera.GetTransform().SetPosition(0.0f, 0.0f, 10.0f);
			m_camera.SetTarget(vec3(0.0f, 0.0f, 0.0f));
			m_camera.ComputeDistance();
			m_camera.LookAt(vec3(0.0f, 1.0f, 0.0f));
		}
	}
	m_camStateTimer += deltaTime * 0.001f;

	//Update time
	m_preTime = m_curTime;
}

void AnimationScene1::OnResize(int width, int height)
{
	m_camera.SetAspect(width, height);
	m_camera.SetFrustum(60.0f, m_camera.GetAspect(), 0.5f, 1000.0f);
	m_camera.SetProjPerspective();

	//Update constant buffer(change on resize)
	Effect::cbResize.proj = m_camera.GetProj();
}