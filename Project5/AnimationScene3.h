#ifndef ANIMATION_SCENE_3_H
#define ANIMATION_SCENE_3_H

#include <GL/glew.h>
#include <vector>
#include "GameScene.h"
#include "Effect.h"
#include "Face.h"
#include "Ball.h"
#include "Arrow.h"
#include "Eigen/Dense"

extern bool useOriginalMouseMotion;

class AnimationScene3 : public GameScene
{
private:
	GLsizei m_sceneWidth = 1280;
	GLsizei m_sceneHeight = 720;
	//Mode
	float m_modeTimer = 0.0f;
	bool m_editMode = false;
	float m_animationModeTimer = 0.0f;
	bool m_animationMode = false;
	float m_animationTimer = 0.0f;
	//Render state
	float m_stateTimer = 0.0f;
	bool m_lineMode = false;
	//Pick state
	bool m_vertexPicked = false;
	bool m_leftPrevious = false;
	//Pasive state
	int m_currArrow = -1;

	//manipulate parameters
	static float m_preDeviation;
	static float m_distanceWeight;

	static std::vector<float> blendWeights;
	std::vector<std::vector<float>> m_blendWeightCache;

	//Time
	float m_curTime;
	float m_preTime;

	//Shaders
	//Basic PBR shader
	Shader m_basicPBRVertexShader;
	Shader m_basicPBRFragShader;
	//Basic PBR blend shader
	Shader m_basicPBRBlendVertexShader;
	Shader m_basicPBRBlendFragShader;
	//Simple shader
	Shader m_simpleShadeVertexShader;
	Shader m_simpleShadeFragShader;

	//Effects
	//Basic phong lighting effect
	Effect m_basicPBREffect;
	//Basic PBR blend effect
	Effect m_basicPBRBlendEffect;
	//Simple shade effect
	Effect m_simpleShadeEffect;

	//Lights
	//Point light
	std::vector<PointLight> m_pointLight;
	//Directional light
	std::vector<DirectionalLight> m_directLight;

	//GameObjects
	//Face
	Face m_faceLow;
	//Manipulator
	//Sphere
	Ball m_manipulatorCent;
	//Arrows
	Arrow m_manipulatorArrow[3];

	//Manipulating target
	GameObject* m_targetObject;
	unsigned int m_targetMesh;
	std::vector<unsigned int> m_targetVertex;

	//Cameras
	Camera m_camera;

	//Draw Calls
	void RenderObjects();

	void CheckOnClickState(vec2 pickPos);
	void CheckPassiveState(vec2 pickPos);
	void CheckHoldState(vec2 pickPos);
	void DisplayAnimation(float dt);


public:
	AnimationScene3() {}
	void Start();
	void InitiScene();
	void DisplayScene();
	void UpdateScene();
	void OnResize(int width, int height);
	void GUIDisplay();
	void MouseMotion(int x, int y);

};

#endif