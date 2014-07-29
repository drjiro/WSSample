/*
* Parametric surface.
*
* Copyright (C) 2014 Wada Takao.
*/
#include "NewParametricShaderNode.h"

/**
* Create a node.
*
* @param vert Vertex shader file name.
* @param frag Fragment shader file name.
* @return Created node
*/
NewParametricShaderNode* NewParametricShaderNode::create(const std::string &vert, const std::string& frag)
{
	auto node = new NewParametricShaderNode();
	node->init(vert, frag);
	node->autorelease();

	return node;
}

/**
* Build a parametric surface.
*/
NewParametricShaderNode::NewParametricShaderNode()
: ParametricMeshShaderNode(50, 50)
{
	_scale = 1.0f;

	// Compute vertices and normal vectors
//	computeVertexAndNormals(-M_PI/2, M_PI/2, -M_PI/2, M_PI/2);
//	computeVertexAndNormals(0, M_PI, 0, M_PI * 2);
//	computeVertexAndNormals(0, 2 * M_PI, 0, 1);
	computeVertexAndNormals(0, 2 * M_PI, 0, 2 * M_PI);

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

NewParametricShaderNode::~NewParametricShaderNode()
{
}

/**
* Evaluate from 2-D to 3-D
*
* Ex. X-Y plane
*
* float x = u;
* float y = v;
* float z = 0;
*
* @param domain Domain
*/
Vec3 NewParametricShaderNode::evaluate(const Vec2& domain) const
{
	float u = domain.x, v = domain.y;

	const float major = 1;
	const float minor = 0.2f;
	float x = (major + minor * cos(v)) * cos(u);
	float y = (major + minor * cos(v)) * sin(u);
	float z = minor * sin(v);
	return Vec3(x, y, z);
}

/**
* Update per frames.
*
* @param dt Elapsed time
*/
void NewParametricShaderNode::update(float dt)
{
	_time += dt;

	_rotation3d.y += 0.1f * M_PI / 180;

	// Pass the current time (float value)
	getGLProgramState()->setUniformFloat("u_time", _time);
}

/**
* Draws the model
*
* @param transform Transform matrix
* @param flags Flags to draw
*/
void NewParametricShaderNode::onDraw(const Mat4 &transform, uint32_t flags)
{
	setCommon3dEnvironment();

	auto glProgramState = getGLProgramState();
	// Pass the position parameter to the vertex shader's attribute variable
	glProgramState->setVertexAttribPointer("a_position",
		3, GL_FLOAT, GL_FALSE, 44, _vertices[0].position);
	glProgramState->setVertexAttribPointer("a_normal",
		3, GL_FLOAT, GL_FALSE, 44, _vertices[0].normal);
	glProgramState->setVertexAttribPointer("a_texCoord",
		2, GL_FLOAT, GL_FALSE, 44, _vertices[0].texCoord);
	glProgramState->setVertexAttribPointer("a_tangent",
		3, GL_FLOAT, GL_FALSE, 44, _vertices[0].tangent);
	glProgramState->apply(transform);

	glEnable(GL_DEPTH_TEST);
	glCullFace(GL_BACK);
	//	glCullFace(GL_FRONT);
	glEnable(GL_CULL_FACE);

	// Alpha blenfing
//	glEnable(GL_BLEND);
//	glDisable(GL_BLEND);
//	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//	glBlendFunc(GL_ONE, GL_ONE);

	// Draw triangles using indices
	glDrawElements(GL_TRIANGLES, _numIndices, GL_UNSIGNED_SHORT, _indices);

//	glDisable(GL_BLEND);
	//	CC_INCREMENT_GL_DRAWN_BATCHES_AND_VERTICES(1, 3);
}
