#include "AnimationScene3.h"

float AnimationScene3::m_preDeviation = 1.0f;
float AnimationScene3::m_distanceWeight = 1.0f;
std::vector<float> AnimationScene3::blendWeights;

void AnimationScene3::Start()
{
	//Shaders
	//Basic PBR shader
	m_basicPBRVertexShader = Shader(GL_VERTEX_SHADER, "../Project5\\Assets\\Shaders\\BasicPBRVS.glsl");
	m_basicPBRFragShader = Shader(GL_FRAGMENT_SHADER, "../Project5\\Assets\\Shaders\\BasicPBRFS.glsl");
	//Basic PBR blend shader
	m_basicPBRBlendVertexShader = Shader(GL_VERTEX_SHADER, "../Project5\\Assets\\Shaders\\BasicPBRFacialVS.glsl");
	m_basicPBRBlendFragShader = Shader(GL_FRAGMENT_SHADER, "../Project5\\Assets\\Shaders\\BasicPBRFacialFS.glsl");
	//Simple shade shader
	m_simpleShadeVertexShader = Shader(GL_VERTEX_SHADER, "../Project5\\Assets\\Shaders\\SimpleShadeVS.glsl");
	m_simpleShadeFragShader = Shader(GL_FRAGMENT_SHADER, "../Project5\\Assets\\Shaders\\SimpleShadeFS.glsl");
	
	//Effects
	//Basic PBR lighting effect
	m_basicPBREffect.AddShader(&m_basicPBRVertexShader, &m_basicPBRFragShader);
	m_basicPBREffect.BindShaders();
	//Basic PBR blend effect
	m_basicPBRBlendEffect.AddShader(&m_basicPBRBlendVertexShader, &m_basicPBRBlendFragShader);
	m_basicPBRBlendEffect.BindShaders();
	//Simple shade effect
	m_simpleShadeEffect.AddShader(&m_simpleShadeVertexShader, &m_simpleShadeFragShader);
	m_simpleShadeEffect.BindShaders();

	//Load lights
	//Directional light
	PointLight pointLight;
	pointLight.lightColor = vec4(23.47f, 21.31f, 20.79f, 1.0f);
	pointLight.position = vec3(5.0, 5.0, 5.0);
	pointLight.att = vec3(0.0f, 0.0f, 0.1f);
	m_pointLight.push_back(pointLight);


	//Load GameObjects
	//Face
	m_faceLow = Face(VertexPosNormal, "High-res Blendshape Model", "neutral.obj", true);
	m_faceLow.GetTransform().SetScale(vec3(0.1f, 0.1f, 0.1f));
	m_faceLow.GetTransform().SetPosition(vec3(0.0f, -1.5f, 0.0f));
	m_faceLow.LoadPBRGameObject(vec3(1.0f), 0.1f, 0.7f);

	m_blendWeightCache.push_back(m_faceLow.GetBlendWeight());

	m_manipulatorCent = Ball(VertexPosNormalTex, 0.5f, 50, 50, vec4(1.0f), "PBRRustedIron");
	m_manipulatorCent.GetTransform().SetScale(vec3(0.1f));
	m_manipulatorCent.LoadPBRGameObject(vec3(0.2f, 0.8f, 0.0f), 0.0f, 1.0f);

	m_manipulatorArrow[0] = Arrow(VertexPos, 0.1f, 0.4f, 50, vec4(1.0, 0.0f, 0.0f, 0.0f));
	m_manipulatorArrow[1] = Arrow(VertexPos, 0.1f, 0.4f, 50, vec4(0.0, 1.0f, 0.0f, 0.0f));
	m_manipulatorArrow[2] = Arrow(VertexPos, 0.1f, 0.4f, 50, vec4(0.0, 0.0f, 1.0f, 0.0f));
	for (int i = 0; i < 3; i++)
	{
		m_manipulatorArrow[i].GetTransform().SetScale(vec3(0.4f));
		m_manipulatorArrow[i].LoadGameObject();
	}
	m_manipulatorArrow[1].GetTransform().SetRotation(vec3(0.0f, 0.0f, radians(90.0f)));
	m_manipulatorArrow[2].GetTransform().SetRotation(vec3(0.0f, radians(90.0f), 0.0f));

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

void AnimationScene3::InitiScene()
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
	//m_faceLow.SetMeshEffectWithIndex(&m_basicPBREffect, 0);
	//m_faceLow.InputAssemble();
	m_faceLow.SetEffect(&m_basicPBRBlendEffect);
	m_faceLow.Assemble();

	m_manipulatorCent.SetMeshEffectWithIndex(&m_basicPBREffect, 0);
	m_manipulatorCent.InputAssemble();

	for (int j = 0; j < 3; j++)
	{
		for (int i = 0; i < m_manipulatorArrow[j].GetMeshCount(); i++)
			m_manipulatorArrow[j].SetMeshEffectWithIndex(&m_simpleShadeEffect, i);
		m_manipulatorArrow[j].InputAssemble();
	}
}

void AnimationScene3::RenderObjects()
{
	//Set camera proj view
	glViewport(0, 0, m_sceneWidth, m_sceneHeight);
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
	
	if(m_lineMode)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	//m_faceLow.DrawPBR();
	m_faceLow.DrawPBRBlend();

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDisable(GL_DEPTH_TEST);
	if (m_vertexPicked)
	{
		m_manipulatorCent.DrawPBR();
		for (int i = 0; i < 3; i++)
			m_manipulatorArrow[i].Draw();
	}
}

void AnimationScene3::DisplayScene()
{
	RenderObjects();
}

void AnimationScene3::UpdateScene()
{
	m_curTime = glutGet(GLUT_ELAPSED_TIME);
	float deltaTime = (m_curTime - m_preTime) * 0.001f;
	//Update camera transform
	if (!m_editMode)
	{
		//Compute distance to target
		m_camera.ComputeDistance();
		m_camera.RotateX(mouse.m_mouseState.deltaY * deltaTime);
		m_camera.RotateY(mouse.m_mouseState.deltaX * deltaTime);
		//Update view
		m_camera.LookAt(vec3(0.0f, 1.0f, 0.0f));
		Effect::cbFrame.eyePos = m_camera.GetTransform().GetPosition();
	}

	//Change edit mode
	if (keyboard.keyState.V && m_modeTimer > 0.5f)
	{
		m_modeTimer = 0.0f;
		m_editMode = !m_editMode;
		if (m_editMode)
		{
			//Set cursor visibility
			mouse.initMouseCursor();
			useOriginalMouseMotion = false;
		}
		else
		{
			//Set cursor visibility
			mouse.initMouseNoCursor();
			useOriginalMouseMotion = true;
		}
	}
	m_modeTimer += deltaTime;

	//Change render states
	if (keyboard.keyState.N && m_stateTimer > 0.5f)
	{
		m_stateTimer = 0.0f;
		m_lineMode = !m_lineMode;
	}
	m_stateTimer += deltaTime;

	//Check mouse
	vec2 pickPos = vec2((mouse.m_mouseState.x / (float)m_sceneWidth - 0.5f) * 2.0f, -((mouse.m_mouseState.y / (float)m_sceneHeight - 0.5f)) * 2.0f);
	if (m_editMode)
	{
		if (mouse.m_mouseState.leftButton && !m_leftPrevious)
		{
			CheckOnClickState(pickPos);
		}
		if (!mouse.m_mouseState.leftButton)
		{
			CheckPassiveState(pickPos);
		}
		if (mouse.m_mouseState.leftButton && m_leftPrevious)
		{
			CheckHoldState(pickPos);
		}
	}


	//Display animation
	if (keyboard.keyState.E && m_animationModeTimer > 0.5f)
	{
		m_animationModeTimer = 0.0f;
		m_animationMode = !m_animationMode;
		if (m_vertexPicked)
		{
			m_blendWeightCache.push_back(m_faceLow.GetBlendWeight());
			m_vertexPicked = false;
		}
	}
	m_animationModeTimer += deltaTime;

	if (m_animationMode)
	{
		DisplayAnimation(deltaTime);
	}


	//Update time
	m_preTime = m_curTime;
	//Update mouse
	m_leftPrevious = mouse.m_mouseState.leftButton;

	mouse.m_mouseState.deltaX = 0;
	mouse.m_mouseState.deltaY = 0;
}

void AnimationScene3::DisplayAnimation(float dt)
{
	float animationTime = m_blendWeightCache.size() - 1;
	int currBegin = m_animationTimer;
	int currEnd = m_animationTimer + 1;

	float interpolation = m_animationTimer - currBegin;
	for (int i = 0; i < m_blendWeightCache[currBegin].size(); i++)
	{
		m_faceLow.SetBlendWeight(i, m_blendWeightCache[currBegin][i] * (1 - interpolation) + interpolation * m_blendWeightCache[currEnd][i]);
	}

	m_animationTimer += dt;
	if (m_animationTimer >= animationTime)
		m_animationTimer = 0.0f;
}

void AnimationScene3::CheckOnClickState(vec2 pickPos)
{
	//Change pick state
	if (!m_vertexPicked)
	{
		//Select picked vertex
		int pickedVertexIndex = m_faceLow.PickVertex(m_faceLow.getNeutralIndex(), pickPos, Effect::cbResize.proj, Effect::cbFrame.view, m_targetVertex, m_camera.GetTransform().GetForwardAxis());
		if (pickedVertexIndex != -1)
		{
			m_manipulatorCent.GetTransform().SetPosition(m_faceLow.GetWorldMatrix() * vec4(m_faceLow.GetMesh(0).m_posL[pickedVertexIndex], 1.0f));
			for (int i = 0; i < 3; i++)
				m_manipulatorArrow[i].GetTransform().SetPosition(m_faceLow.GetWorldMatrix() * vec4(m_faceLow.GetMesh(0).m_posL[pickedVertexIndex], 1.0f));
			m_vertexPicked = true;
			m_targetObject = &m_faceLow;
			m_targetMesh = m_faceLow.getNeutralIndex();
			
		}
	}
	else
	{
		//check manipulator first
		int pickedVertexIndex;
		if (m_currArrow != -1)
		{
			//change state
		}
		//not manipulator
		else
		{
			//Check vertex
			pickedVertexIndex = m_faceLow.PickVertex(m_faceLow.getNeutralIndex(), pickPos, Effect::cbResize.proj, Effect::cbFrame.view, m_targetVertex, m_camera.GetTransform().GetForwardAxis());
			if (pickedVertexIndex != -1)
			{
				m_manipulatorCent.GetTransform().SetPosition(m_faceLow.GetWorldMatrix() * vec4(m_faceLow.GetMesh(0).m_posL[pickedVertexIndex], 1.0f));
				for (int i = 0; i < 3; i++)
					m_manipulatorArrow[i].GetTransform().SetPosition(m_faceLow.GetWorldMatrix() * vec4(m_faceLow.GetMesh(0).m_posL[pickedVertexIndex], 1.0f));
				m_vertexPicked = true;
				m_targetObject = &m_faceLow;
				m_targetMesh = m_faceLow.getNeutralIndex();
				//For sequence animation
				m_blendWeightCache.push_back(m_faceLow.GetBlendWeight());
			}
		}
	}
}

void AnimationScene3::CheckPassiveState(vec2 pickPos)
{
	if (m_vertexPicked)
	{
		//check manipulator first
		int pickedVertexIndex;
		int i;
		for (i = 0; i < 3; i++)
		{
			for (int j = 0; j < 2; j++)
			{
				std::vector<unsigned int> temp;
				pickedVertexIndex = m_manipulatorArrow[i].PickVertex(j, pickPos, Effect::cbResize.proj, Effect::cbFrame.view, temp, m_camera.GetTransform().GetForwardAxis());
				if (pickedVertexIndex != -1)
				{
					//change state
					if (m_currArrow != -1)
					{
						//clear previous color
						m_manipulatorArrow[m_currArrow].ResetColor();
					}
					m_currArrow = i;
					m_manipulatorArrow[i].SetColor(vec4(1.0f, 1.0f, 0.0f, 0.0f));
					break;
				}
			}
			if (pickedVertexIndex != -1)
			{
				break;
			}
		}
		if (pickedVertexIndex == -1)
		{
			//No arrow found
			if (m_currArrow != -1)
			{
				//reset color
				m_manipulatorArrow[m_currArrow].ResetColor();
			}
			m_currArrow = -1;

		}
	}
}

void AnimationScene3::CheckHoldState(vec2 pickPos)
{
	if (m_vertexPicked)
	{
		if (m_currArrow != -1)
		{
			vec4 centPosClip = Effect::cbResize.proj * Effect::cbFrame.view * vec4(m_manipulatorCent.GetTransform().GetPosition(), 1.0f);
			vec2 centPosNDC = vec2(centPosClip.x / centPosClip.w, centPosClip.y / centPosClip.w);
			//vec2 dirNDC = glm::normalize(pickPos - centPosNDC);
			//float distanceNDC = glm::distance(pickPos, centPosNDC);
			vec2 dirNDC = vec2(mouse.m_mouseState.deltaX / (float)m_sceneWidth, -mouse.m_mouseState.deltaY / (float)m_sceneHeight);
			float distanceNDC = glm::length(dirNDC) * glm::distance(m_camera.GetTransform().GetPosition(), m_manipulatorCent.GetTransform().GetPosition()) * 2;
			if (distanceNDC != 0)
			{
				dirNDC = glm::normalize(dirNDC);
				vec3 dirWorld = dirNDC.x * m_camera.GetTransform().GetRightAxis() + dirNDC.y * m_camera.GetTransform().GetUpAxis();
				vec3 arrowDir;
				if (m_currArrow == 0)
				{
					arrowDir = m_manipulatorCent.GetTransform().GetRightAxis();
				}
				else if (m_currArrow == 1)
				{
					arrowDir = m_manipulatorCent.GetTransform().GetUpAxis();
				}
				else if (m_currArrow == 2)
				{
					arrowDir = m_manipulatorCent.GetTransform().GetForwardAxis();
				}
				float costheta = glm::dot(arrowDir, dirWorld);
				//Compute marker offset
				m_manipulatorCent.GetTransform().SetPosition(m_manipulatorCent.GetTransform().GetPosition() + costheta * distanceNDC * arrowDir);
				for (int i = 0; i < 3; i++)
					m_manipulatorArrow[i].GetTransform().SetPosition(m_manipulatorCent.GetTransform().GetPosition());
				vec3 markerOffset = vec3(glm::inverse(m_faceLow.GetWorldMatrix()) * vec4(m_manipulatorCent.GetTransform().GetPosition(), 1.0f)) - m_faceLow.GetMesh(m_faceLow.getNeutralIndex()).m_posL[m_targetVertex[0]];
				//Minimize least square
				int rowNum = 3;
				int columnNum = m_faceLow.GetMeshCount() - 1;
				//Current vertex blendShape
				Eigen::MatrixXf vertexBlendShape = Eigen::MatrixXf(rowNum, columnNum);
				for (int i = 0; i < m_faceLow.GetMeshCount(); i++)
				{
					//Load positions to the matrix
					if (i != m_faceLow.getNeutralIndex())
					{
						vertexBlendShape(0, i) = m_faceLow.GetMesh(i).m_posL[m_targetVertex[0]].x - m_faceLow.GetMesh(m_faceLow.getNeutralIndex()).m_posL[m_targetVertex[0]].x;
						vertexBlendShape(1, i) = m_faceLow.GetMesh(i).m_posL[m_targetVertex[0]].y - m_faceLow.GetMesh(m_faceLow.getNeutralIndex()).m_posL[m_targetVertex[0]].y;
						vertexBlendShape(2, i) = m_faceLow.GetMesh(i).m_posL[m_targetVertex[0]].z - m_faceLow.GetMesh(m_faceLow.getNeutralIndex()).m_posL[m_targetVertex[0]].z;
					}
				}
				//Get transpose
				Eigen::MatrixXf vertexBlendShapeTrans = vertexBlendShape.transpose();
				//Multiplied blendshape
				Eigen::MatrixXf mulVertexBlendShape = vertexBlendShapeTrans * vertexBlendShape;
				//Compute deviation
				Eigen::MatrixXf deviationMat = Eigen::MatrixXf::Identity(vertexBlendShape.cols(), vertexBlendShape.cols());
				deviationMat *= (m_distanceWeight + m_preDeviation);
				//Final matrix
				Eigen::MatrixXf finalMat = mulVertexBlendShape + deviationMat;
				//Compute marker offset
				Eigen::Vector3f markerOffsetEigen = Eigen::Vector3f(markerOffset.x, markerOffset.y, markerOffset.z);
				//Align previous weight
				Eigen::VectorXf preWeightVec = Eigen::VectorXf(m_faceLow.GetBlendWeight().size() - 1);
				std::vector<float> preWeight = m_faceLow.GetBlendWeight();
				for (int i = 0; i < m_faceLow.GetBlendWeight().size(); i++)
				{
					if (i != m_faceLow.getNeutralIndex())
					{
						preWeightVec[i] = preWeight[i];
					}
				}
				//Final value
				Eigen::VectorXf finalValue = vertexBlendShapeTrans * markerOffsetEigen + m_preDeviation * preWeightVec;
				//Update weight
				Eigen::LDLT<Eigen::MatrixXf> minSolver(finalMat);
				Eigen::VectorXf updatedWeights = minSolver.solve(finalValue);
				for (int i = 0; i < m_faceLow.GetBlendWeight().size() - 1; i++)
				{
					float weight = 0.0f;
					for (int j = 0; j < m_blendWeightCache.size(); j++)
					{
						weight += m_blendWeightCache[j][i];
					}
					weight += updatedWeights[i];
					weight = glm::clamp(weight, 0.0f, 1.0f);
					m_faceLow.SetBlendWeight(i, weight);
				}
			}
		}
	}
}

void AnimationScene3::OnResize(int width, int height)
{
	m_camera.SetAspect(width, height);
	m_camera.SetFrustum(60.0f, m_camera.GetAspect(), 0.5f, 1000.0f);
	m_camera.SetProjPerspective();

	//Update constant buffer(change on resize)
	Effect::cbResize.proj = m_camera.GetProj();
}

void AnimationScene3::GUIDisplay()
{
	blendWeights = m_faceLow.GetBlendWeight();
	
	ImGui::Begin("Display Manager");
	ImGui::SetWindowPos(ImVec2(0.0f, 0.0f));
	ImGui::SetWindowSize(ImVec2(500.0f, 500.0f));
	ImGui::Checkbox("Vertex Edit mode (Press V)", &m_editMode);
	ImGui::Checkbox("Render lines (Press N)", &m_lineMode);
	ImGui::Checkbox("Display Animation (Press E)", &m_animationMode);

	ImGui::SliderFloat("Deviation", &m_preDeviation, 0.0f, 10.0f);
	ImGui::SliderFloat("Small Weight", &m_distanceWeight, 0.0f, 10.0f);

	for (int i = 0; i < blendWeights.size() - 1; i++)
	{
		ImGui::SliderFloat(m_faceLow.GetMesh(i).m_name.c_str(), &blendWeights[i], 0.0f, 1.0f);
		m_faceLow.SetBlendWeight(i, blendWeights[i]);
	}

	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	ImGui::End();
}

void AnimationScene3::MouseMotion(int x, int y)
{

}