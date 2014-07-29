/*
 * Parametric sphere using the parameteric surface.
 *
 * Copyright (C) 2014 Wada Takao.
 */
#include "ParametricSphereShaderNode.h"

/**
* Create a node.
*
* @param vert Vertex shader file name.
* @param frag Fragment shader file name.
* @return Created node
*/
ParametricSphereShaderNode* ParametricSphereShaderNode::create(const std::string &vert, const std::string& frag)
{
	auto node = new ParametricSphereShaderNode();
	node->init(vert, frag);
	node->autorelease();

	return node;
}

/**
* Build a parametric surface.
*/
ParametricSphereShaderNode::ParametricSphereShaderNode()
: ParametricMeshShaderNode(50, 50)
{
	_scale = 1.0f;

	// Compute the number of indices
	_numIndices = (_sampleU - 1) * (_sampleV - 1) * 2 * 3;

	// Generate and fill in the index array.
	generateIndices();

	// Compute vertices and normal vectors
	computeVertexAndNormals(0, M_PI, 0, 2 * M_PI);

	for (int i = 0; i < _numVertices; i++) {
		log("[%d] Pos=(%.2f %.2f %.2f) N=(%.2f %.2f %.2f) uv=(%.2f %.2f)\n", i,
			_vertices[i].position[0], _vertices[i].position[1], _vertices[i].position[2],
			_vertices[i].normal[0], _vertices[i].normal[1], _vertices[i].normal[2],
			_vertices[i].texCoord[0], _vertices[i].texCoord[1]);
	}
}

ParametricSphereShaderNode::~ParametricSphereShaderNode()
{
}

/**
* Evaluate from 2-D to 3-D
*
* @param domain Domain
*/
Vec3 ParametricSphereShaderNode::evaluate(const Vec2& domain) const
{
	float u = domain.x, v = domain.y;
	float x = _scale * -sin(u) * sin(v);
	float y = _scale * sin(u) * cos(v);
	float z = _scale * cos(u);
	return Vec3(x, y, z);
}

/**
* Update per frames.
*
* @param dt Elapsed time
*/
void ParametricSphereShaderNode::update(float dt)
{
	_time += dt;

	_rotation3d.y += M_PI / 180;
}
