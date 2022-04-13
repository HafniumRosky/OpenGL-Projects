#include "BasicScene.h"

void BasicScene::Start()
{
	//Load shaders
	//Basic lighting shader
	m_shipVertexShader = Shader(GL_VERTEX_SHADER, "../Project5\\Assets\\Shaders\\ShipVS.glsl");
	m_shipFragShader = Shader(GL_FRAGMENT_SHADER, "../Project5\\Assets\\Shaders\\ShipFS.glsl");
	//Skybox shader
	m_skyboxVertexShader = Shader(GL_VERTEX_SHADER, "../Project5\\Assets\\Shaders\\SkyboxVS.glsl");
	m_skyboxFragShader = Shader(GL_FRAGMENT_SHADER, "../Project5\\Assets\\Shaders\\SkyboxFS.glsl");
	//Ocean shader
	m_oceanVertexShader = Shader(GL_VERTEX_SHADER, "../Project5\\Assets\\Shaders\\OceanVS.glsl");
	m_oceanHullShader = Shader(GL_TESS_CONTROL_SHADER, "../Project5\\Assets\\Shaders\\OceanHS.glsl");
	m_oceanDomainShader = Shader(GL_TESS_EVALUATION_SHADER, "../Project5\\Assets\\Shaders\\OceanDS.glsl");
	m_oceanFragShader = Shader(GL_FRAGMENT_SHADER, "../Project5\\Assets\\Shaders\\OceanFS.glsl");
	m_oceanComputeShader = Shader(GL_COMPUTE_SHADER, "../Project5\\Assets\\Shaders\\OceanCS.glsl");
	//General purpose shader
	m_depthInitiShader = Shader(GL_COMPUTE_SHADER, "../Project5\\Assets\\Shaders\\DepthInitCS.glsl");
	m_depthComputeShader = Shader(GL_COMPUTE_SHADER, "../Project5\\Assets\\Shaders\\DepthCS.glsl");
	//Smoke shader
	m_smokeUpdateVertexShader = Shader(GL_VERTEX_SHADER, "../Project5\\Assets\\Shaders\\SmokeUpdateVS.glsl");
	m_smokeUpdateGeometryShader = Shader(GL_GEOMETRY_SHADER, "../Project5\\Assets\\Shaders\\SmokeUpdateGS.glsl");
	m_smokeRenderVertexShader = Shader(GL_VERTEX_SHADER, "../Project5\\Assets\\Shaders\\SmokeRenderVS.glsl");
	m_smokeRenderGeometryShader = Shader(GL_GEOMETRY_SHADER, "../Project5\\Assets\\Shaders\\SmokeRenderGS.glsl");
	m_smokeRenderFragShader = Shader(GL_FRAGMENT_SHADER, "../Project5\\Assets\\Shaders\\SmokeRenderFS.glsl");
	//Cannon ball shader
	m_ballVertexShader = Shader(GL_VERTEX_SHADER, "../Project5\\Assets\\Shaders\\CannonBallVS.glsl");
	m_ballFragShader = Shader(GL_FRAGMENT_SHADER, "../Project5\\Assets\\Shaders\\CannonBallFS.glsl");

	//Load effects
	//Basic lighting effect
	m_shipEffect.AddShader(&m_shipVertexShader, &m_shipFragShader);
	m_shipEffect.BindShaders();
	m_shipEffect.SetTextureNum(1);
	//Skybox rendering effect
	m_skyboxEffect.AddShader(&m_skyboxVertexShader, &m_skyboxFragShader);
	m_skyboxEffect.BindShaders();
	m_skyboxEffect.SetTextureNum(1);
	//Ocean rendering effect
	m_oceanEffect.AddShader(&m_oceanVertexShader, &m_oceanHullShader, &m_oceanDomainShader, &m_oceanFragShader);
	m_oceanEffect.BindShaders();
	//Ocean wave effect
	m_oceanWaveEffect.AddShader(&m_oceanComputeShader);
	m_oceanWaveEffect.BindShaders();
	//Depth initializing effect
	m_depthInitiEffect.AddShader(&m_depthInitiShader);
	m_depthInitiEffect.BindShaders();
	//Ship floating effect
	m_shipFloatEffect.AddShader(&m_depthComputeShader);
	m_shipFloatEffect.BindShaders();
	//Smoke update effect
	m_smokeUpdateEffect.AddShader(&m_smokeUpdateVertexShader);
	m_smokeUpdateEffect.AddShader(&m_smokeUpdateGeometryShader);
	//Smoke render effect
	m_smokeRenderEffect.AddShader(&m_smokeRenderVertexShader);
	m_smokeRenderEffect.AddShader(&m_smokeRenderGeometryShader);
	m_smokeRenderEffect.AddShader(&m_smokeRenderFragShader);
	m_smokeRenderEffect.BindShaders();
	//Cannon ball rendering effect
	m_ballEffect.AddShader(&m_ballVertexShader, &m_ballFragShader);
	m_ballEffect.BindShaders();

	//ShadowMap
	m_shadowMap.GenerateDepthTexture(m_shadowMapWidth, m_shadowMapHeight, GL_DEPTH_COMPONENT32F, GL_READ_ONLY);

	//Load lights
	//Directional light
	DirectionalLight directLight;
	directLight.direction = vec3(1.0f, -1.0f, 1.0f);
	directLight.lightColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
	m_directLight.push_back(directLight);

	//Load Game objects
	//Player ship
	m_playerShip = Ship(VertexPosNormalTex, "ColonialShip", "Colonial Ship.FBX", true);
	m_playerShip.GetTransform().SetScale(vec3(0.1f, 0.1f, 0.1f));
	m_playerShip.GetTransform().SetPosition(vec3(0.0f, 1.0f, 0.0f));
	m_playerShip.LoadGameObject();
	for (int i = 0; i < m_playerShip.GetMeshCount(); i++)
		m_playerShip.SetMeshEffectWithIndex(&m_shipEffect, i);
	m_playerShip.SetShadowMap(m_shadowMap.GetTextureID());

	//Load Game objects
	//Player ship
	m_hostileShip = Ship(VertexPosNormalTex, "ColonialShip", "Colonial Ship.FBX", false);
	m_hostileShip.GetTransform().SetScale(vec3(0.1f, 0.1f, 0.1f));
	m_hostileShip.GetTransform().SetPosition(vec3(-20.0f, 1.0f, 0.0f));
	m_hostileShip.LoadGameObject();
	for (int i = 0; i < m_hostileShip.GetMeshCount(); i++)
		m_hostileShip.SetMeshEffectWithIndex(&m_shipEffect, i);
	m_hostileShip.SetShadowMap(m_shadowMap.GetTextureID());

	//Camera

	//Main Camera
	//Set frustum
	m_camera.SetFrustum(60.0f, 16.0f / 9.0f, 0.1f, 1000.0f);
	//Set transform
	//Set camera position
	//m_camera.GetTransform().SetPosition(vec3(8.0f, 8.0f, 8.0f));
	m_camera.GetTransform().SetPosition(m_playerShip.GetTransform().GetPosition() + vec3(0.0f, 1.8f, 0.0f) + 4.2f * m_playerShip.GetTransform().GetForwardAxis());
	m_camera.GetTransform().SetRotation(m_playerShip.GetTransform().GetRotation() + vec3(0.0f, 3.1415926f, 0.0f));
	//Set camera target
	//m_camera.SetTarget(m_playerShip.GetTransform().GetPosition() + vec3(0.0f, 3.0f, 0.0f));
	m_camera.SetTarget(m_camera.GetTransform().GetPosition() + m_camera.GetTransform().GetForwardAxis());
	//Compute distance to target
	m_camera.ComputeDistance();
	//Set view
	m_camera.LookAt(vec3(0.0f, 1.0f, 0.0f));
	//Set camera projection
	m_camera.SetProjPerspective();

	//DepthCamera
	//Set frustum
	m_depthCamera.SetFrustum(-1.5f, 1.5f, -5.0f, 5.0f, 0.0f, 100.0f);
	//Set transform
	//Set camera position
	m_depthCamera.GetTransform().SetPosition(vec3(m_playerShip.GetTransform().GetPosition().x, -20.0f, m_playerShip.GetTransform().GetPosition().z - 0.0001f));
	//Set camera target
	m_depthCamera.SetTarget(m_playerShip.GetTransform().GetPosition());
	//Compute distance to target
	//m_depthCamera.ComputeDistance();
	//Set view
	m_depthCamera.LookAt(vec3(0.0f, 1.0f, 0.0f));
	//Set camera projection
	m_depthCamera.SetProjOrtho();

	//Set the number of FBO
	m_depthCamera.SetFBONum(4);
	//Set camera render target
	m_depthCamera.GenerateFBO();
	//Set camera render texture
	//m_depthCamera.BindTextureToFBO();
	//m_depthCamera.BindTextureToFBO(m_playerShip.GetOceanDepth(), 0);
	//m_depthCamera.BindTextureToFBO(m_playerShip.GetShipDepth(), 1);
	m_depthCamera.BindDepthBufferWithTexToFBO(m_playerShip.GetOceanDepth(), 0);
	m_depthCamera.BindDepthBufferWithTexToFBO(m_playerShip.GetShipDepth(), 1);
	m_depthCamera.BindDepthBufferWithTexToFBO(m_hostileShip.GetOceanDepth(), 2);
	m_depthCamera.BindDepthBufferWithTexToFBO(m_hostileShip.GetShipDepth(), 3);

	//ShadowCamera
	//Set frustum
	m_shadowCamera.SetFrustum(-20.0f, 20.0f, -20.0f, 20.0f, 0.0f, 500.0f);
	//Set transform
	//Set camera position
	m_shadowCamera.GetTransform().SetPosition(m_playerShip.GetTransform().GetPosition() - normalize(directLight.direction) * 100.0f);
	//Set camera target
	m_shadowCamera.SetTarget(m_playerShip.GetTransform().GetPosition());
	//Set view
	m_shadowCamera.LookAt(vec3(0.0f, 1.0f, 0.0f));
	//Set camera projection
	m_shadowCamera.SetProjOrtho();
	//Set the number of FBO
	m_shadowCamera.SetFBONum(1);
	//Set camera render target
	m_shadowCamera.GenerateFBO();
	//Set camera render texture and buffer
	m_shadowCamera.BindDepthBufferWithTexToFBO(m_shadowMap, 0);


	//Skybox
	m_skybox = Skybox(VertexPos, "Sunny1", "png");
	m_skybox.GetTransform().SetScale(vec3(1000.0f, 1000.0f, 1000.0f));
	m_skybox.GetTransform().SetPosition(m_camera.GetTransform().GetPosition());
	m_skybox.LoadGameObject();
	m_skybox.SetMeshEffectWithIndex(&m_skyboxEffect, 0);

	//Ocean
	m_ocean = Plane(VertexPosNormalTangentTex, 1000, 1000);
	m_ocean.GetTransform().SetScale(vec3(2.0f, 2.0f, 2.0f));
	m_ocean.LoadGameObject(1024, 1024);
	m_ocean.SetMeshEffectWithIndex(&m_oceanEffect, 0);
	m_ocean.SetShadowMap(m_shadowMap.GetTextureID());
	m_ocean.SetEnvironmentMapID(m_skybox.GetMeshTextureID(0, 0));

	//Smoke
	m_smoke = ParticleSystem(0.1f, 3.0f, "smoke_thin.png", 100);
	m_smoke.GetTransform().SetPosition(vec3(5.0f, 1.0f, 5.0f));
	m_smoke.LoadGameObject();
	m_smoke.SetUpdateEffect(&m_smokeUpdateEffect);
	m_smoke.SetRenderEffect(&m_smokeRenderEffect);
	m_smoke.SetInitialVelocity(vec3(0.0f, 1.0f, 0.0f));

	m_smokeUpdateEffect.BindShadersWithTFV(m_smoke.GetVarings());

	/*//Cannon ball
	m_ball = Ball(VertexPosNormalColor, 0.05f, 20.0f, 20.0f, vec4(0.5f, 0.5f, 0.5f, 1.0f));
	m_ball.GetTransform().SetPosition(m_playerShip.GetTransform().GetPosition() + m_playerShip.GetTransform().GetRightAxis() * vec3(3.0f) 
		- m_playerShip.GetTransform().GetForwardAxis() * 3.0f);
	m_ball.LoadGameObject();
	m_ball.SetMeshEffectWithIndex(&m_ballEffect, 0);*/
}

void BasicScene::InitiScene()
{
	//Game object
	//Load buffers of meshes
	m_skybox.InputAssemble();
	m_playerShip.InputAssemble();
	m_hostileShip.InputAssemble();
	m_ocean.InputAssemble();
	//m_ball.InputAssemble();

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
	//m_oceanWaveEffect.BindEffect();
	//m_oceanWaveEffect.ComputeTexture(m_ocean.GetMeshTextureID(0, 0), 32, 32, 1);
}

void BasicScene::FetchData(RenderTexture rt, GLfloat* data)
{
	glBindTexture(GL_TEXTURE_2D, rt.GetTextureID());
	glGetTexImage(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, GL_FLOAT, data);
	/*for (int i = 0; i < 32; i++)
	{
		for (int j = 0; j < 32; j++)
		{
			//if(i * 32 + j == 512)
				std::cout << i * 32 + j << " " << data[i * 32 + j] << std::endl;
		}
	}*/
}

void BasicScene::FetchData(RenderTexture rt, GLint* data)
{
	glBindTexture(GL_TEXTURE_2D, rt.GetTextureID());
	glGetTexImage(GL_TEXTURE_2D, 0, GL_RED, GL_INT, data);
	for (int i = 0; i < 4; i++)
	{
		std::cout << i << " " << data[i]<< std::endl;
	}
}

void BasicScene::RenderDepth()
{
	//Move Depth camera
	m_depthCamera.GetTransform().SetPosition(vec3(m_playerShip.GetTransform().GetPosition().x, -20.0f, m_playerShip.GetTransform().GetPosition().z));
	m_depthCamera.GetTransform().SetPosition(m_depthCamera.GetTransform().GetPosition() - vec3(m_playerShip.GetTransform().GetForwardAxis().x * 0.00001,
		0.0f, m_playerShip.GetTransform().GetForwardAxis().z * 0.00001));
	m_depthCamera.SetTarget(m_playerShip.GetTransform().GetPosition());
	m_depthCamera.LookAt(vec3(0.0f, 1.0f, 0.0f));

	//Set camera proj view
	Effect::cbFrame.view = m_depthCamera.GetView();
	Effect::cbResize.proj = m_depthCamera.GetProj();
	glBindBuffer(GL_UNIFORM_BUFFER, Effect::m_UBOid[1]);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(CBChangesEveryFrame), &Effect::cbFrame, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	glBindBuffer(GL_UNIFORM_BUFFER, Effect::m_UBOid[2]);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(CBChangesOnResize), &Effect::cbResize, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	//Set frame buffer
	m_depthCamera.BindFBO(0);
	glViewport(0, 0, m_depthSize, m_depthSize);
	glDisable(GL_BLEND);
	//Enable depth test
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glDepthMask(GL_TRUE);
	//Set clear flags
	glClear(GL_DEPTH_BUFFER_BIT);
	//Set render states
	Effect::cbState.renderDepth = true;
	glBindBuffer(GL_UNIFORM_BUFFER, Effect::m_UBOid[4]);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(CBRenderState), &Effect::cbState, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	m_ocean.Draw();
	FetchData(m_playerShip.GetOceanDepth(), m_playerOceanDepth);
	//Set frame buffer
	m_depthCamera.BindFBO(1);
	//glViewport(0, 0, 32, 32);
	//m_camera.BindFBO();
	//clear the current buffer for rendering the next object
	glClear(GL_DEPTH_BUFFER_BIT);
	m_playerShip.Draw();
	FetchData(m_playerShip.GetShipDepth(), m_playerShipDepth);

	//Move depth camera
	m_depthCamera.GetTransform().SetPosition(vec3(m_hostileShip.GetTransform().GetPosition().x, -20.0f, m_hostileShip.GetTransform().GetPosition().z));
	m_depthCamera.GetTransform().SetPosition(m_depthCamera.GetTransform().GetPosition() - vec3(m_hostileShip.GetTransform().GetForwardAxis().x * 0.00001,
		0.0f, m_hostileShip.GetTransform().GetForwardAxis().z * 0.00001));
	m_depthCamera.SetTarget(m_hostileShip.GetTransform().GetPosition());
	m_depthCamera.LookAt(vec3(0.0f, 1.0f, 0.0f));

	//Set camera proj view
	Effect::cbFrame.view = m_depthCamera.GetView();
	glBindBuffer(GL_UNIFORM_BUFFER, Effect::m_UBOid[1]);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(CBChangesEveryFrame), &Effect::cbFrame, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	m_depthCamera.BindFBO(2);
	glClear(GL_DEPTH_BUFFER_BIT);
	m_ocean.Draw();
	FetchData(m_hostileShip.GetOceanDepth(), m_hostileOceanDepth);

	m_depthCamera.BindFBO(3);
	glClear(GL_DEPTH_BUFFER_BIT);
	m_hostileShip.Draw();
	FetchData(m_hostileShip.GetShipDepth(), m_hostileShipDepth);
	//Initialize depth subtract
	//m_depthInitiEffect.BindEffect();
	//m_depthInitiEffect.ComputeTexture(m_playerShip.GetDepth().GetTextureID(), m_playerShip.GetDepth().GetReadWrite(), m_playerShip.GetDepth().GetFormat(), 1, 1, 1);
	//glMemoryBarrier(GL_TEXTURE_FETCH_BARRIER_BIT);
	//FetchData(m_playerShip.GetDepth(), m_depth);

	//Compute depth subtract
	//m_shipFloatEffect.BindEffect();
	//m_shipFloatEffect.ComputeTexture(m_playerShip.GetDepthID(), m_playerShip.GetDepthReadWrite(), m_playerShip.GetDepthFormat(), 1, 1, 1);
	//glMemoryBarrier(GL_TEXTURE_FETCH_BARRIER_BIT);
	//FetchData(m_playerShip.GetDepth(), m_playerShipDepth);

}

void BasicScene::RenderShadows()
{
	//Set frame buffer
	m_shadowCamera.BindFBO(0);
	glViewport(0, 0, m_shadowMapWidth, m_shadowMapHeight);
	glClear(GL_DEPTH_BUFFER_BIT);
	//Set camera proj view
	Effect::cbFrame.view = m_shadowCamera.GetView();
	Effect::cbResize.proj = m_shadowCamera.GetProj();
	glBindBuffer(GL_UNIFORM_BUFFER, Effect::m_UBOid[1]);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(CBChangesEveryFrame), &Effect::cbFrame, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	glBindBuffer(GL_UNIFORM_BUFFER, Effect::m_UBOid[2]);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(CBChangesOnResize), &Effect::cbResize, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glDepthMask(GL_TRUE);
	//Set render states
	Effect::cbState.renderDepth = true;
	glBindBuffer(GL_UNIFORM_BUFFER, Effect::m_UBOid[4]);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(CBRenderState), &Effect::cbState, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glEnable(GL_POLYGON_OFFSET_FILL);
	float offsetFactor = 2.0f;
	float offsetunit = 10000.0f;
	glPolygonOffset(offsetFactor, offsetunit);
	m_playerShip.Draw();
	m_hostileShip.Draw();
	//m_ball.Draw();
	for (int i = 0; i < m_rounds.size(); i++)
		m_rounds[i].Draw();
	glPolygonOffset(0.0f, 0.0f);
	glDisable(GL_POLYGON_OFFSET_FILL);
}

void BasicScene::RenderObjects()
{
	//Set frame buffer
	m_camera.BindFBO();
	glViewport(0, 0, m_sceneWidth, m_sceneHeight);
	//Set camera proj view
	Effect::cbFrame.view = m_camera.GetView();
	Effect::cbFrame.shadowMatrix = m_shadowCamera.GetProj() * m_shadowCamera.GetView();
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

	//Draw objects
	//Ship
	//Enable depth write
	glDepthMask(GL_TRUE);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	m_playerShip.Draw();
	m_hostileShip.Draw();
	//Cannon balls
	glPolygonMode(GL_FRONT, GL_FILL);
	//m_ball.Draw();
	for (int i = 0; i < m_rounds.size(); i++)
		m_rounds[i].Draw();
	//Ocean
	//Enable blending
	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glPolygonMode(GL_FRONT, GL_FILL);
	m_ocean.Draw();
	//Skybox
	//Disable blending
	glDisable(GL_BLEND);
	//Disable depth write
	glDepthMask(GL_FALSE);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	m_skybox.Draw(0);
	//Smoke
	//Enable blending
	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glPolygonMode(GL_FRONT, GL_FILL);
	//m_smoke.Draw();
}

void BasicScene::DisplayScene()
{
	RenderDepth();
	RenderShadows();
	RenderObjects();
}

void BasicScene::UpdateScene()
{
	m_curTime = glutGet(GLUT_ELAPSED_TIME);
	float deltaTime = m_curTime - m_preTime;
	//Update player ship
	vec3 camSyn;
	camSyn = m_playerShip.Update(m_depthSize, m_depthCamera, deltaTime * 0.001f, vec3(0, 0, 0), m_playerShipDepth, m_playerOceanDepth);
	m_hostileShip.Update(m_depthSize, m_depthCamera, deltaTime * 0.001f, vec3(0, 0, 0), m_hostileShipDepth, m_hostileOceanDepth);

	//Update smoke
	m_smoke.Update(deltaTime * 0.001f);

	//Cannon ball
	for (std::vector<Ball>::iterator iter = m_rounds.begin(); iter != m_rounds.end();)
	{
		iter->Update(deltaTime * 0.001f);
		float forwardProj = dot(vec2(iter->GetTransform().GetPosition().x - m_hostileShip.GetTransform().GetPosition().x, 
			iter->GetTransform().GetPosition().z - m_hostileShip.GetTransform().GetPosition().z),
			vec2(m_hostileShip.GetTransform().GetForwardAxis().x, m_hostileShip.GetTransform().GetForwardAxis().z));
		float horProj = dot(vec2(iter->GetTransform().GetPosition().x - m_hostileShip.GetTransform().GetPosition().x, 
			iter->GetTransform().GetPosition().z - m_hostileShip.GetTransform().GetPosition().z),
			vec2(m_hostileShip.GetTransform().GetRightAxis().x, m_hostileShip.GetTransform().GetRightAxis().z));
		bool hit1 = abs(forwardProj) <= 4.175f;
		bool hit2 = abs(horProj) <= 1.7f;
		if (distance(iter->GetTransform().GetPosition(), m_playerShip.GetTransform().GetPosition()) > 500.0f || (hit1 && hit2))
		{
			if (hit1 && hit2)
				m_hostileShip.Hit();
			iter = m_rounds.erase(iter);
		}
		else
			iter++;
	}
	vec3 viewDir = m_camera.GetTarget() - m_camera.GetTransform().GetPosition();
	std::vector<Ball> m_newRounds;
	m_playerShip.Fire(normalize(viewDir), m_newRounds);
	for (int i = 0; i < m_newRounds.size(); i++)
	{
		m_newRounds[i].LoadGameObject(vec4(1.0f), 8.0f);
		m_newRounds[i].SetMeshEffectWithIndex(&m_ballEffect, 0);
		m_newRounds[i].InputAssemble();
		m_rounds.push_back(m_newRounds[i]);
	}
	//Update cameras
	m_camera.Update(m_playerShip.GetTransform(), camSyn, deltaTime * 0.001f);


	//Set camera position
	m_shadowCamera.GetTransform().SetPosition(m_playerShip.GetTransform().GetPosition() - normalize(m_directLight[0].direction) * 100.0f);
	//Set camera target
	m_shadowCamera.SetTarget(m_playerShip.GetTransform().GetPosition());
	m_shadowCamera.LookAt(vec3(0.0f, 1.0f, 0.0f));

	//Update skybox position with camera
	m_skybox.GetTransform().SetPosition(m_camera.GetTransform().GetPosition());
	//Update time
	m_preTime = m_curTime;
	//Update ocean wave time
	m_ocean.Update(m_curTime);


	//Update constant buffer(change every fram)
	Effect::cbFrame.view = m_camera.GetView();
	Effect::cbFrame.eyePos = m_camera.GetTransform().GetPosition();
}

void BasicScene::OnResize(int width, int height)
{
	m_camera.SetAspect(width, height);
	m_camera.SetFrustum(60.0f, m_camera.GetAspect(), 0.5f, 1000.0f);
	m_camera.SetProjPerspective();

	//Update constant buffer(change on resize)
	Effect::cbResize.proj = m_camera.GetProj();
}