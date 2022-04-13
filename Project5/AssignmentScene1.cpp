#include "AssignmentScene1.h"

void AssignmentScene1::Start()
{
	//Load shaders
	//Basic PBR shader
	m_basicPBRVertexShader = Shader(GL_VERTEX_SHADER, "../Project5\\Assets\\Shaders\\BasicPBRNoTexVS.glsl");
	m_basicPBRFragShader = Shader(GL_FRAGMENT_SHADER, "../Project5\\Assets\\Shaders\\BasicPBRNoTexFS.glsl");
	//Basic phong shdaer
	m_phongVertexShader = Shader(GL_VERTEX_SHADER, "../Project5\\Assets\\Shaders\\CannonBallVS.glsl");
	m_phongFragShader = Shader(GL_FRAGMENT_SHADER, "../Project5\\Assets\\Shaders\\CannonBallFS.glsl");
	//Anime shader
	m_animeVertexShader = Shader(GL_VERTEX_SHADER, "../Project5\\Assets\\Shaders\\AnimeVS.glsl");
	m_animeFragShader = Shader(GL_FRAGMENT_SHADER, "../Project5\\Assets\\Shaders\\AnimeFS.glsl");
	//Outline shader
	m_outlineVertexShader = Shader(GL_VERTEX_SHADER, "../Project5\\Assets\\Shaders\\OutlineVS.glsl");
	m_outlineFragShader = Shader(GL_FRAGMENT_SHADER, "../Project5\\Assets\\Shaders\\OutlineFS.glsl");

	//Load effects
	//Basic PBR effect
	m_basicPBREffect.AddShader(&m_basicPBRVertexShader, &m_basicPBRFragShader);
	m_basicPBREffect.BindShaders();
	//Basic phong lighting effect
	m_phongEffect.AddShader(&m_phongVertexShader, &m_phongFragShader);
	m_phongEffect.BindShaders();
	//Anime effect
	m_animeEffect.AddShader(&m_animeVertexShader, &m_animeFragShader);
	m_animeEffect.BindShaders();
	//Outline effect
	m_outlineEffect.AddShader(&m_outlineVertexShader, &m_outlineFragShader);
	m_outlineEffect.BindShaders();

	//Load lights
	PointLight pointLight;
	pointLight.lightColor = vec4(23.47f, 21.31f, 20.79f, 1.0f);
	pointLight.position = vec3(0.0, 0.0, 3.0);
	pointLight.att = vec3(0.0f, 0.0f, 1.0f);
	m_pointLight.push_back(pointLight);

	//Load GameObjects
	//PBR material Cube
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			m_Cube[i * 5 + j] = Cube(VertexPosNormal);
			m_Cube[i * 5 + j].GetTransform().SetPosition(vec3(-6.5f + j * 2.0f, 4.0f - i * 2.0f, 0.0f));
			m_Cube[i * 5 + j].GetTransform().SetRotation(vec3(0.7f, 0.0f, 0.7f));
			m_Cube[i * 5 + j].LoadPBRGameObject(vec3(1.0, 1.0f, 1.0f), 0.1f + j * 0.2f, 0.1f + i * 0.2f);
			m_Cube[i * 5 + j].SetMeshEffectWithIndex(&m_basicPBREffect, 0);
		}
	}

	//PBR material Sphere
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			m_Sphere[i * 5 + j] = Ball(VertexPosNormal, 0.5f, 20.0f, 20.0f, vec4(1.0f), "PBRRustedIron");
			m_Sphere[i * 5 + j].GetTransform().SetPosition(vec3(-6.5f + j * 2.0f, 4.0f - i * 2.0f, 0.0f));
			m_Sphere[i * 5 + j].LoadPBRGameObject(vec3(1.0, 1.0f, 1.0f), 0.1f + j * 0.2f, 0.1f + i * 0.2f);
			m_Sphere[i * 5 + j].SetMeshEffectWithIndex(&m_basicPBREffect, 0);
		}
	}

	//Phong material cube
	for (int i = 25; i < 30; i++)
	{
		m_Cube[i] = Cube(VertexPosNormalColor);
		m_Cube[i].GetTransform().SetPosition(vec3(4.5f, 4.0f - (i - 25) * 2.0f, 0.0f));
		m_Cube[i].GetTransform().SetRotation(vec3(0.7f, 0.0f, 0.7f));
		m_Cube[i].LoadGameObject(8.0f + (i - 25) * 49.0f);
		m_Cube[i].SetMeshEffectWithIndex(&m_phongEffect, 0);
	}

	//Phong material sphere
	for (int i = 25; i < 30; i++)
	{
		m_Sphere[i] = Ball(VertexPosNormalColor, 0.5f, 20.0f, 20.0f, vec4(1.0f), "PBRRustedIron");
		m_Sphere[i].GetTransform().SetPosition(vec3(4.5f, 4.0f - (i - 25) * 2.0f, 0.0f));
		m_Sphere[i].LoadGameObject(vec4(1.0f), 8.0f + (i - 25) * 49.0f);
		m_Sphere[i].SetMeshEffectWithIndex(&m_phongEffect, 0);
	}

	//Anime material cube
	for (int i = 30; i < 35; i++)
	{
		m_Cube[i] = Cube(VertexPosNormal);
		m_Cube[i].GetTransform().SetPosition(vec3(6.5f, 4.0f - (i - 30) * 2.0f, 0.0f));
		m_Cube[i].GetTransform().SetRotation(vec3(0.7f, 0.0f, 0.7f));
		m_Cube[i].LoadGameObject(8.0f + (i - 30) * 49.0f);
		
	}

	//Anime material Sphere
	for (int i = 30; i < 35; i++)
	{
		m_Sphere[i] = Ball(VertexPosNormal, 0.5f, 20.0f, 20.0f, vec4(1.0f), "PBRRustedIron");
		m_Sphere[i].GetTransform().SetPosition(vec3(6.5f, 4.0f - (i - 30) * 2.0f, 0.0f));
		m_Sphere[i].LoadGameObject(vec4(1.0f), 8.0f + (i - 30) * 49.0f);
		
	}

	//Camera
	//Main Camera
	//Set frustum
	m_camera.SetFrustum(60.0f, 16.0f / 9.0f, 0.1f, 1000.0f);
	//Set transform
	//Set camera position
	//m_camera.GetTransform().SetPosition(vec3(8.0f, 8.0f, 8.0f));
	m_camera.GetTransform().SetPosition(0.0f, 0.0f, 10.0f);
	//Set camera target
	//m_camera.SetTarget(m_playerShip.GetTransform().GetPosition() + vec3(0.0f, 3.0f, 0.0f));
	m_camera.SetTarget(vec3(0.0f));
	//Compute distance to target
	m_camera.ComputeDistance();
	//Set view
	m_camera.LookAt(vec3(0.0f, 1.0f, 0.0f));
	//Set camera projection
	m_camera.SetProjPerspective();
}

void AssignmentScene1::InitiScene()
{
	//Game object
	//Load buffers of meshes
	for (int i = 0; i < 30; i++)
	{
		m_Cube[i].InputAssemble();
	}

	//PBR material Sphere
	for (int i = 0; i < 30; i++)
	{
		m_Sphere[i].InputAssemble();
	}
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

void AssignmentScene1::RenderObjects()
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
	if (m_renderCube)
	{
		for (int i = 0; i < 25; i++)
		{
			m_Cube[i].DrawPBR();
		}
		for (int i = 25; i < 30; i++)
		{
			m_Cube[i].Draw();
		}
		for (int i = 30; i < 35; i++)
		{
			glCullFace(GL_FRONT);
			glEnable(GL_CULL_FACE);
			m_Cube[i].SetMeshEffectWithIndex(&m_outlineEffect, 0);
			m_Cube[i].InputAssemble();
			m_Cube[i].Draw();
			glDisable(GL_CULL_FACE);
			m_Cube[i].SetMeshEffectWithIndex(&m_animeEffect, 0);
			m_Cube[i].InputAssemble();
			m_Cube[i].Draw();
		}
	}
	else
	{
		for (int i = 0; i < 25; i++)
		{
			m_Sphere[i].DrawPBR();
		}
		for (int i = 25; i < 30; i++)
		{
			m_Sphere[i].Draw();
		}
		for (int i = 30; i < 35; i++)
		{
			glCullFace(GL_FRONT);
			glEnable(GL_CULL_FACE);
			m_Sphere[i].SetMeshEffectWithIndex(&m_outlineEffect, 0);
			m_Sphere[i].InputAssemble();
			m_Sphere[i].Draw();
			glDisable(GL_CULL_FACE);
			m_Sphere[i].SetMeshEffectWithIndex(&m_animeEffect, 0);
			m_Sphere[i].InputAssemble();
			m_Sphere[i].Draw();
		}
	}
}

void AssignmentScene1::DisplayScene()
{
	RenderObjects();
}

void AssignmentScene1::UpdateScene()
{
	m_curTime = glutGet(GLUT_ELAPSED_TIME);
	float deltaTime = m_curTime - m_preTime;
	//Update camera transform
		//GetTransform().SetPosition(GetTransform().GetPosition() + sync);
		//Compute distance to target
	m_camera.ComputeDistance();
	m_camera.RotateX(mouse.m_mouseState.deltaY * deltaTime * 0.001f);
	m_camera.RotateY(mouse.m_mouseState.deltaX * deltaTime * 0.001f);
	//Set camera target
	//SetTarget(target.GetPosition() + vec3(0.0f, 3.0f, 0.0f));
	//Update view
	m_camera.LookAt(vec3(0.0f, 1.0f, 0.0f));
	Effect::cbFrame.eyePos = m_camera.GetTransform().GetPosition();

	//Update Objects
	for (int i = 0; i < 35; i++)
	{
		m_Cube[i].Update(deltaTime * 0.001f);
	}
	if (m_lightMovingRight)
		m_pointLight[0].position.x += deltaTime * 0.001f * 2;
	else
		m_pointLight[0].position.x -= deltaTime * 0.001f * 2;
	if (m_pointLight[0].position.x > 6.5)
		m_lightMovingRight = false;
	if (m_pointLight[0].position.x < -6.5)
		m_lightMovingRight = true;
	Effect::cbRare.pointLight[0] = m_pointLight[0];
	glBindBuffer(GL_UNIFORM_BUFFER, Effect::m_UBOid[3]);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(CBChangesRarely), &Effect::cbRare, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	//Update time
	m_preTime = m_curTime;
}

void AssignmentScene1::OnResize(int width, int height)
{
	m_camera.SetAspect(width, height);
	m_camera.SetFrustum(60.0f, m_camera.GetAspect(), 0.5f, 1000.0f);
	m_camera.SetProjPerspective();

	//Update constant buffer(change on resize)
	Effect::cbResize.proj = m_camera.GetProj();
}