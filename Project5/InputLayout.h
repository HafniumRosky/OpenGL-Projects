#ifndef INPUT_LAYOUT_H
#define INPUT_LAYOUT_H

#include <GL/glew.h>
#include <glm.hpp>
#include <vector>
#include <map>
#include <string>

using namespace glm;

//This struct defines a vertex input element
typedef struct INPUT_VERTEX_DESC
{
	const GLchar*	name;				//This one matches the variable name in vertex shader
	GLuint			elementNum;			//The number of components in this property
	GLenum			type;				//The data type of components
	GLuint			size;				//This one is size of the element(BYTE)
	GLuint			offset;				//This one defines how many bytes should skip in each data row
	
}INPUT_VERTEX_DESC;


namespace InputLayout
{
	//VertexData contains all posible data properties (This is for mapping the data into matching vertexType)
	struct VertexData
	{
		vec3 pos;
		vec3 normal;
		vec4 tangent;
		vec4 color;
		vec2 tex;
	};

	//VertexPos only contains position information for each vertex
	struct VertexPos
	{
		VertexPos() = default;

		VertexPos(const VertexPos&) = default;
		VertexPos& operator=(const VertexPos&) = default;

		VertexPos(VertexPos&&) = default;
		VertexPos& operator=(VertexPos&&) = default;

		std::vector<vec3> pos;

		static const INPUT_VERTEX_DESC inputLayout[1];

	};

	//VertexPosColor contains position and color information for each vertex
	struct VertexPosColor
	{
		VertexPosColor() = default;

		VertexPosColor(const VertexPosColor&) = default;
		VertexPosColor& operator=(const VertexPosColor&) = default;

		VertexPosColor(VertexPosColor&&) = default;
		VertexPosColor& operator=(VertexPosColor&&) = default;

		std::vector<vec3> pos;
		std::vector<vec4> color;
		static const INPUT_VERTEX_DESC inputLayout[2];
	};

	//VertexPosTex contains position and texture coordinate information for each vertex
	struct VertexPosTex
	{
		VertexPosTex() = default;

		VertexPosTex(const VertexPosTex&) = default;
		VertexPosTex& operator=(const VertexPosTex&) = default;

		VertexPosTex(VertexPosTex&&) = default;
		VertexPosTex& operator=(VertexPosTex&&) = default;

		std::vector<vec3> pos;
		std::vector<vec2> tex;
		static const INPUT_VERTEX_DESC inputLayout[2];
	};

	//VertexPosNormalColor contains position, normal and color information for each vertex
	struct VertexPosNormalColor
	{
		VertexPosNormalColor() = default;

		VertexPosNormalColor(const VertexPosNormalColor&) = default;
		VertexPosNormalColor& operator=(const VertexPosNormalColor&) = default;

		VertexPosNormalColor(VertexPosNormalColor&&) = default;
		VertexPosNormalColor& operator=(VertexPosNormalColor&&) = default;

		std::vector<vec3> pos;
		std::vector<vec3> normal;
		std::vector<vec4> color;
		static const INPUT_VERTEX_DESC inputLayout[3];
	};

	//VertexPosNormalColor contains position, normal and texture coordinate information for each vertex
	struct VertexPosNormalTex
	{
		VertexPosNormalTex() = default;

		VertexPosNormalTex(const VertexPosNormalTex&) = default;
		VertexPosNormalTex& operator=(const VertexPosNormalTex&) = default;

		VertexPosNormalTex(VertexPosNormalTex&&) = default;
		VertexPosNormalTex& operator=(VertexPosNormalTex&&) = default;

		std::vector<vec3> pos;
		std::vector<vec3> normal;
		std::vector<vec2> tex;
		static const INPUT_VERTEX_DESC inputLayout[3];
	};

	//VertexPosNormalColor contains position, normal, tangent and texture coordinate information for each vertex
	struct VertexPosNormalTangentTex
	{
		VertexPosNormalTangentTex() = default;

		VertexPosNormalTangentTex(const VertexPosNormalTangentTex&) = default;
		VertexPosNormalTangentTex& operator=(const VertexPosNormalTangentTex&) = default;

		VertexPosNormalTangentTex(VertexPosNormalTangentTex&&) = default;
		VertexPosNormalTangentTex& operator=(VertexPosNormalTangentTex&&) = default;

		std::vector<vec3> pos;
		std::vector<vec3> normal;
		std::vector<vec4> tangent;
		std::vector<vec2> tex;
		static const INPUT_VERTEX_DESC inputLayout[4];
	};
}

#endif