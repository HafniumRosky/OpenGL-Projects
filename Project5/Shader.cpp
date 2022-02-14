#include "Shader.h"

void Shader::CompileShader()
{
	// create a shader object
	GLuint ShaderObj = glCreateShader(this->m_ShaderType);

	if (ShaderObj == 0) 
	{
		fprintf(stderr, "Error creating shader type %d\n", this->m_ShaderType);
		exit(0);
	}

	// Bind the source code to the shader, this happens before compilation
	glShaderSource(ShaderObj, 1, (const GLchar**)&this->m_pShaderText, NULL);
	// compile the shader and check for errors
	glCompileShader(ShaderObj);
	GLint success;
	// check for shader related errors using glGetShaderiv
	glGetShaderiv(ShaderObj, GL_COMPILE_STATUS, &success);
	if (!success) 
	{
		GLchar InfoLog[1024];
		glGetShaderInfoLog(ShaderObj, 1024, NULL, InfoLog);
		fprintf(stderr, "Error compiling shader type %d: '%s'\n", this->m_ShaderType, InfoLog);
		exit(1);
	}

	this->m_ShaderObject = ShaderObj;
}