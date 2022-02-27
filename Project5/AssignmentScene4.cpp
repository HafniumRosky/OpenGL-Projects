#include "AssignmentScene4.h"

void AssignmentScene4::Start()
{
	//Load shaders
	//Texture debug shader
	m_textureDebugVertexShader = Shader(GL_VERTEX_SHADER, "../Project5\\Assets\\Shaders\\TextureDebugVS.glsl");
	m_textureDebugFragShader = Shader(GL_FRAGMENT_SHADER, "../Project5\\Assets\\Shaders\\TextureDebugFS.glsl");

	//Load effects
	//Texture debug effect
	m_textureDebugEffect.AddShader(&m_textureDebugVertexShader, &m_textureDebugFragShader);
	m_textureDebugEffect.BindShaders();

	//Load lights
	//Directional light
	DirectionalLight directLight;
	directLight.direction = vec3(-1.0f, -1.0f, -1.0f);
	directLight.lightColor = vec4(23.47f, 21.31f, 20.79f, 1.0f) / 2.0f;
	m_directLight.push_back(directLight);

	//Load GameObjects
	//Plane
	m_plane = Plane(VertexPosTex, 1, 1);
	m_plane.GetTransform().SetScale(vec3(100.0f, 100.0f, 100.0f));
	m_plane.SetTextureScale(50.0f);
	m_plane.LoadDebugObject(1024, 1024);
	m_plane.SetMeshEffectWithIndex(&m_textureDebugEffect, 0);

	//Camera

	//Main Camera
	//Set frustum
	m_camera.SetFrustum(60.0f, 16.0f / 9.0f, 0.1f, 1000.0f);
	//Set transform
	//Set camera position
	m_camera.GetTransform().SetPosition(0.0f, 0.0f, 5.0f);
	//Set camera target
	//m_camera.SetTarget(m_playerShip.GetTransform().GetPosition() + vec3(0.0f, 3.0f, 0.0f));
	m_camera.SetTarget(vec3(0.0f, 0.0f, 0.0f));
	//Compute distance to target
	m_camera.ComputeDistance();
	//Set view
	m_camera.LookAt(vec3(0.0f, 1.0f, 0.0f));
	//Set camera projection
	m_camera.SetProjPerspective();
}

void AssignmentScene4::InitiScene()
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

	//Game object
	//Load buffers of meshes
	m_plane.InputAssemble();
}

void AssignmentScene4::RenderObjects()
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
	//Draw
	m_plane.DrawDebug();
}

void AssignmentScene4::DisplayScene()
{
	RenderObjects();
}

void AssignmentScene4::UpdateScene()
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

	//Change mip map state
	if (keyboard.keyState.N && m_renderStateTimer > 0.5f)
	{
		m_renderStateTimer = 0.0f;
		m_useMipMap = !m_useMipMap;
		if (m_useMipMap)
			glUniform1i(glGetUniformLocation(m_textureDebugEffect.getShaderProgramID(), "useMipMap"), 1);
		else
			glUniform1i(glGetUniformLocation(m_textureDebugEffect.getShaderProgramID(), "useMipMap"), 0);
	}
	m_renderStateTimer += deltaTime;
	

	//Update time
	m_preTime = m_curTime;
}

void AssignmentScene4::OnResize(int width, int height)
{
	m_camera.SetAspect(width, height);
	m_camera.SetFrustum(60.0f, m_camera.GetAspect(), 0.5f, 1000.0f);
	m_camera.SetProjPerspective();

	//Update constant buffer(change on resize)
	Effect::cbResize.proj = m_camera.GetProj();
}

void AssignmentScene4::GUIDisplay()
{
	ImGui::Begin("Mip Map Manager");
	ImGui::SetWindowSize(ImVec2((float)500, (float)100));
	ImGui::Checkbox("Use mip map (Press N)", &m_useMipMap);
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	ImGui::End();
}