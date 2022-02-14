#include "Effect.h"
#include <iostream>

CBChangesEveryDrawing Effect::cbDraw;
CBChangesEveryFrame Effect::cbFrame;
CBChangesOnResize Effect::cbResize;
CBChangesRarely Effect::cbRare;
CBRenderState Effect::cbState;
CBChangesEveryDrawingPBR Effect::cbDrawPBR;

GLuint Effect::m_UBOid[6];

void Effect::AddShader(Shader* pShader)
{
	if(pShader->ShaderType() == GL_VERTEX_SHADER)
		this->m_pVertexShader = pShader;
	else if (pShader->ShaderType() == GL_TESS_CONTROL_SHADER)
		this->m_pHullShader = pShader;
	else if (pShader->ShaderType() == GL_TESS_EVALUATION_SHADER)
		this->m_pDomainShader = pShader;
	else if (pShader->ShaderType() == GL_GEOMETRY_SHADER)
		this->m_pGeometryShader = pShader;
	else if (pShader->ShaderType() == GL_FRAGMENT_SHADER)
		this->m_pFragShader = pShader;
	else if (pShader->ShaderType() == GL_COMPUTE_SHADER)
		this->m_pComputeShader = pShader;
}

void Effect::AddShader(Shader* pVertexShader, Shader* pFragShader)
{
	this->m_pVertexShader = pVertexShader;
	this->m_pFragShader = pFragShader;
}

void Effect::AddShader(Shader* pVertexShader, Shader* pHullShader, Shader* pDomainShader, Shader* pFragShader)
{
	this->m_pVertexShader = pVertexShader;
	this->m_pHullShader = pHullShader;
	this->m_pDomainShader = pDomainShader;
	this->m_pFragShader = pFragShader;
}

void Effect::BindTFV(const GLchar** varyings, int num)
{
	glTransformFeedbackVaryings(m_shaderProgramID, num, varyings, GL_INTERLEAVED_ATTRIBS);
}

void Effect::BindShaders()
{
	if (this->m_pComputeShader != nullptr)
	{
		glAttachShader(this->m_shaderProgramID, m_pComputeShader->ShaderObject());
	}
	else
	{
		//Check if vertex shader is added
		if (this->m_pVertexShader == nullptr)
		{
			fprintf(stderr, "You haven't add vertex shader to this effect");
			exit(0);
		}
		glAttachShader(this->m_shaderProgramID, m_pVertexShader->ShaderObject());

		//Check if fragment shader is added
		if (this->m_pFragShader != nullptr)
		{
			glAttachShader(this->m_shaderProgramID, m_pFragShader->ShaderObject());
		}

		//Check if Tess is added
		if (this->m_pHullShader != nullptr && this->m_pDomainShader != nullptr)
		{
			glAttachShader(this->m_shaderProgramID, m_pHullShader->ShaderObject());
			glAttachShader(this->m_shaderProgramID, m_pDomainShader->ShaderObject());
		}

		//Check if geometry shader is added
		if (this->m_pGeometryShader != nullptr)
		{
			glAttachShader(this->m_shaderProgramID, m_pGeometryShader->ShaderObject());
		}
	}

	GLint Success = 0;
	GLchar ErrorLog[1024] = { 0 };
	// After compiling all shader objects and attaching them to the program, we can finally link it
	glLinkProgram(this->m_shaderProgramID);
	// check for program related errors using glGetProgramiv
	glGetProgramiv(this->m_shaderProgramID, GL_LINK_STATUS, &Success);
	if (Success == 0) 
	{
		glGetProgramInfoLog(this->m_shaderProgramID, sizeof(ErrorLog), NULL, ErrorLog);
		fprintf(stderr, "Error linking shader program: '%s'\n", ErrorLog);
		exit(1);
	}
	// program has been successfully linked but needs to be validated to check whether the program can execute given the current pipeline state
	glValidateProgram(this->m_shaderProgramID);
	// check for program related errors using glGetProgramiv
	glGetProgramiv(this->m_shaderProgramID, GL_VALIDATE_STATUS, &Success);
	if (!Success) 
	{
		glGetProgramInfoLog(this->m_shaderProgramID, sizeof(ErrorLog), NULL, ErrorLog);
		fprintf(stderr, "Invalid shader program: '%s'\n", ErrorLog);
		exit(1);
	}
}

void Effect::BindShadersWithTFV(const GLchar** varyings)
{
	if (this->m_pComputeShader != nullptr)
	{
		glAttachShader(this->m_shaderProgramID, m_pComputeShader->ShaderObject());
	}
	else
	{
		//Check if vertex shader is added
		if (this->m_pVertexShader == nullptr)
		{
			fprintf(stderr, "You haven't add vertex shader to this effect");
			exit(0);
		}
		glAttachShader(this->m_shaderProgramID, m_pVertexShader->ShaderObject());

		//Check if fragment shader is added
		if (this->m_pFragShader != nullptr)
		{
			glAttachShader(this->m_shaderProgramID, m_pFragShader->ShaderObject());
		}

		//Check if Tess is added
		if (this->m_pHullShader != nullptr && this->m_pDomainShader != nullptr)
		{
			glAttachShader(this->m_shaderProgramID, m_pHullShader->ShaderObject());
			glAttachShader(this->m_shaderProgramID, m_pDomainShader->ShaderObject());
		}

		//Check if geometry shader is added
		if (this->m_pGeometryShader != nullptr)
		{
			glAttachShader(this->m_shaderProgramID, m_pGeometryShader->ShaderObject());
		}
	}

	glTransformFeedbackVaryings(this->m_shaderProgramID, 4, varyings, GL_INTERLEAVED_ATTRIBS);

	GLint Success = 0;
	GLchar ErrorLog[1024] = { 0 };
	// After compiling all shader objects and attaching them to the program, we can finally link it
	glLinkProgram(this->m_shaderProgramID);
	// check for program related errors using glGetProgramiv
	glGetProgramiv(this->m_shaderProgramID, GL_LINK_STATUS, &Success);
	if (Success == 0)
	{
		glGetProgramInfoLog(this->m_shaderProgramID, sizeof(ErrorLog), NULL, ErrorLog);
		fprintf(stderr, "Error linking shader program: '%s'\n", ErrorLog);
		exit(1);
	}
	// program has been successfully linked but needs to be validated to check whether the program can execute given the current pipeline state
	glValidateProgram(this->m_shaderProgramID);
	// check for program related errors using glGetProgramiv
	glGetProgramiv(this->m_shaderProgramID, GL_VALIDATE_STATUS, &Success);
	if (!Success)
	{
		glGetProgramInfoLog(this->m_shaderProgramID, sizeof(ErrorLog), NULL, ErrorLog);
		fprintf(stderr, "Invalid shader program: '%s'\n", ErrorLog);
		exit(1);
	}
}

void Effect::BindEffect()
{
	glUseProgram(this->m_shaderProgramID);
}

void Effect::BindTexture(GLuint texID, const char* texParaName)
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texID);
	glUniform1i(glGetUniformLocation(m_shaderProgramID, texParaName), 0);
}

void Effect::BindTexture(std::vector<GLuint> texID, std::vector<const char*> texParaName, std::vector<GLenum> texTarget)
{
	for (int i = 0; i < texID.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(texTarget[i], texID[i]);
		glUniform1i(glGetUniformLocation(m_shaderProgramID, texParaName[i]), i);
	}
}

void Effect::BindCubeMap(GLuint cubeID)
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubeID);
}

void Effect::BindConstantBuffer(CBChangesEveryDrawing* cbDraw, GLuint bindIndex)
{
	glGenBuffers(1, &m_UBOid[0]);
	glBindBuffer(GL_UNIFORM_BUFFER, m_UBOid[0]);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(CBChangesEveryDrawing), cbDraw, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	glBindBufferRange(GL_UNIFORM_BUFFER, bindIndex, m_UBOid[0], 0, sizeof(CBChangesEveryDrawing));
}

void Effect::BindConstantBuffer(CBChangesEveryFrame* cbFrame, GLuint bindIndex)
{
	glGenBuffers(1, &m_UBOid[1]);
	glBindBuffer(GL_UNIFORM_BUFFER, m_UBOid[1]);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(CBChangesEveryFrame),cbFrame, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	glBindBufferRange(GL_UNIFORM_BUFFER, bindIndex, m_UBOid[1], 0, sizeof(CBChangesEveryFrame));
}

void Effect::BindConstantBuffer(CBChangesOnResize* cbResize, GLuint bindIndex)
{
	glGenBuffers(1, &m_UBOid[2]);
	glBindBuffer(GL_UNIFORM_BUFFER, m_UBOid[2]);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(CBChangesOnResize), cbResize, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	glBindBufferRange(GL_UNIFORM_BUFFER, bindIndex, m_UBOid[2], 0, sizeof(CBChangesOnResize));
}

void Effect::BindConstantBuffer(CBChangesRarely* cbRare, GLuint bindIndex)
{
	glGenBuffers(1, &m_UBOid[3]);
	glBindBuffer(GL_UNIFORM_BUFFER, m_UBOid[3]);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(CBChangesRarely), cbRare, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	glBindBufferRange(GL_UNIFORM_BUFFER, bindIndex, m_UBOid[3], 0, sizeof(CBChangesRarely));
}

void Effect::BindConstantBuffer(CBRenderState* cbState, GLuint bindIndex)
{
	glGenBuffers(1, &m_UBOid[4]);
	glBindBuffer(GL_UNIFORM_BUFFER, m_UBOid[4]);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(CBRenderState), cbState, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	glBindBufferRange(GL_UNIFORM_BUFFER, bindIndex, m_UBOid[4], 0, sizeof(CBRenderState));
}

void Effect::BindConstantBuffer(CBChangesEveryDrawingPBR* cbDrawPBR, GLuint bindIndex)
{
	glGenBuffers(1, &m_UBOid[5]);
	glBindBuffer(GL_UNIFORM_BUFFER, m_UBOid[5]);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(CBChangesEveryDrawingPBR), cbDrawPBR, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	glBindBufferRange(GL_UNIFORM_BUFFER, bindIndex, m_UBOid[5], 0, sizeof(CBChangesEveryDrawingPBR));
}

void Effect::ComputeTexture(std::vector<GLuint> texID, std::vector<GLenum> readWrite, std::vector<GLenum> format, GLuint numGroupX, GLuint numGroupY, GLuint numGroupZ)
{
	int size = texID.size();
	for (int i = 0; i < size; i++)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		glBindImageTexture(0 + i, texID[i], 0, GL_FALSE, 0, readWrite[i], format[i]);
	}
	glDispatchCompute(numGroupX, numGroupY, numGroupZ);
}

void Effect::ComputeTexture(GLuint texID, GLenum readWrite, GLenum format, GLuint numGroupX, GLuint numGroupY, GLuint numGroupZ)
{
	glBindImageTexture(0, texID, 0, GL_FALSE, 0, readWrite, format);
	glDispatchCompute(numGroupX, numGroupY, numGroupZ);
}