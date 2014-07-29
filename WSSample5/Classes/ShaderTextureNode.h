/**
 * Shader node for 2-D with texture.
 *
 * Copyright (c) 2014 Takao WADA.
 */
#ifndef _ShaderTextureNode_H_
#define _ShaderTextureNode_H_

#include "cocos2d.h"
#include "Shader2dNode.h"

USING_NS_CC;

class ShaderTextureNode : public Shader2dNode
{
public:
	// Factory
	static ShaderTextureNode* create(const std::string &vert, const std::string &frag, Texture2D* texture);

	// Override
	virtual void update(float dt);

	// Setup a texture
	void setTexture(Texture2D* texture)
	{
		_texture0 = texture;
	}

protected:
	ShaderTextureNode();
	virtual ~ShaderTextureNode();

	// Override
	bool init(const std::string &vert, const std::string &frag, Texture2D* texture);
	// Override
	void onDraw(const Mat4 &transform, uint32_t flags);
};

#endif