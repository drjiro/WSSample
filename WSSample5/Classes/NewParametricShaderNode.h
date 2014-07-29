/*
* Parametric surface.
*
* Copyright (C) 2014 Wada Takao.
*/
#ifndef _NewParametricShaderNode_H_
#define _NewParametricShaderNode_H_

#include "ParametricMeshShaderNode.h"

class NewParametricShaderNode : public ParametricMeshShaderNode
{
public:
	static NewParametricShaderNode* create(const std::string &vert, const std::string& frag);

	virtual void update(float dt);

protected:
	NewParametricShaderNode();
	~NewParametricShaderNode();

	Vec3 evaluate(const Vec2& domain) const;

	void onDraw(const Mat4 &transform, uint32_t flags);
};

#endif