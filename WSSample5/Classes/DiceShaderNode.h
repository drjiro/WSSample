/**
 * Node for a dice.
 *
 * Copyright (c) 2014 Takao WADA.
 */
#ifndef _DiceShaderNode_H_
#define _DiceShaderNode_H_

#include "cocos2d.h"
#include "ShaderNode.h"

USING_NS_CC;

#include "Camera3d.h"

class DiceShaderNode : public ShaderNode
{
public:
	static DiceShaderNode* create(const std::string &vert, const std::string &frag, Texture2D* texture);

	virtual void update(float dt);

protected:
	DiceShaderNode();
	virtual ~DiceShaderNode();

	virtual bool init(const std::string &vert, const std::string &frag);

	// Override
	void onDraw(const Mat4 &transform, uint32_t flags);
};

#endif