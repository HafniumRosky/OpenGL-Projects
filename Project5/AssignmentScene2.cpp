#include "AssignmentScene2.h"

void AssignmentScene2::Start()
{
	//Load shaders
	//Basic PBR shader
	m_perfectSurfaceVertexShader = Shader(GL_VERTEX_SHADER, "../Project5\\Assets\\Shaders\\PerfectSurfaceVS.glsl");
	m_perfectSurfaceFragShader = Shader(GL_FRAGMENT_SHADER, "../Project5\\Assets\\Shaders\\PerfectSurfaceFS.glsl");
	//HDR sampler shader
	m_HDRSamplerVertexShader = Shader(GL_VERTEX_SHADER, "../Project5\\Assets\\Shaders\\HDRSamplerVS.glsl");
	m_HDRSamplerFragShader = Shader(GL_FRAGMENT_SHADER, "../Project5\\Assets\\Shaders\\HDRSamplerFS.glsl");
	//HDR environment map shader
	m_HDREnvMapVertexShader = Shader(GL_VERTEX_SHADER, "../Project5\\Assets\\Shaders\\HDREnvMapVS.glsl");
	m_HDREnvMapFragShader = Shader(GL_FRAGMENT_SHADER, "../Project5\\Assets\\Shaders\\HDREnvMapFS.glsl");

	//Load effects
	//Basic PBR effect
	m_perfectSurfaceEffect.AddShader(&m_perfectSurfaceVertexShader, &m_perfectSurfaceFragShader);
	m_perfectSurfaceEffect.BindShaders();
	//HDR sampler effect
	m_HDRSamplerEffect.AddShader(&m_HDRSamplerVertexShader, &m_HDRSamplerFragShader);
	m_HDRSamplerEffect.BindShaders();
	//HDR environment map effect
	m_HDREnvMapEffect.AddShader(&m_HDREnvMapVertexShader, &m_HDREnvMapFragShader);
	m_HDREnvMapEffect.BindShaders();

	//Load GameObjects
	//PBR material sphere
	m_sphere = Ball(VertexPosNormal, 1.0f, 100.0f, 100.0f, vec4(1.0f), "PBRRustedIron");
	m_sphere.GetTransform().SetPosition(vec3(0.0f, 0.0f, 0.0f));
	m_sphere.LoadPBRGameObject();
	m_sphere.SetMeshEffectWithIndex(&m_perfectSurfaceEffect, 0);

	//Dragon
	m_dragon = Dragon(VertexPosNormal, "Dragon", "dragon.obj");
	m_dragon.GetTransform().SetPosition(vec3(0.0f, -0.5f, 0.0f));
	m_dragon.GetTransform().SetScale(vec3(0.2f));
	m_dragon.LoadPBRGameObject();
	for (int i = 0; i < m_dragon.GetMeshCount(); i++)
		m_dragon.SetMeshEffectWithIndex(&m_perfectSurfaceEffect, i);

	//HDR environment
	m_HDREnvir = Skybox(VertexPos, "Tropical_Beach_3k.hdr", "hdr");
	m_HDREnvir.GetTransform().SetPosition(vec3(0.0f));
	m_HDREnvir.LoadHDRSampler(1024, 1024);


	//Camera
	//Main Camera
	//Set frustum
	m_camera.SetFrustum(60.0f, 16.0f / 9.0f, 0.1f, 1000.0f);
	//Set transform
	//Set camera position
	//m_camera.GetTransform().SetPosition(vec3(8.0f, 8.0f, 8.0f));
	m_camera.GetTransform().SetPosition(0.0f, 0.0f, 5.0f);
	//Set camera target
	//m_camera.SetTarget(m_playerShip.GetTransform().GetPosition() + vec3(0.0f, 3.0f, 0.0f));
	m_camera.SetTarget(vec3(0.0f));
	//Compute distance to target
	m_camera.ComputeDistance();
	//Set view
	m_camera.LookAt(vec3(0.0f, 1.0f, 0.0f));
	//Set camera projection
	m_camera.SetProjPerspective();

	//HDR precomputation
	//HDR sampler camera
	m_samplerCam.SetFrustum(90.0f, 1.0f, 0.1f, 10.0f);
	m_samplerCam.GetTransform().SetPosition(0.0f, 0.0f, 0.0f);
	m_samplerCam.SetProjPerspective();
	m_samplerCam.SetFBONum(1);
	m_samplerCam.GenerateFBO();
	m_samplerCam.BindFBOWithRenderBuffer(1024, 1024, 0);
}

void AssignmentScene2::SampleHDR()
{
	m_HDREnvir.SetMeshEffectWithIndex(&m_HDRSamplerEffect, 0);
	m_HDREnvir.InputAssemble(0);
	//Sampler the HDR map for 6 times for each face
	m_samplerCam.BindFBO(0);
	glViewport(0, 0, 1024, 1024);
	glDepthMask(GL_TRUE);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	vec3 targetVec[] =
	{
		vec3(1.0f, 0.0f, 0.0f),
		vec3(-1.0f, 0.0f, 0.0f),
		vec3(0.0f, 1.0f, 0.0f),
		vec3(0.0f, -1.0f, 0.0f),
		vec3(0.0f, 0.0f, 1.0f),
		vec3(0.0f, 0.0f, -1.0f)
	};

	vec3 upVec[] =
	{
		vec3(0.0f, -1.0f, 0.0f),
		vec3(0.0f, -1.0f, 0.0f),
		vec3(0.0f, 0.0f, 1.0f),
		vec3(0.0f, 0.0f, -1.0f),
		vec3(0.0f, -1.0f, 0.0f),
		vec3(0.0f, -1.0f, 0.0f)
	};
	Effect::cbResize.proj = m_samplerCam.GetProj();
	glBindBuffer(GL_UNIFORM_BUFFER, Effect::m_UBOid[2]);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(CBChangesOnResize), &Effect::cbResize, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	for (int i = 0; i < 6; i++)
	{
		m_samplerCam.SetTarget(targetVec[i]);
		m_samplerCam.LookAt(upVec[i]);
		Effect::cbFrame.view = m_samplerCam.GetView();
		glBindBuffer(GL_UNIFORM_BUFFER, Effect::m_UBOid[1]);
		glBufferData(GL_UNIFORM_BUFFER, sizeof(CBChangesEveryFrame), &Effect::cbFrame, GL_DYNAMIC_DRAW);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
		//Bind a texture to the frame buffer
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
			GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, m_HDREnvir.GetMeshTextureID(0, 1), 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		m_HDREnvir.SampleHDR();
	}
}

void AssignmentScene2::InitiScene()
{
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

	//Precomputations
	//SphericalMap -> CubeMap
	SampleHDR();
	//Game object
	//Load buffers of meshes
	m_sphere.InputAssemble();
	m_dragon.InputAssemble();
	m_HDREnvir.SetMeshEffectWithIndex(&m_HDREnvMapEffect, 0);
	m_HDREnvir.InputAssemble(0);
}

void AssignmentScene2::RenderObjects()
{
	//Set frame buffer
	m_camera.BindFBO();
	glViewport(0, 0, m_sceneWidth, m_sceneHeight);
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
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//Set render state
	Effect::cbState.renderDepth = false;
	glBindBuffer(GL_UNIFORM_BUFFER, Effect::m_UBOid[4]);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(CBRenderState), &Effect::cbState, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	glDepthMask(GL_TRUE);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);
	if(m_renderTarget == 0)
		m_sphere.DrawPerfect(m_HDREnvir.GetMeshTextureID(0, 1), m_renderState, m_etas);
	else
		m_dragon.DrawPerfect(m_HDREnvir.GetMeshTextureID(0, 1), m_renderState, m_etas);
	//Draw Environment map
	glDisable(GL_CULL_FACE);
	glDepthFunc(GL_LEQUAL);
	m_HDREnvir.Draw(1);

}

void AssignmentScene2::DisplayScene()
{
	RenderObjects();
}

void AssignmentScene2::UpdateScene()
{
	m_curTime = glutGet(GLUT_ELAPSED_TIME);
	float deltaTime = (m_curTime - m_preTime) * 0.001f;
	//Update camera transform
		//GetTransform().SetPosition(GetTransform().GetPosition() + sync);
		//Compute distance to target
	m_camera.ComputeDistance();
	m_camera.RotateX(mouse.m_mouseState.deltaY * deltaTime);
	m_camera.RotateY(mouse.m_mouseState.deltaX * deltaTime);
	//Set camera target
	//SetTarget(target.GetPosition() + vec3(0.0f, 3.0f, 0.0f));
	//Update view
	m_camera.LookAt(vec3(0.0f, 1.0f, 0.0f));
	Effect::cbFrame.eyePos = m_camera.GetTransform().GetPosition();

	//Update parameters
	//Update etas
	if (keyboard.keyState.Q)
	{
		if (m_etas[0] <= 1.0f)
			m_etas[0] += deltaTime * 0.1f;
	}
	if (keyboard.keyState.A)
	{
		if (m_etas[0] >= 0.0f)
			m_etas[0] -= deltaTime * 0.1f;
	}
	if (keyboard.keyState.W)
	{
		if (m_etas[1] <= 1.0f)
			m_etas[1] += deltaTime * 0.1f;
	}
	if (keyboard.keyState.S)
	{
		if (m_etas[1] >= 0.0f)
			m_etas[1] -= deltaTime * 0.1f;
	}
	if (keyboard.keyState.E)
	{
		if (m_etas[2] <= 1.0f)
			m_etas[2] += deltaTime * 0.1f;
	}
	if (keyboard.keyState.D)
	{
		if (m_etas[2] >= 0.0f)
			m_etas[2] -= deltaTime * 0.1f;
	}
	if (keyboard.keyState.R)
	{
		if (m_etas[3] <= 1.0f)
			m_etas[3] += deltaTime * 0.1f;
	}
	if (keyboard.keyState.F)
	{
		if (m_etas[3] >= 0.0f)
			m_etas[3] -= deltaTime * 0.1f;
	}
	//Update renderState
	if (keyboard.keyState.N && m_renderStateTimer > 1.0f)
	{
		m_renderStateTimer = 0.0f;
		if (m_renderState != 3)
			m_renderState += 1;
		else
			m_renderState = 0;
	}
	m_renderStateTimer += deltaTime;

	//Update renderTarget
	if (keyboard.keyState.V && m_renderTargetTimer > 1.0f)
	{
		m_renderTargetTimer = 0.0f;
		if (m_renderTarget == 0)
			m_renderTarget = 1;
		else
			m_renderTarget = 0;
	}
	m_renderTargetTimer += deltaTime;

	//Update dragon
	m_dragon.Update(deltaTime);

	//Update time
	m_preTime = m_curTime;
}

void AssignmentScene2::OnResize(int width, int height)
{
	m_camera.SetAspect(width, height);
	m_camera.SetFrustum(60.0f, m_camera.GetAspect(), 0.5f, 1000.0f);
	m_camera.SetProjPerspective();

	//Update constant buffer(change on resize)
	Effect::cbResize.proj = m_camera.GetProj();
}