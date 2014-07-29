#ifndef _VertexPositionNormalColorTexture_H_
#define _VertexPositionNormalColorTexture_H_

#include "cocos2d.h"

USING_NS_CC;

/**
*	Vertex format
*/
struct VertexPositionNormalColorTexture
{
	GLfloat position[3];
	GLfloat normal[3];
	GLfloat color[4];
	GLfloat texCoord[2];

	VertexPositionNormalColorTexture() {}
	VertexPositionNormalColorTexture(float x, float y, float z,
		float nx, float ny, float nz,
		float u, float v)
	{
		position[0] = x;
		position[1] = y;
		position[2] = z;
		normal[0] = nx;
		normal[1] = ny;
		normal[2] = nz;
		color[0] = 1;
		color[1] = 1;
		color[2] = 1;
		color[3] = 1;
		texCoord[0] = u;
		texCoord[1] = v;
	}
};

#endif