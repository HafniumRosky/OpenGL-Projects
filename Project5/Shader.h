#ifndef SHADER_H
#define SHADER_H

#include <GL/glew.h>
#include <fstream>
#include <string>
#include <iostream>

//This Class is for reading the shader program from a GLSL file
class Shader
{
private:
	GLuint			m_ShaderObject;					//Identification for shaders
	GLenum			m_ShaderType;					//VS,FS, ...(GL_..._SHADER)
	const char*		m_pShaderText;					//Contents of the shader

	//Compile the current shader
	void CompileShader();

public:
	Shader() = default;
	//A constructor which can set all the properties of the shader
	Shader(GLenum shaderType, const char* pFilename)
	{
		using namespace std;
		//Read GLSL file
		ifstream fileStream(pFilename, ios::in);
		if (!fileStream.is_open())
		{ 
			cout << "Error opening GLSL file"; 
			exit(1); 
		}
		string content;
		string line;
		while (!fileStream.eof()) 
		{
			getline(fileStream, line);
			content.append(line + "\n");
		}
		fileStream.close();

		this->m_ShaderType = shaderType;
		this->m_pShaderText = content.c_str();
		CompileShader();
	}

	//Accessors
	GLenum ShaderType()
	{
		return this->m_ShaderType;
	}

	const char* ShaderText()
	{
		return this->m_pShaderText;
	}

	GLuint ShaderObject()
	{
		return this->m_ShaderObject;
	}

};



#endif
