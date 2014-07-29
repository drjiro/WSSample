/*
 * Sphere surface builder.
 *
 * Copyright (C) 2014 Wada Takao.
 */
#ifndef _SphereShaderNode_H_
#define _SphereShaderNode_H_

#include "cocos2d.h"
#include "ShaderNode.h"
#include "Camera3d.h"
#include "VertexPositionNormalColorTexture.h"

USING_NS_CC;

/**
 * This class creates the sphere meshes.
 */
class SphereShaderNode : public ShaderNode
{
protected:
	// Vertex data array
	VertexPositionNormalColorTexture* _vertices;

	// Index data array
	GLushort* _indices;

public:
	static SphereShaderNode* create(const std::string &vert, const std::string &frag, Texture2D* texture);

	virtual void update(float dt);

protected:
	SphereShaderNode();
	virtual ~SphereShaderNode();

	// Override
	virtual bool init(const std::string &vert, const std::string &frag);

	// Override
	void onDraw(const Mat4 &transform, uint32_t flags);

	// Add this line!!!!!!!
	void setCommon3dEnvironment();
};

#endif