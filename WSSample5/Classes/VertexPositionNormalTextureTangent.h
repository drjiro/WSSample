#ifndef _VertexPositionNormalTextureTangent_H_
#define _VertexPositionNormalTextureTangent_H_

#include "cocos2d.h"

USING_NS_CC;

/**
*	Vertex format
*/
struct VertexPositionNormalTextureTangent
{
	GLfloat position[3];
	GLfloat normal[3];
	GLfloat texCoord[2];
	GLfloat tangent[3];

	VertexPositionNormalTextureTangent() {}
	VertexPositionNormalTextureTangent(float x, float y, float z,
		float nx, float ny, float nz,
		float u, float v,
		float tx, float ty, float tz)
	{
		position[0] = x;
		position[1] = y;
		position[2] = z;
		normal[0] = nx;
		normal[1] = ny;
		normal[2] = nz;
		texCoord[0] = u;
		texCoord[1] = v;
		tangent[0] = tx;
		tangent[1] = ty;
		tangent[2] = tz;
	}
};

#endif