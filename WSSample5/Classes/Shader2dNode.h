/*
 * Shader node for 2-D with postion and color.
 *
 * Copyright (C) 2014 Wada Takao.
 */

#ifndef _SHADER2DNODE_H_
#define _SHADER2DNODE_H_

#include "cocos2d.h"
#include "ShaderNode.h"

USING_NS_CC;

class Shader2dNode : public ShaderNode
{
protected:
	Vec2 _center;
	Vec2 _resolution;

public:
	// Factory
	static Shader2dNode* create(const std::string &vert, const std::string &frag);

	// Override
	virtual void update(float dt);

	virtual void setPosition(const Vec2 &newPosition);

protected:
	Shader2dNode();
	virtual ~Shader2dNode();

	// Override
	virtual bool init(const std::string &vert, const std::string &frag);
	// Override
	virtual void onDraw(const Mat4 &transform, uint32_t flags);
};

#endif