#ifndef _NewShaderNode_H_
#define _NewShaderNode_H_

#include "cocos2d.h"

USING_NS_CC;

class NewShaderNode : public Node
{
public:
	static NewShaderNode* create(const std::string &vert, const std::string &frag, Texture2D* texture);

	virtual void update(float dt);
	virtual void setPosition(const Vec2 &newPosition);
	virtual void draw(Renderer *renderer, const Mat4 &transform, uint32_t flags) override;

protected:
	NewShaderNode();
	~NewShaderNode();

	bool init(const std::string &vert, const std::string &frag, Texture2D* texture);
	void loadShaders(const std::string &vert, const std::string &frag);

	void onDraw(const Mat4 &transform, uint32_t flags);

	Vec2 _center;
	Vec2 _resolution;
	float      _time;
	std::string _vertFileName;
	std::string _fragFileName;
	CustomCommand _customCommand;
};

#endif