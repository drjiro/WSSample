/**
 * Directional light
 * Copyright(c) 2014 Takao WADA.
 */
#ifndef __LIGHT_H__
#define __LIGHT_H__

#include "cocos2d.h"

USING_NS_CC; 

class Light
{
	// Directin of the light
	Vec3 _direction;

public:
	Light();
	~Light();

	void setDirection(float x, float y, float z)
	{
		_direction.x = x;
		_direction.y = y;
		_direction.z = z;
	}

	Vec3& getDirection()
	{
		return _direction;
	}
};

#endif
