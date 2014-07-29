/**
 * 3-D Camera
 *
 * Copyright (c) 2014 Takao WADA.
 */
#include "Camera3d.h"

/**
 * Constructor.
 *
 * @param width Screen width
 * @param height Screen height
 */
Camera3d::Camera3d(int width, int height)
{
	// Pass the model-view matrix
	//	Vec3 eye(2, 2, 2);
	_eye = Vec3(0, 0, 10);
	_lookat = Vec3(0, 0, 0);
	_up = Vec3(0, 1, 0);
	Mat4::createLookAt(_eye, _lookat, _up, &_viewMatrix);

	// Pass the projection matrix
	_fov = 60;
	_screenWidth = width;
	_screenHeight = height;
	_nearClip = 0.1f;
	_farClip = 100.0f;


	Mat4::createPerspective(_fov, (GLfloat)_screenWidth / _screenHeight, _nearClip, _farClip, &_projectionMatrix);
}

/**
 * Destructor
 */
Camera3d::~Camera3d()
{
}
