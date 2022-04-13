#include "GIScene.h"

void GIScene::Start()
{
	//Load shaders
	//Basic PBR shader
	m_basicPBRVertexShader = Shader(GL_VERTEX_SHADER, "../Project5\\Assets\\Shaders\\BasicPBRVS.glsl");
	m_basicPBRFragShader = Shader(GL_FRAGMENT_SHADER, "../Project5\\Assets\\Shaders\\BasicPBRFS.glsl");
	//Basic PBR shader with probes
	m_basicPBRProbeVertexShader = Shader(GL_VERTEX_SHADER, "../Project5\\Assets\\Shaders\\IBLVS.glsl");
	m_basicPBRProbeFragShader = Shader(GL_FRAGMENT_SHADER, "../Project5\\Assets\\Shaders\\IBLFS.glsl");
	//Diffuse irradiance shader
	m_diffuseIrradianceVertexShader = Shader(GL_VERTEX_SHADER, "../Project5\\Assets\\Shaders\\BakeGIDiffVS.glsl");
	m_diffuseIrradianceFragShader = Shader(GL_FRAGMENT_SHADER, "../Project5\\Assets\\Shaders\\BakeGIDiffFS.glsl");
	//Prefilter map shader
	m_prefilterMapVertexShader = Shader(GL_VERTEX_SHADER, "../Project5\\Assets\\Shaders\\PrefilterMapVS.glsl");
	m_prefilterMapFragShader = Shader(GL_FRAGMENT_SHADER, "../Project5\\Assets\\Shaders\\PrefilterMapFS.glsl");
	//Integrate BRDF shader
	m_IntegrateBRDFVertexShader = Shader(GL_VERTEX_SHADER, "../Project5\\Assets\\Shaders\\IntegrateBRDFVS.glsl");
	m_IntegrateBRDFFragShader = Shader(GL_FRAGMENT_SHADER, "../Project5\\Assets\\Shaders\\IntegrateBRDFFS.glsl");
	//HDR sampler shader
	m_HDRSamplerVertexShader = Shader(GL_VERTEX_SHADER, "../Project5\\Assets\\Shaders\\HDRSamplerVS.glsl");
	m_HDRSamplerFragShader = Shader(GL_FRAGMENT_SHADER, "../Project5\\Assets\\Shaders\\HDRSamplerFS.glsl");
	//HDR environment map shader
	m_HDREnvMapVertexShader = Shader(GL_VERTEX_SHADER, "../Project5\\Assets\\Shaders\\HDREnvMapVS.glsl");
	m_HDREnvMapFragShader = Shader(GL_FRAGMENT_SHADER, "../Project5\\Assets\\Shaders\\HDREnvMapFS.glsl");
	//Dual-paraboloid generator shader
	m_DualParaGenVertexShader = Shader(GL_VERTEX_SHADER, "../Project5\\Assets\\Shaders\\DualParaGenVS.glsl");
	m_DualParaGenFragShader = Shader(GL_FRAGMENT_SHADER, "../Project5\\Assets\\Shaders\\DualParaGenFS.glsl");

	//Load effects
	//Basic PBR effect
	m_basicPBREffect.AddShader(&m_basicPBRVertexShader, &m_basicPBRFragShader);
	m_basicPBREffect.BindShaders();
	//Basic PBR effect with probes
	m_basicPBRProbeEffect.AddShader(&m_basicPBRProbeVertexShader, &m_basicPBRProbeFragShader);
	m_basicPBRProbeEffect.BindShaders();
	//Diffuse irradiance effect
	m_diffuseIrradianceEffect.AddShader(&m_diffuseIrradianceVertexShader, &m_diffuseIrradianceFragShader);
	m_diffuseIrradianceEffect.BindShaders();
	//Prefilter map shader
	m_prefilterMapEffect.AddShader(&m_prefilterMapVertexShader, &m_prefilterMapFragShader);
	m_prefilterMapEffect.BindShaders();
	//Integrate BRDF Effect
	m_IntegrateBRDFEffect.AddShader(&m_IntegrateBRDFVertexShader, &m_IntegrateBRDFFragShader);
	m_IntegrateBRDFEffect.BindShaders();
	//HDR sampler effect
	m_HDRSamplerEffect.AddShader(&m_HDRSamplerVertexShader, &m_HDRSamplerFragShader);
	m_HDRSamplerEffect.BindShaders();
	//HDR environment map effect
	m_HDREnvMapEffect.AddShader(&m_HDREnvMapVertexShader, &m_HDREnvMapFragShader);
	m_HDREnvMapEffect.BindShaders();
	//Dual-paraboloid generator effect
	m_DualParaGenEffect.AddShader(&m_DualParaGenVertexShader, &m_DualParaGenFragShader);
	m_DualParaGenEffect.BindShaders();

	//Load lights
	PointLight pointLight;
	pointLight.lightColor = vec4(23.47f, 21.31f, 20.79f, 1.0f) / 2.0f;
	pointLight.position = vec3(0.0f, 4.0f, 4.0f);
	pointLight.att = vec3(0.0f, 0.0f, 0.1f);
	m_pointLight.push_back(pointLight);
	//pointLight.position = vec3(5.0, -5.0, 5.0);
	//m_pointLight.push_back(pointLight);
	//pointLight.position = vec3(-5.0, 5.0, 5.0);
	//m_pointLight.push_back(pointLight);
	//pointLight.position = vec3(-5.0, -5.0, 5.0);
	//m_pointLight.push_back(pointLight);

	//Load GameObjects
	//PBR material sphere
	//m_sphere = Ball(VertexPosNormalTex, 1.0f, 50.0f, 50.0f, vec4(1.0f), "PBRRustedIron");
	//m_sphere.GetTransform().SetPosition(vec3(0.0f, 0.0f, 0.0f));
	//m_sphere.LoadPBRGameObject(vec3(1.0f), 0.2f, 0.1f);
	//m_sphere.SetMeshEffectWithIndex(&m_basicPBREffect, 0);

	for (int i = 0; i < 6; i++)
	{
		for (int j = 0; j < 6; j++)
		{
			m_sphere[i * 6 + j] = Ball(VertexPosNormalTex, 0.5f, 50.0f, 50.0f, vec4(1.0f), "PBRRustedIron");
			m_sphere[i * 6 + j].GetTransform().SetPosition(vec3(-3.5f + j * 1.5f, 4.0f - i * 1.5f, 0.0f));
			m_sphere[i * 6 + j].LoadPBRGameObject(vec3(1.0f), j * 0.2f, i * 0.2f);
			m_sphere[i * 6 + j].SetMeshEffectWithIndex(&m_basicPBREffect, 0);
		}
	}


	//Camera
	//Main Camera
	//Set frustum
	m_camera.SetFrustum(60.0f, 16.0f / 9.0f, 0.1f, 1000.0f);
	//Set transform
	//Set camera position
	//m_camera.GetTransform().SetPosition(vec3(8.0f, 8.0f, 8.0f));
	m_camera.GetTransform().SetPosition(0.0f, 0.0f, -5.0f);
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
	m_samplerCam.BindFBOWithRenderBuffer(m_HDRCubeWidth, m_HDRCubeHeight, 0);

	//HDR environment
	m_HDREnvir = LightProbe(VertexPos, "Global", "GIScene", true, "Tropical_Beach_3k.hdr");
	m_HDREnvir.GetTransform().SetPosition(m_samplerCam.GetTransform().GetPosition());
	m_HDREnvir.LoadHDRSampler(m_HDRCubeWidth, m_HDRCubeHeight);
	m_HDREnvir.LoadLightProbe(m_HDRCubeWidth, m_HDRCubeHeight, m_prefilterResolution, m_BRDFLUTResolution);

	//Light probes
	for (int i = 0; i < 2; i++)
	{
		std::stringstream strStream;
		strStream << i;
		std::string numString;
		strStream >> numString;
		m_LightProbe[i] = LightProbe(VertexPos, "Local" + numString, "GIScene", false, BOXBASED);
		m_LightProbe[i].SetVolume(50.0f, 50.0f, 50.0f);
		m_LightProbe[i].LoadLocalLightProbe(m_HDRCubeWidth, m_HDRCubeWidth, m_prefilterResolution, m_BRDFLUTResolution);
	}
	m_LightProbe[0].GetTransform().SetPosition(vec3(0.0f, 0.0f, -5.0f));
	m_LightProbe[1].GetTransform().SetPosition(vec3(0.0f, 0.0f, -10.0f));

	//Walls
	m_Wall[0] = Plane(VertexPosNormalTex, 10, 10, "PBRWhiteTile");
	m_Wall[0].LoadPBRGameObject(vec3(0.65f, 0.05f, 0.05f), 0.7f, 0.1f);
	m_Wall[0].SetMeshEffectWithIndex(&m_basicPBREffect, 0);

	m_Wall[1] = Plane(VertexPosNormalTex, 10, 10, "PBRWhiteTile");
	m_Wall[1].LoadPBRGameObject(vec3(0.12f, 0.45f, 0.15f), 0.7f, 0.1f);
	m_Wall[1].SetMeshEffectWithIndex(&m_basicPBREffect, 0);
	for (int i = 2; i < 5; i++)
	{
		m_Wall[i] = Plane(VertexPosNormalTex, 10, 10, "PBRWhiteTile");
		m_Wall[i].LoadPBRGameObject(vec3(0.73f), 0.7f, 0.1f);
		m_Wall[i].SetMeshEffectWithIndex(&m_basicPBREffect, 0);
	}
	for (int i = 5; i < 6; i++)
	{
		m_Wall[i] = Plane(VertexPosNormalTex, 10, 10, "PBRWhiteTile");
		m_Wall[i].LoadPBRGameObject(vec3(0.0f), 0.7f, 0.1f, -1.0f, vec4(23.47f, 21.31f, 20.79f, 1.0f));
		m_Wall[i].SetMeshEffectWithIndex(&m_basicPBREffect, 0);
	}

	m_Wall[0].GetTransform().SetPosition(vec3(0.0f, 0.0f, -15.0f));
	m_Wall[0].GetTransform().SetRotation(vec3(radians(90.0f), 0.0f, 0.0f));
	m_Wall[1].GetTransform().SetPosition(vec3(0.0f, 0.0f, 5.0f));
	m_Wall[1].GetTransform().SetRotation(vec3(radians(-90.0f), 0.0f, 0.0f));
	m_Wall[2].GetTransform().SetPosition(vec3(5.0f, 0.0f, 0.0f));
	m_Wall[2].GetTransform().SetRotation(vec3(0.0f, 0.0f, radians(90.0f)));
	m_Wall[3].GetTransform().SetPosition(vec3(5.0f, 0.0f, -10.0f));
	m_Wall[3].GetTransform().SetRotation(vec3(0.0f, 0.0f, radians(90.0f)));
	m_Wall[4].GetTransform().SetPosition(vec3(-5.0f, 0.0f, 0.0f));
	m_Wall[4].GetTransform().SetRotation(vec3(0.0f, 0.0f, radians(-90.0f)));
	m_Wall[5].GetTransform().SetPosition(vec3(-5.0f, 0.0f, -10.0f));
	m_Wall[5].GetTransform().SetRotation(vec3(0.0f, 0.0f, radians(-90.0f)));

	//Floors
	for (int i = 0; i < 2; i++)
	{
		m_floor[i] = Plane(VertexPosNormalTex, 10, 10, "PBRWhiteTile");
		m_floor[i].LoadPBRGameObject(vec3(0.73f), 0.7f, 0.1f);
		m_floor[i].SetMeshEffectWithIndex(&m_basicPBREffect, 0);
	}
	m_floor[0].GetTransform().SetPosition(vec3(0.0f, -5.0f, 0.0f));
	m_floor[1].GetTransform().SetPosition(vec3(0.0f, -5.0f, -10.0f));

	//Ceilings
	for (int i = 0; i < 2; i++)
	{
		m_ceil[i] = Plane(VertexPosNormalTex, 10, 10, "PBRWhiteTile");
		m_ceil[i].LoadPBRGameObject(vec3(0.73f), 0.7f, 0.1f);
		m_ceil[i].SetMeshEffectWithIndex(&m_basicPBREffect, 0);
	}
	m_ceil[0].GetTransform().SetPosition(vec3(0.0f, 5.0f, 0.0f));
	m_ceil[0].GetTransform().SetRotation(vec3(0.0f, 0.0f, radians(180.0f)));
	m_ceil[1].GetTransform().SetPosition(vec3(0.0f, 5.0f, -10.0f));
	m_ceil[1].GetTransform().SetRotation(vec3(0.0f, 0.0f, radians(180.0f)));

	//AreaLight
	m_areaLight = Plane(VertexPosNormalTex, 2, 2, "PBRWhiteTile");
	m_areaLight.LoadPBRGameObject(vec3(0.0f), 0.1f, 0.7f, -1.0f, vec4(23.47f, 0.0f, 0.0f, 1.0f));
	m_areaLight.SetMeshEffectWithIndex(&m_basicPBREffect, 0);
	m_areaLight.GetTransform().SetPosition(vec3(4.999f, 0.0f, 0.0f));
	m_areaLight.GetTransform().SetRotation(vec3(0.0f, 0.0f, radians(90.0f)));
}

void GIScene::SampleHDR()
{
	m_HDREnvir.SetMeshEffectWithIndex(&m_HDRSamplerEffect, 0);
	m_HDREnvir.InputAssemble(0);
	//Sampler the HDR map for 6 times for each face
	m_samplerCam.BindFBO(0);
	glViewport(0, 0, m_HDRCubeWidth, m_HDRCubeHeight);
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
	//Generate mip maps
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_HDREnvir.GetMeshTextureID(0, 1));
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
}

void GIScene::TransformCubeToDualPara()
{
	m_HDREnvir.SetMeshEffectWithIndex(&m_DualParaGenEffect, 1);
	m_HDREnvir.InputAssemble(1);
	
	m_samplerCam.BindFBO(0);
	glViewport(0, 0, m_HDRCubeWidth * 4, m_HDRCubeHeight * 4);
	glDepthMask(GL_TRUE);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	Effect::cbResize.proj = m_samplerCam.GetProj();
	glBindBuffer(GL_UNIFORM_BUFFER, Effect::m_UBOid[2]);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(CBChangesOnResize), &Effect::cbResize, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	m_samplerCam.SetTarget(vec3(0.0f, 0.0f, 1.0f));
	m_samplerCam.LookAt(vec3(0.0f, -1.0f, 0.0f));
	Effect::cbFrame.view = m_samplerCam.GetView();
	glBindBuffer(GL_UNIFORM_BUFFER, Effect::m_UBOid[1]);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(CBChangesEveryFrame), &Effect::cbFrame, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	//Bind textures to the frame buffer
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_HDREnvir.GetMeshTextureID(1, 0), 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	m_HDREnvir.SampleCube(1.0f);

	GLfloat* data = new GLfloat[m_HDRCubeWidth * 4 * m_HDRCubeWidth * 4 * 3];
	glBindTexture(GL_TEXTURE_2D, m_HDREnvir.GetMeshTextureID(1, 0));
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);
	glGetTexImage(GL_TEXTURE_2D, 0, GL_RGB, GL_FLOAT, data);
	std::string savePath = "../Project5\\Assets\\Precomputation\\" + m_sceneName + "Probe" + m_HDREnvir.getProbeName() + "\\DualParaboloidFront.hdr";
	stbi_write_hdr(savePath.c_str(), m_HDRCubeWidth * 4, m_HDRCubeHeight * 4, 3, data);
	//Bind the front texture to the probe
	m_HDREnvir.SetDualParaMap(0, m_HDREnvir.GetMeshTexture(1, 0));

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_HDREnvir.GetMeshTextureID(1, 1), 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	m_HDREnvir.SampleCube(-1.0f);

	glBindTexture(GL_TEXTURE_2D, m_HDREnvir.GetMeshTextureID(1, 1));
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);
	glGetTexImage(GL_TEXTURE_2D, 0, GL_RGB, GL_FLOAT, data);
	savePath = "../Project5\\Assets\\Precomputation\\" + m_sceneName + "Probe" + m_HDREnvir.getProbeName() + "\\DualParaboloidBack.hdr";
	stbi_write_hdr(savePath.c_str(), m_HDRCubeWidth * 4, m_HDRCubeHeight * 4, 3, data);
	//Bind back texture to the probe
	m_HDREnvir.SetDualParaMap(0, m_HDREnvir.GetMeshTexture(1, 1));
}

void GIScene::SampleSurrounding()
{
	//Bind Cam
	m_samplerCam.BindFBO(0);
	glViewport(0, 0, m_HDRCubeWidth, m_HDRCubeHeight);

	glDepthMask(GL_TRUE);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	//Reset view frustum
	m_samplerCam.SetFrustum(90.0f, 1, 0.001f, 28.0f);
	m_samplerCam.SetProjPerspective();
	Effect::cbResize.proj = m_samplerCam.GetProj();
	glBindBuffer(GL_UNIFORM_BUFFER, Effect::m_UBOid[2]);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(CBChangesOnResize), &Effect::cbResize, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

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

	for (int j = 0; j < 2; j++)
	{
		//Set camera position
		m_samplerCam.GetTransform().SetPosition(m_LightProbe[j].GetTransform().GetPosition());
		for (int i = 0; i < 6; i++)
		{
			m_samplerCam.SetTarget(m_samplerCam.GetTransform().GetPosition() + targetVec[i]);
			m_samplerCam.LookAt(upVec[i]);
			Effect::cbFrame.view = m_samplerCam.GetView();
			glBindBuffer(GL_UNIFORM_BUFFER, Effect::m_UBOid[1]);
			glBufferData(GL_UNIFORM_BUFFER, sizeof(CBChangesEveryFrame), &Effect::cbFrame, GL_DYNAMIC_DRAW);
			glBindBuffer(GL_UNIFORM_BUFFER, 0);

			//Bind frame buffer
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
				GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, m_LightProbe[j].GetMeshTextureID(0, 0), 0);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			//Sample surrounding
			//Assemble input
			for (int i = 0; i < 6; i++)
			{
				m_Wall[i].InputAssemble();
			}

			for (int i = 0; i < 2; i++)
			{
				m_floor[i].InputAssemble();
			}

			for (int i = 0; i < 2; i++)
			{
				m_ceil[i].InputAssemble();
			}

			m_areaLight.InputAssemble();

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
			//for(int i = 0; i < 36; i++)
				//m_sphere[i].DrawPBR();
			for (int i = 0; i < 6; i++)
				m_Wall[i].DrawPBR();
			for (int i = 0; i < 2; i++)
				m_floor[i].DrawPBR();
			for (int i = 0; i < 2; i++)
				m_ceil[i].DrawPBR();
			m_areaLight.DrawPBR();
		}
	}
}

void GIScene::BakeIrradianceMap()
{
	m_HDREnvir.SetMeshEffectWithIndex(&m_diffuseIrradianceEffect, 0);
	m_HDREnvir.InputAssemble(0);

	for (int i = 0; i < 2; i++)
	{
		m_LightProbe[i].SetMeshEffectWithIndex(&m_diffuseIrradianceEffect, 0);
		m_LightProbe[i].InputAssemble(0);
	}

	m_samplerCam.BindFBO(0);
	glViewport(0, 0, m_HDRCubeWidth, m_HDRCubeHeight);

	glDepthMask(GL_TRUE);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	//Reset camera projection and location
	m_samplerCam.SetFrustum(90.0f, 1.0f, 0.1f, 10.0f);
	m_samplerCam.GetTransform().SetPosition(0.0f, 0.0f, 0.0f);
	m_samplerCam.SetProjPerspective();
	Effect::cbResize.proj = m_samplerCam.GetProj();
	glBindBuffer(GL_UNIFORM_BUFFER, Effect::m_UBOid[2]);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(CBChangesOnResize), &Effect::cbResize, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

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

	for (int i = 0; i < 6; i++)
	{
		m_samplerCam.SetTarget(targetVec[i]);
		m_samplerCam.LookAt(upVec[i]);
		Effect::cbFrame.view = m_samplerCam.GetView();
		glBindBuffer(GL_UNIFORM_BUFFER, Effect::m_UBOid[1]);
		glBufferData(GL_UNIFORM_BUFFER, sizeof(CBChangesEveryFrame), &Effect::cbFrame, GL_DYNAMIC_DRAW);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);

		//Bind frame buffer
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
			GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, m_HDREnvir.GetMeshTextureID(0, 2), 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		m_HDREnvir.GenerateIrradianceMap(250, 250, m_HDREnvir.GetMeshTextureID(0, 1));

		GLfloat* data = new GLfloat[m_HDRCubeWidth * m_HDRCubeWidth * 3];
		//Save to local file
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_HDREnvir.GetMeshTextureID(0, 2));
		glGetTexImage(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, GL_FLOAT, data);
		std::stringstream strStream;
		strStream << i;
		std::string numString;
		strStream >> numString;
		std::string savePath = "../Project5\\Assets\\Precomputation\\" + m_sceneName + "Probe" + m_HDREnvir.getProbeName() + "\\IrradianceMap" + numString + ".hdr";
		stbi_write_hdr(savePath.c_str(), m_HDRCubeWidth, m_HDRCubeHeight, 3, data);
	}

	//Bind irradiance map to the probe
	m_HDREnvir.SetIrradianceMap(m_HDREnvir.GetMeshTexture(0, 2));

	//Light probes
	for (int j = 0; j < 2; j++)
	{
		for (int i = 0; i < 6; i++)
		{
			m_samplerCam.SetTarget(targetVec[i]);
			m_samplerCam.LookAt(upVec[i]);
			Effect::cbFrame.view = m_samplerCam.GetView();
			glBindBuffer(GL_UNIFORM_BUFFER, Effect::m_UBOid[1]);
			glBufferData(GL_UNIFORM_BUFFER, sizeof(CBChangesEveryFrame), &Effect::cbFrame, GL_DYNAMIC_DRAW);
			glBindBuffer(GL_UNIFORM_BUFFER, 0);

			//Bind frame buffer
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
				GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, m_LightProbe[j].GetMeshTextureID(0, 1), 0);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			m_LightProbe[j].GenerateIrradianceMap(250, 250, m_LightProbe[j].GetMeshTextureID(0, 0));

			GLfloat* data = new GLfloat[m_HDRCubeWidth * m_HDRCubeWidth * 3];
			//Save to local file
			glBindTexture(GL_TEXTURE_CUBE_MAP, m_LightProbe[j].GetMeshTextureID(0, 1));
			glGetTexImage(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, GL_FLOAT, data);
			std::stringstream strStream;
			strStream << i;
			std::string numString;
			strStream >> numString;
			std::string savePath = "../Project5\\Assets\\Precomputation\\" + m_sceneName + "Probe" + m_LightProbe[j].getProbeName() + "\\IrradianceMap" + numString + ".hdr";
			stbi_write_hdr(savePath.c_str(), m_HDRCubeWidth, m_HDRCubeHeight, 3, data);
		}
	}


	//For test
	//m_sphere.PushIrradianceMap(m_HDREnvir.GetMeshTexture(0, 2));
	for (int i = 0; i < 36; i++)
	{
		m_sphere[i].PushIrradianceMap(m_HDREnvir.GetMeshTexture(0, 2));
	}

	for (int i = 0; i < 6; i++)
	{
		m_Wall[i].PushIrradianceMap(m_HDREnvir.GetMeshTexture(0, 2));
		for (int j = 0; j < 2; j++)
		{
			m_Wall[i].PushIrradianceMap(m_LightProbe[j].GetMeshTexture(0, 1));
		}
	}

	for (int i = 0; i < 2; i++)
	{
		m_floor[i].PushIrradianceMap(m_HDREnvir.GetMeshTexture(0, 2));
		for (int j = 0; j < 2; j++)
		{
			m_floor[i].PushIrradianceMap(m_LightProbe[j].GetMeshTexture(0, 1));
		}
	}

	for (int i = 0; i < 2; i++)
	{
		m_ceil[i].PushIrradianceMap(m_HDREnvir.GetMeshTexture(0, 2));
		for (int j = 0; j < 2; j++)
		{
			m_ceil[i].PushIrradianceMap(m_LightProbe[j].GetMeshTexture(0, 1));
		}
	}

	m_areaLight.PushIrradianceMap(m_HDREnvir.GetMeshTexture(0, 2));
	for (int j = 0; j < 2; j++)
	{
		m_areaLight.PushIrradianceMap(m_LightProbe[j].GetMeshTexture(0, 1));
	}
}

void GIScene::BakePrefilteredMap()
{
	m_HDREnvir.SetMeshEffectWithIndex(&m_prefilterMapEffect, 0);
	m_HDREnvir.InputAssemble(0);

	for (int i = 0; i < 2; i++)
	{
		m_LightProbe[i].SetMeshEffectWithIndex(&m_prefilterMapEffect, 0);
		m_LightProbe[i].InputAssemble(0);
	}

	m_samplerCam.BindFBO(0);

	//Swap render states
	glDepthMask(GL_TRUE);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	//list 6 faces properties for the camera
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

	//filter all mip maps
	for (unsigned int i = 0; i < m_maxMipLevel; i++)
	{
		//Resize render buffer
		unsigned int mipResolution = m_prefilterResolution * pow(0.5, i);
		glBindRenderbuffer(GL_RENDERBUFFER, m_samplerCam.GetRBO(0));
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, mipResolution, mipResolution);
		//Resize view port
		glViewport(0, 0, mipResolution, mipResolution);
		float roughness = i / (float)(m_maxMipLevel - 1);

		//prefilter 6 different faces
		for (unsigned int j = 0; j < 6; j++)
		{
			//Update view
			m_samplerCam.SetTarget(targetVec[j]);
			m_samplerCam.LookAt(upVec[j]);
			Effect::cbFrame.view = m_samplerCam.GetView();
			glBindBuffer(GL_UNIFORM_BUFFER, Effect::m_UBOid[1]);
			glBufferData(GL_UNIFORM_BUFFER, sizeof(CBChangesEveryFrame), &Effect::cbFrame, GL_DYNAMIC_DRAW);
			glBindBuffer(GL_UNIFORM_BUFFER, 0);
			//Bind mip map to the render target
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
				GL_TEXTURE_CUBE_MAP_POSITIVE_X + j, m_HDREnvir.GetMeshTextureID(0, 3), i);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			//Prefilter
			m_HDREnvir.Prefilter(roughness, m_HDREnvir.GetMeshTextureID(0, 1));
		}
	}

	//LightProbes
	for (int k = 0; k < 2; k++)
	{
		//filter all mip maps
		for (unsigned int i = 0; i < m_maxMipLevel; i++)
		{
			//Resize render buffer
			unsigned int mipResolution = m_prefilterResolution * pow(0.5, i);
			glBindRenderbuffer(GL_RENDERBUFFER, m_samplerCam.GetRBO(0));
			glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, mipResolution, mipResolution);
			//Resize view port
			glViewport(0, 0, mipResolution, mipResolution);
			float roughness = i / (float)(m_maxMipLevel - 1);

			//prefilter 6 different faces
			for (unsigned int j = 0; j < 6; j++)
			{
				//Update view
				m_samplerCam.SetTarget(targetVec[j]);
				m_samplerCam.LookAt(upVec[j]);
				Effect::cbFrame.view = m_samplerCam.GetView();
				glBindBuffer(GL_UNIFORM_BUFFER, Effect::m_UBOid[1]);
				glBufferData(GL_UNIFORM_BUFFER, sizeof(CBChangesEveryFrame), &Effect::cbFrame, GL_DYNAMIC_DRAW);
				glBindBuffer(GL_UNIFORM_BUFFER, 0);
				//Bind mip map to the render target
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
					GL_TEXTURE_CUBE_MAP_POSITIVE_X + j, m_LightProbe[k].GetMeshTextureID(0, 2), i);
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
				//Prefilter
				m_LightProbe[k].Prefilter(roughness, m_LightProbe[k].GetMeshTextureID(0, 0));
			}
		}
	}


	//m_sphere.PushPrefilteredMap(m_HDREnvir.GetMeshTexture(0, 3));
	for (int i = 0; i < 36; i++)
	{
		m_sphere[i].PushPrefilteredMap(m_HDREnvir.GetMeshTexture(0, 3));
	}

	for (int i = 0; i < 6; i++)
	{
		m_Wall[i].PushPrefilteredMap(m_HDREnvir.GetMeshTexture(0, 3));
		for (int j = 0; j < 2; j++)
		{
			m_Wall[i].PushPrefilteredMap(m_LightProbe[j].GetMeshTexture(0, 2));
		}
	}

	for (int i = 0; i < 2; i++)
	{
		m_floor[i].PushPrefilteredMap(m_HDREnvir.GetMeshTexture(0, 3));
		for (int j = 0; j < 2; j++)
		{
			m_floor[i].PushPrefilteredMap(m_LightProbe[j].GetMeshTexture(0, 2));
		}
	}

	for (int i = 0; i < 2; i++)
	{
		m_ceil[i].PushPrefilteredMap(m_HDREnvir.GetMeshTexture(0, 3));
		for (int j = 0; j < 2; j++)
		{
			m_ceil[i].PushPrefilteredMap(m_LightProbe[j].GetMeshTexture(0, 2));
		}
	}

	m_areaLight.PushPrefilteredMap(m_HDREnvir.GetMeshTexture(0, 3));
	for (int j = 0; j < 2; j++)
	{
		m_areaLight.PushPrefilteredMap(m_LightProbe[j].GetMeshTexture(0, 2));
	}
}

void GIScene::IntegrateBRDF()
{
	m_HDREnvir.SetMeshEffectWithIndex(&m_IntegrateBRDFEffect, 1);
	m_HDREnvir.InputAssemble(1);

	m_samplerCam.BindFBO(0);

	glViewport(0, 0, m_BRDFLUTResolution, m_BRDFLUTResolution);
	glDepthMask(GL_TRUE);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	Effect::cbResize.proj = m_samplerCam.GetProj();
	glBindBuffer(GL_UNIFORM_BUFFER, Effect::m_UBOid[2]);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(CBChangesOnResize), &Effect::cbResize, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	m_samplerCam.SetTarget(vec3(0.0f, 0.0f, 1.0f));
	m_samplerCam.LookAt(vec3(0.0f, -1.0f, 0.0f));
	Effect::cbFrame.view = m_samplerCam.GetView();
	glBindBuffer(GL_UNIFORM_BUFFER, Effect::m_UBOid[1]);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(CBChangesEveryFrame), &Effect::cbFrame, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	//Bind textures to the frame buffer
	glBindRenderbuffer(GL_RENDERBUFFER, m_samplerCam.GetRBO(0));
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, m_BRDFLUTResolution, m_BRDFLUTResolution);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_HDREnvir.GetMeshTextureID(1, 0), 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//Integrate BRDF
	m_HDREnvir.IntegrateBRDF();

	//Save LUT in local files
	GLfloat* data = new GLfloat[m_BRDFLUTResolution * m_BRDFLUTResolution * 3];
	glBindTexture(GL_TEXTURE_2D, m_HDREnvir.GetMeshTextureID(1, 0));
	glGetTexImage(GL_TEXTURE_2D, 0, GL_RGB, GL_FLOAT, data);
	std::string savePath = "../Project5\\Assets\\Precomputation\\" + m_sceneName + "Probe" + m_HDREnvir.getProbeName() + "\\BRDFLUT.hdr";
	stbi_write_hdr(savePath.c_str(), m_BRDFLUTResolution, m_BRDFLUTResolution, 3, data);

	//m_sphere.PushLUT(m_HDREnvir.GetMeshTexture(1, 0));
	for (int i = 0; i < 36; i++)
	{
		m_sphere[i].PushLUT(m_HDREnvir.GetMeshTexture(1, 0));
	}

	for (int i = 0; i < 6; i++)
	{
		m_Wall[i].PushLUT(m_HDREnvir.GetMeshTexture(1, 0));
	}

	for (int i = 0; i < 2; i++)
	{
		m_floor[i].PushLUT(m_HDREnvir.GetMeshTexture(1, 0));
	}

	for (int i = 0; i < 2; i++)
	{
		m_ceil[i].PushLUT(m_HDREnvir.GetMeshTexture(1, 0));
	}

	m_areaLight.PushLUT(m_HDREnvir.GetMeshTexture(1, 0));
}

void GIScene::Bake()
{
	//Sample Surrounding
	SampleSurrounding();

	//Diffuse
	BakeIrradianceMap();

	//Specular
	//Prefiltered environment map
	BakePrefilteredMap();
	//Integrating BRDF
	IntegrateBRDF();
}

void GIScene::InitiScene()
{
	//Game object
	//Load buffers of meshes
	
	//m_sphere.InputAssemble();

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
	//CubeMap -> Dual-paraboloid Map
	//if(!m_HDREnvir.HasDualParaMap())
		//TransformCubeToDualPara();
	//Bake GI
	Bake();



	//Bind rendering effect
	m_HDREnvir.SetMeshEffectWithIndex(&m_HDREnvMapEffect, 0);
	m_HDREnvir.InputAssemble(0);

	for (int i = 0; i < 36; i++)
	{
		m_sphere[i].InputAssemble();
	}

	for (int i = 0; i < 6; i++)
	{
		m_Wall[i].SetMeshEffectWithIndex(&m_basicPBRProbeEffect, 0);
		m_Wall[i].InputAssemble();
	}

	for (int i = 0; i < 2; i++)
	{
		m_floor[i].SetMeshEffectWithIndex(&m_basicPBRProbeEffect, 0);
		m_floor[i].InputAssemble();
	}

	for (int i = 0; i < 2; i++)
	{
		m_ceil[i].SetMeshEffectWithIndex(&m_basicPBRProbeEffect, 0);
		m_ceil[i].InputAssemble();
	}
	m_areaLight.SetMeshEffectWithIndex(&m_basicPBRProbeEffect, 0);
	m_areaLight.InputAssemble();
}

void GIScene::ComputeWeights()
{
	float weight0;
	float weight1;
	float weight;
	for (int i = 0; i < 6; i++)
	{
		weight0 = m_LightProbe[0].ComputeWeight(m_Wall[i].GetTransform().GetPosition());
		weight1 = m_LightProbe[1].ComputeWeight(m_Wall[i].GetTransform().GetPosition());
		weight = weight0 + weight1;
		m_Wall[i].m_probeWeight[0] = weight0 / weight;
		m_Wall[i].m_probeWeight[1] = weight1 / weight;
		m_Wall[i].m_probePos[0] = m_LightProbe[0].GetTransform().GetPosition();
		m_Wall[i].m_probePos[1] = m_LightProbe[1].GetTransform().GetPosition();
	}
	for (int i = 0; i < 2; i++)
	{
		weight0 = m_LightProbe[0].ComputeWeight(m_floor[i].GetTransform().GetPosition());
		weight1 = m_LightProbe[1].ComputeWeight(m_floor[i].GetTransform().GetPosition());
		weight = weight0 + weight1;
		m_floor[i].m_probeWeight[0] = weight0 / weight;
		m_floor[i].m_probeWeight[1] = weight1 / weight;
		m_floor[i].m_probePos[0] = m_LightProbe[0].GetTransform().GetPosition();
		m_floor[i].m_probePos[1] = m_LightProbe[1].GetTransform().GetPosition();
	}
	for (int i = 0; i < 2; i++)
	{
		weight0 = m_LightProbe[0].ComputeWeight(m_ceil[i].GetTransform().GetPosition());
		weight1 = m_LightProbe[1].ComputeWeight(m_ceil[i].GetTransform().GetPosition());
		weight = weight0 + weight1;
		m_ceil[i].m_probeWeight[0] = weight0 / weight;
		m_ceil[i].m_probeWeight[1] = weight1 / weight;
		m_ceil[i].m_probePos[0] = m_LightProbe[0].GetTransform().GetPosition();
		m_ceil[i].m_probePos[1] = m_LightProbe[1].GetTransform().GetPosition();
	}
	weight0 = m_LightProbe[0].ComputeWeight(m_areaLight.GetTransform().GetPosition());
	weight1 = m_LightProbe[1].ComputeWeight(m_areaLight.GetTransform().GetPosition());
	weight = weight0 + weight1;
	m_areaLight.m_probeWeight[0] = weight0 / weight;
	m_areaLight.m_probeWeight[1] = weight1 / weight;
	m_areaLight.m_probePos[0] = m_LightProbe[0].GetTransform().GetPosition();
	m_areaLight.m_probePos[1] = m_LightProbe[1].GetTransform().GetPosition();
}

void GIScene::RenderObjects()
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
	//for(int i = 0; i < 36; i++)
		//m_sphere[i].DrawPBR();
	for (int i = 0; i < 6; i++)
		m_Wall[i].DrawPBR();
	for (int i = 0; i < 2; i++)
		m_floor[i].DrawPBR();
	for (int i = 0; i < 2; i++)
		m_ceil[i].DrawPBR();
	m_areaLight.DrawPBR();
	//Draw Environment map
	glDepthFunc(GL_LEQUAL);
	m_HDREnvir.Draw(1);

}

void GIScene::DisplayScene()
{
	ComputeWeights();
	RenderObjects();
}

void GIScene::UpdateScene()
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
	//Update time
	m_preTime = m_curTime;
}

void GIScene::OnResize(int width, int height)
{
	m_camera.SetAspect(width, height);
	m_camera.SetFrustum(60.0f, m_camera.GetAspect(), 0.5f, 1000.0f);
	m_camera.SetProjPerspective();

	//Update constant buffer(change on resize)
	Effect::cbResize.proj = m_camera.GetProj();
}