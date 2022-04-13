#include "AnimationScene2.h"

void AnimationScene2::Start()
{
	//Load shaders
	//Basic PBR shader
	m_basicPBRVertexShader = Shader(GL_VERTEX_SHADER, "../Project5\\Assets\\Shaders\\BasicPBRTangentAniVS.glsl");
	m_basicPBRFragShader = Shader(GL_FRAGMENT_SHADER, "../Project5\\Assets\\Shaders\\BasicPBRTangentAniFS.glsl");
	//Basic PBR sphere shader
	m_basicPBRSphereVertexShader = Shader(GL_VERTEX_SHADER, "../Project5\\Assets\\Shaders\\BasicPBRVS.glsl");
	m_basicPBRSphereFragShader = Shader(GL_FRAGMENT_SHADER, "../Project5\\Assets\\Shaders\\BasicPBRFS.glsl");

	//Load effects
	//Basic PBR effect
	m_basicPBREffect.AddShader(&m_basicPBRVertexShader, &m_basicPBRFragShader);
	m_basicPBREffect.BindShaders();

	m_basicPBRSphereEffect.AddShader(&m_basicPBRSphereVertexShader, &m_basicPBRSphereFragShader);
	m_basicPBRSphereEffect.BindShaders();

	//Load lights
	//Directional light
	DirectionalLight directLight;
	directLight.direction = vec3(-1.0f, -1.0f, -1.0f);
	directLight.lightColor = vec4(23.47f, 21.31f, 20.79f, 1.0f) / 2.0f;
	m_directLight.push_back(directLight);

	//Load GameObjects
	m_militia = Character(VertexPosNormalTangentTex, "Military_Male_03", "Military_Male_03.fbx", true);
	m_militia.GetTransform().SetScale(vec3(0.02f, 0.02f, 0.02f));
	//m_militia.GetTransform().SetRotation(vec3(glm::radians(-90.0f), 0.0f, 0.0f));
	m_militia.GetTransform().SetPosition(vec3(0.0f, -2.0f, 0.0f));
	m_militia.LoadPBRGameObject(vec3(-1.0f), -0.1f, 0.6f);
	m_militia.InitiAnimation();
	for (int i = 0; i < m_militia.GetMeshCount(); i++)
		m_militia.SetMeshEffectWithIndex(&m_basicPBREffect, i);

	//PBR material sphere
	m_sphere = Ball(VertexPosNormalTex, 1.0f, 20.0f, 20.0f, vec4(1.0f), "PBRRustedIron");
	m_sphere.GetTransform().SetPosition(vec3(0.0f, -2.0f, 0.0f));
	m_sphere.GetTransform().SetScale(vec3(0.1f, 0.1f, 0.1f));
	m_sphere.LoadPBRGameObject(vec3(1.0f), 0.1f, 0.1f);
	m_sphere.SetMeshEffectWithIndex(&m_basicPBRSphereEffect, 0);

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

void AnimationScene2::InitiScene()
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
	m_militia.InputAssemble();
	m_sphere.InputAssemble();
}

void AnimationScene2::RenderObjects()
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
	m_sphere.DrawPBR();
	m_militia.DrawPBR();
}

void AnimationScene2::DisplayScene()
{
	RenderObjects();
}

void AnimationScene2::UpdateScene()
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
	vec3 test = m_militia.GetWorldMatrix() * vec4(m_militia.m_animationVec[0].GetTarget(), 1.0f);

	m_sphere.GetTransform().SetPosition(test);


	//Update animation
	m_militia.UpdateAnimation(deltaTime, m_sphere.GetTransform().GetPosition());

	//Update time
	m_preTime = m_curTime;
}

void AnimationScene2::OnResize(int width, int height)
{
	m_camera.SetAspect(width, height);
	m_camera.SetFrustum(60.0f, m_camera.GetAspect(), 0.5f, 1000.0f);
	m_camera.SetProjPerspective();

	//Update constant buffer(change on resize)
	Effect::cbResize.proj = m_camera.GetProj();
}