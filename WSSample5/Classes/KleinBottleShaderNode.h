/*
* Sphere surface builder.
*
* Copyright (C) 2014 Wada Takao.
*/
#ifndef _KleinBottleShaderNode_H_
#define _KleinBottleShaderNode_H_

#include "ParametricMeshShaderNode.h"

class KleinBottleShaderNode : public ParametricMeshShaderNode
{
public:
	static KleinBottleShaderNode* create(const std::string &vert, const std::string& frag);

	virtual void update(float dt);

protected:
	KleinBottleShaderNode();
	~KleinBottleShaderNode();

	Vec3 evaluate(const Vec2& domain) const;

	bool invertNormal(const Vec2& domain) const;
};

#endif