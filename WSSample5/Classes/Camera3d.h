/**
 * 3 - D Camera
 * Copyright(c) 2014 Takao WADA.
 */ 
#ifndef __CAMERA_H__
#define __CAMERA_H__

#include "cocos2d.h"

USING_NS_CC;

class Camera3d
{
	Vec3 _eye;			// Eye position
	Vec3 _lookat;		// Lookat position
	Vec3 _up;			// Up vector
	Mat4 _viewMatrix;

	float _fov;			// FOVy
	int _screenWidth;	// Screen width
	int _screenHeight;	// Screen height
	float _nearClip;		// Distance to near clipping plane
	float _farClip;		// Distance to far clipping plane
	Mat4 _projectionMatrix;

public:
	Camera3d(int _width, int _height);
	virtual ~Camera3d();

	Mat4& getViewMatrix() {
		return _viewMatrix;
	}

	Mat4& getProjectionMatrix() {
		return _projectionMatrix;
	}

	// Get the eye position
	Vec3& getEye()
	{
		return _eye;
	}

	// Set the eye postion
	void setEye(float x, float y, float z)
	{
		_eye.x = x;
		_eye.y = y;
		_eye.z = z;
		setViewMatrix();
	}

	// Set lookat (target) postion
	void setLookat(float x, float y, float z)
	{
		_lookat.x = x;
		_lookat.y = y;
		_lookat.z = z;
		setViewMatrix();
	}

	// Set FOV in degree
	void setFov(float fov)
	{
		_fov = fov;
		setPeojectionMatrix();
	}

protected:
	void setViewMatrix()
	{
		Mat4::createLookAt(_eye, _lookat, _up, &_viewMatrix);
	}
	void setPeojectionMatrix()
	{
		Mat4::createPerspective(_fov, (GLfloat)_screenWidth / _screenHeight, _nearClip, _farClip, &_projectionMatrix);
	}
};

#endif
