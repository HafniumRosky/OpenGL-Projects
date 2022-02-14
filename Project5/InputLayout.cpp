#include "InputLayout.h"

const INPUT_VERTEX_DESC InputLayout::VertexPos::inputLayout[1] =
{
	{"posL",	3,	GL_FLOAT,	3 * sizeof(GLfloat),	0}
};

const INPUT_VERTEX_DESC InputLayout::VertexPosColor::inputLayout[2] =
{
	{"posL",	3,	GL_FLOAT,	3 * sizeof(GLfloat),	0},
	{"color",	4,	GL_FLOAT,	4 * sizeof(GLfloat),	3 * sizeof(GLfloat)}
};

const INPUT_VERTEX_DESC InputLayout::VertexPosTex::inputLayout[2] =
{
	{"posL",	3,	GL_FLOAT,	3 * sizeof(GLfloat),	0},
	{"texcoord",2,	GL_FLOAT,	2 * sizeof(GLfloat),	3 * sizeof(GLfloat)}
};

const INPUT_VERTEX_DESC InputLayout::VertexPosNormalColor::inputLayout[3] =
{
	{"posL",	3,	GL_FLOAT,	3 * sizeof(GLfloat),	0},
	{"normalL", 3,	GL_FLOAT,	3 * sizeof(GLfloat),	3 * sizeof(GLfloat)},
	{"color",	4,	GL_FLOAT,	4 * sizeof(GLfloat),	6 * sizeof(GLfloat)}
};

const INPUT_VERTEX_DESC InputLayout::VertexPosNormalTex::inputLayout[3] =
{
	{"posL",	3,	GL_FLOAT,	3 * sizeof(GLfloat),	0},
	{"normalL", 3,	GL_FLOAT,	3 * sizeof(GLfloat),	3 * sizeof(GLfloat)},
	{"texcoord",2,	GL_FLOAT,	2 * sizeof(GLfloat),	6 * sizeof(GLfloat)}
};

const INPUT_VERTEX_DESC InputLayout::VertexPosNormalTangentTex::inputLayout[4] =
{
	{"posL",	3,	GL_FLOAT,	3 * sizeof(GLfloat),	0},
	{"normalL", 3,	GL_FLOAT,	3 * sizeof(GLfloat),	3 * sizeof(GLfloat)},
	{"tangent", 4,	GL_FLOAT,	4 * sizeof(GLfloat),	6 * sizeof(GLfloat)},
	{"texcoord",2,	GL_FLOAT,	2 * sizeof(GLfloat),	10 * sizeof(GLfloat)}
};