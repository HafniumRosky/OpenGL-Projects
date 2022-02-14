#ifndef EFFECT_H
#define EFFECT_H

#include <GL/glew.h>
#include "Shader.h"
#include <glm.hpp>
#include "LightHelper.h"
#include <vector>

using namespace glm;

struct CBChangesEveryDrawing
{
	mat4 world;
	mat4 worldInvTranspose;
	Material material;
};

struct CBChangesEveryDrawingPBR
{
	mat4 world;
	mat4 worldInvTranspose;
	PBRMaterial material;
};

struct CBChangesEveryFrame
{
	mat4 view;
	mat4 shadowMatrix;
	vec3 eyePos;
	float pad;
};

struct CBChangesOnResize
{
	mat4 proj;
};

struct CBRenderState
{
	bool renderDepth;
	bool pad0;
	bool pad1;
	bool pad2;
	vec3 pad3;
};

struct CBChangesRarely
{
	//mat4 reflection;
	//mat4 shadow;
	//mat4 refShadow;
	DirectionalLight dirLight[1];
	PointLight pointLight[4];
	int numDirLight;
	int numPointLight;
	float pad[2];
};
//This class is for managing input assembly part
class Effect
{
private:
	GLuint	m_shaderProgramID;				//It is used to identify current shader program					

	unsigned int m_textureNum = 0;
	unsigned int m_bindedTextureNum = 0;

	Shader*	m_pVertexShader	= nullptr;		//Vertex shader
	Shader* m_pHullShader = nullptr;		//TESS control shader
	Shader* m_pDomainShader = nullptr;		//TESS evaluation shader
	Shader* m_pGeometryShader = nullptr;	//Geometry shader
	Shader*	m_pFragShader	= nullptr;		//Fragment shader

	Shader* m_pComputeShader = nullptr;		//Compute shader

public:
	//Constant Buffers
	static CBChangesEveryDrawing cbDraw;
	static CBChangesEveryFrame cbFrame;
	static CBChangesOnResize cbResize;
	static CBChangesRarely cbRare;
	static CBRenderState cbState;
	static CBChangesEveryDrawingPBR cbDrawPBR;

	static GLuint m_UBOid[6];

	//A constructor which binds a shader program to the effect
	Effect()
	{
		m_shaderProgramID = glCreateProgram();
		if (m_shaderProgramID == 0) 
		{
			fprintf(stderr, "Error creating shader program\n");
			exit(1);
		}
	}

	~Effect()
	{
		delete m_pVertexShader;
		delete m_pHullShader;
		delete m_pDomainShader;
		delete m_pGeometryShader;
		delete m_pFragShader;
		delete m_pComputeShader;
	}

	//Add the shader into the class (Can override for different shader sets, this is the basic one)
	void AddShader(Shader* pShader);
	void AddShader(Shader* pVertexShader, Shader* pFragShader);
	void AddShader(Shader* pVertexShader, Shader* pHullShader, Shader* pDomainShader, Shader* pFragShader);
	
	//This is for data streaming(Must call before binding shaders)
	void BindTFV(const GLchar** varyings, int num);

	//Bind all the shaders to the shader program of the current effect (This must be done after adding all necessary shaders)
	void BindShaders();
	void BindShadersWithTFV(const GLchar** varyings);


	//Set all the render states into current effect
	// Note: this program will stay in effect for all draw calls until you replace it with another or explicitly disable its use
	void BindEffect();

	void BindTexture(GLuint texID, const char* texParaName);
	void BindTexture(std::vector<GLuint> texID, std::vector<const char*> texParaName, std::vector<GLenum> texTarget);
	void BindCubeMap(GLuint cubeID);

	static void BindConstantBuffer(CBChangesEveryDrawing* cbDraw, GLuint bindIndex);
	static void BindConstantBuffer(CBChangesEveryFrame* cbFrame, GLuint bindIndex);
	static void BindConstantBuffer(CBChangesOnResize* cbResize, GLuint bindIndex);
	static void BindConstantBuffer(CBChangesRarely* cbRare, GLuint bindIndex);
	static void BindConstantBuffer(CBRenderState* cbState, GLuint bindIndex);
	static void BindConstantBuffer(CBChangesEveryDrawingPBR* cbDrawPBR, GLuint bindIndex);

	void ComputeTexture(std::vector<GLuint> texID, std::vector<GLenum> readWrite, std::vector<GLenum> format, GLuint numGroupX, GLuint numGroupY, GLuint numGroupZ);
	void ComputeTexture(GLuint texID, GLenum readWrite, GLenum format, GLuint numGroupX, GLuint numGroupY, GLuint numGroupZ);

	GLuint getShaderProgramID()
	{
		return this->m_shaderProgramID;
	}

	void SetTextureNum(unsigned int num)
	{
		m_textureNum = num;
	}

	unsigned int GetTextureNum()
	{
		return m_textureNum;
	}

};


#endif
