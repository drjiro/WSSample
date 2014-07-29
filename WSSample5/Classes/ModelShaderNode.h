#pragma once
#include "cocos2d.h"

USING_NS_CC;

#include "Camera3d.h"

class ModelShaderNode : public Node
{
public:
	static ModelShaderNode* shaderNodeWithFile(const std::string &vert, const std::string &frag,
		const std::string& objFilePath, const std::string &textureFilePath);

	virtual void update(float dt);
	virtual void setPosition(const Vec3 &newPosition);
	virtual void draw(Renderer *renderer, const Mat4 &transform, uint32_t flags) override;

	void setCamera(Camera3d* camera);


protected:
	ModelShaderNode();
	~ModelShaderNode();

	bool createFromObjFileAndTexture(const std::string &vert, const std::string &frag,
		const std::string& objFilePath, const std::string &textureFilePath);
	void loadShaderFile(const std::string &vert, const std::string &frag);

	void onDraw(const Mat4 &transform, uint32_t flags);

	float _time;
	float _rotY;


	Sprite3D* _sprite3d;

	Vec3 _position3d;

	Camera3d *camera;

	std::string _vertFileName;
	std::string _fragFileName;
	CustomCommand _customCommand;
};

