#ifndef __SHADER_SCENE_H__
#define __SHADER_SCENE_H__

#include "cocos2d.h"
#include "Camera3d.h"
#include "Light.h"

class ShaderScene : public cocos2d::Layer
{
	// Camera
	Camera3d* _camera;

	// Light
	Light* _light;

public:
	ShaderScene();
	~ShaderScene();

	// there's no 'id' in cpp, so we recommend returning the class instance pointer
	static cocos2d::Scene* createScene();

	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init();

	// a selector callback
	void menuCloseCallback(cocos2d::Ref* pSender);

	// implement the "static create()" method manually
	CREATE_FUNC(ShaderScene);
};

#endif // __SHADER_SCENE_H__
