/*
 * Shader node.
 *
 * Copyright (C) 2014 Wada Takao.
 */

#ifndef _SHADERNODE_H_
#define _SHADERNODE_H_

#include "cocos2d.h"
#include "Camera3d.h"
#include "Light.h"

USING_NS_CC;

class ShaderNode : public Node
{
protected:
	// Elapsed time;
	float _time;

	// Number of vertices
	int _numVertices;
	// Number of Indices
	int _numIndices;

	//	int _numFaces;

	// Texture
	Texture2D* _texture0;

	// Texture for normal mapping
	Texture2D* _texture1;

	// Vertex shader name
	std::string _vertFileName;
	// Fragment shader name
	std::string _fragFileName;

	// Camera
	Camera3d *_camera;

	// Light
	Light *_light;

	// Position
	Vec3 _position3d;

	// Rotation
	Vec3 _rotation3d;

	// Scaling
	Vec3 _scale3d;

	// Draw command for Cocos2d-x
	CustomCommand _customCommand;

public:
	// Update
	virtual void update(float dt);
	// Draw
	virtual void draw(Renderer *renderer, const Mat4 &transform, uint32_t flags) override;

	// Set position
	// 3D
	void setPosition(float x, float y, float z) {
		_position3d.x = x;
		_position3d.y = y;
		_position3d.z = z;
	}
	// Set rotation
	void setRotation(float x, float y, float z) {
		_rotation3d.x = x;
		_rotation3d.y = y;
		_rotation3d.z = z;
	}
	// Set scaling
	void setScale(float x, float y, float z) {
		_scale3d.x = x;
		_scale3d.y = y;
		_scale3d.z = z;
	}

	// Setup a camera
	void setCamera(Camera3d* camera)
	{
		_camera = camera;
	}
	// Setup a light
	void setLight(Light* light)
	{
		_light = light;
	}
	// Setup a texture
	void setTexture0(Texture2D* texture)
	{
		_texture0 = texture;
	}
	// Setup a second texture, such as normal map, etc.
	void setTexture1(Texture2D* texture)
	{
		_texture1 = texture;
	}

protected:
	ShaderNode();
	virtual ~ShaderNode();

	// Draw callback function
	virtual void onDraw(const Mat4 &transform, uint32_t flags) = 0;

	// Common shader loader
	void loadShaders(const std::string &vert, const std::string &frag);

	// Common setup 3-D environment
	virtual void setCommon3dEnvironment();
};

#endif