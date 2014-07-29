/*
 * Klein bottle using the parameteric surface.
 *
 * Copyright (C) 2014 Wada Takao.
 */
#include "KleinBottleShaderNode.h"

/**
 * Create a node.
 *
 * @param vert Vertex shader file name.
 * @param frag Fragment shader file name.
 * @return Created node
 */
KleinBottleShaderNode* KleinBottleShaderNode::create(const std::string &vert, const std::string& frag)
{
	auto node = new KleinBottleShaderNode();
	node->init(vert, frag);
	node->autorelease();

	return node;
}

/**
 * Build a parametric surface.
 */
KleinBottleShaderNode::KleinBottleShaderNode()
: ParametricMeshShaderNode(20, 30)
{
	_scale = 0.1f;

	// Compute vertices and normal vectors
	computeVertexAndNormals(0, (float)(2 * M_PI), 0, (float)(2 * M_PI));

	for (int i = 0; i < _numVertices; i++) {
		log("[%d] Pos=(%.2f %.2f %.2f) N=(%.2f %.2f %.2f) uv=(%.2f %.2f)\n", i,
			_vertices[i].position[0], _vertices[i].position[1], _vertices[i].position[2],
			_vertices[i].normal[0], _vertices[i].normal[1], _vertices[i].normal[2],
			_vertices[i].texCoord[0], _vertices[i].texCoord[1]);
	}

	// Compute the number of indices
	_numIndices = (_sampleU - 1) * (_sampleV - 1) * 2 * 3;

	// Generate and fill in the index array.
	generateIndices();
}

KleinBottleShaderNode::~KleinBottleShaderNode()
{
}

/**
* Evaluate from 2-D to 3-D
*
* @param domain Domain
* @return 3-D position
*/
Vec3 KleinBottleShaderNode::evaluate(const Vec2& domain) const
{
	/*
	float v = 1 - domain.x;
	float u = domain.y;

	float x0 = 3 * cos(u) * (1 + sin(u)) +
				(2 * (1 - cos(u) / 2)) * cos(u) * cos(v);
	float y0 = 8 * sin(u) + (2 * (1 - cos(u) / 2)) * sin(u) * cos(v);
	float x1 = 3 * cos(u) * (1 + sin(u)) +
				(2 * (1 - cos(u) / 2)) * cos(v + M_PI);

	float y1 = 8 * sin(u);
	Vec3 range;
	range.x = u < M_PI ? x0 : x1;
	range.y = u < M_PI ? -y0 : -y1;
	range.z = (-2 * (1 - cos(u) / 2)) * sin(v);
	*/

	float v = 1 - domain.x;
	float u = domain.y;
	Vec3 range;
	float botx = (6 - 2)  * (float)cos(u) * (1 + (float)sin(u));
	float boty = (16 - 4) * (float)sin(u);
	float rad = (4 - 1)  * (1 - (float)cos(u) / 2);

	if (u > 1.7 * M_PI)
	{
		range.x = botx + rad * (float)cos(u) * (float)cos(v);
		range.y = boty + rad * (float)sin(u) * (float)cos(v);
	}
	else if (u > M_PI)
	{
		range.x = botx + rad * (float)cos(v + M_PI);
		range.y = boty;
	}
	else
	{
		range.x = botx + rad * (float)cos(u) * (float)cos(v);
		range.y = boty + rad * (float)sin(u) * (float)cos(v);
	}

	range.z = rad * (float)-sin(v);
	//	range.y -= 2;
	range.y = -range.y;

	return range * _scale;
}

/**
 * Diside inverting a normal vector or not
 *
 * @param domain Domain
 * @return if invert, return true
 */
bool KleinBottleShaderNode::invertNormal(const Vec2& domain) const
{
	return false;
//	return domain.y > 3 * M_PI / 2;
}

/**
* Update per frames.
*
* @param dt Elapsed time
*/
void KleinBottleShaderNode::update(float dt)
{
	_time += dt;

	_rotation3d.y += M_PI / 180;
}
