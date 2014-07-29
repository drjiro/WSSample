/*
* Torus using the parameteric surface.
*
* Copyright (C) 2014 Wada Takao.
*/
/*
* Parametric surface.
*
* Copyright (C) 2014 Wada Takao.
*/
#ifndef _TorusShaderNode_H_
#define _TorusShaderNode_H_

#include "ParametricMeshShaderNode.h"

class TorusShaderNode : public ParametricMeshShaderNode
{
public:
	static TorusShaderNode* create(const std::string &vert, const std::string& frag);

	virtual void update(float dt);

protected:
	TorusShaderNode();
	~TorusShaderNode();

	Vec3 evaluate(const Vec2& domain) const;
};

#endif