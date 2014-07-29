/*
 * Parametric sphere using the parameteric surface.
 *
 * Copyright (C) 2014 Wada Takao.
 */
#ifndef _ParametricSphereShaderNode_H_
#define _ParametricSphereShaderNode_H_

#include "ParametricMeshShaderNode.h"

class ParametricSphereShaderNode : public ParametricMeshShaderNode
{
public:
	static ParametricSphereShaderNode* create(const std::string &vert, const std::string& frag);

	virtual void update(float dt);

protected:
	ParametricSphereShaderNode();
	~ParametricSphereShaderNode();

	Vec3 evaluate(const Vec2& domain) const;
};

#endif