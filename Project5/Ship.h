#ifndef SHIP_H
#define SHIP_H

#include "GameObject.h"
#include "Keyboard.h"
#include "RenderTexture.h"
#include "Ball.h"

extern Keyboard keyboard;
extern Mouse mouse;

class Ship : public GameObject
{
private:
	std::string m_modelPath;
	std::string m_textureDir;
	std::string m_tag;

	//Property
	bool m_playerTag;

	vec3 m_forward;
	vec3 m_up;
	vec3 m_right;

	float m_vertAcc = 0.0f;
	float m_vertSpeed = 0.0f;
	float m_vertSpeedDecay = 0.1f;
	float m_forwardCount = 0.0f;
	float m_rightCount = 0.0f;
	float m_forwartAcc = 0.0f;
	float m_rightAcc = 0.0f;

	float m_moveAcc = 0.0f;
	float m_moveSpeed = 0.0f;

	float m_turnAcc = 0.0f;
	float m_turnSpeed = 0.0f;
	bool m_turnRight = false;

	int m_wheelIndex;
	bool m_wheelBackTurning = false;
	bool m_wheelBackTurnAble = false;

	float m_reloadTime = 10.0f;
	float m_reloadTimeLeft = 0.0f;
	float m_reloadTimeRight = 0.0f;

	float m_cannonSpacingPos[8] = {2.25f, 1.65f, 1.05f, 0.45f, -0.15f, -1.25f, -1.8f};
	float m_cannonHorDistance = 1.7f;
	float m_cannonVertDistance = 0.3f;

	int m_health = 16;
	float m_sinkTime = 0.0f;
	float m_sinkTurnSpeed = 0.2f;
	float m_sinkSpeed = 0.0f;

	//Depth Textures
	RenderTexture m_depthTex;
	RenderTexture m_oceanDepthTex;
	RenderTexture m_shipDepthTex;

	//Generate Depth Texture
	void GenerateDepthTex();

	void UpdateVert(GLfloat* shipDepth, GLfloat* oceanDepth, float dt);
	void UpdateHor(vec3 wind, float dt);
	void UpdateTransform(float dt);
	void Sink(float dt);

	void Reload(float dt);

public:
	Ship() = default;
	Ship(VertexType type, std::string modelDirectory, std::string modelName, bool tag)
	{
		m_type = type;
		m_modelPath = "../Project5\\Assets\\Models\\" + modelDirectory + "\\" + modelName;
		m_textureDir = "../Project5\\Assets\\Textures\\" + modelDirectory;
		m_playerTag = tag;
	}
	~Ship() {}
	void LoadGameObject();
	void Draw();
	vec3 Update(GLsizei size, Camera camera, float dt, vec3 wind, GLfloat* shipDepth, GLfloat* oceanDepth);
	void Fire(vec3 viewDir, std::vector<Ball>& rounds);
	void Hit();

	RenderTexture GetOceanDepth()
	{
		return m_oceanDepthTex;
	}

	RenderTexture GetShipDepth()
	{
		return m_shipDepthTex;
	}

	RenderTexture GetDepth()
	{
		return m_depthTex;
	}

	std::vector<GLuint> GetDepthID()
	{
		std::vector<GLuint> vec;
		vec.push_back(m_depthTex.GetTextureID());
		vec.push_back(m_oceanDepthTex.GetTextureID());
		vec.push_back(m_shipDepthTex.GetTextureID());
		return vec;
	}

	std::vector<GLenum> GetDepthReadWrite()
	{
		std::vector<GLenum> vec;
		vec.push_back(m_depthTex.GetReadWrite());
		vec.push_back(m_oceanDepthTex.GetReadWrite());
		vec.push_back(m_shipDepthTex.GetReadWrite());
		return vec;
	}

	std::vector<GLenum> GetDepthFormat()
	{
		std::vector<GLenum> vec;
		vec.push_back(m_depthTex.GetFormat());
		vec.push_back(m_oceanDepthTex.GetFormat());
		vec.push_back(m_shipDepthTex.GetFormat());
		return vec;
	}

};

#endif