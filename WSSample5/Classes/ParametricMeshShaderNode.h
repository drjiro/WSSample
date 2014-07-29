/*
 * Parameteric surface builder.
 *
 * Copyright (C) 2014 Wada Takao.
 */
#ifndef _PARAMETRICMESH_H_
#define _PARAMETRICMESH_H_

#include "cocos2d.h"
#include "ShaderNode.h"
#include "VertexPositionNormalTextureTangent.h"

USING_NS_CC;

/**
 * This class creates the geometrical meshes.
 */
class ParametricMeshShaderNode : public ShaderNode
{
protected:
	// Vertex data array
	VertexPositionNormalTextureTangent* _vertices;

	// Index data array
	GLushort* _indices;

	// Minimum U value of a texture
	float _minU;

	// Maximum U value of a texture
	float _maxU;

	// Minimum V value of a texture
	float _minV;

	// Maximum V value of a texture
	float _maxV;

	// Sampling U parameter
	int _sampleU;

	// Sampling V parameter
	int _sampleV;

	// Scaling ratio
	float _scale;

public:
	static ParametricMeshShaderNode* create(const std::string &vert, const std::string &frag,
		int sampleU, int sampleV);

	int getNumFaces();

protected:
	ParametricMeshShaderNode(int sampleU, int sampleV);
	virtual ~ParametricMeshShaderNode();

	virtual bool init(const std::string &vert, const std::string &frag);

	// Override
	virtual void onDraw(const Mat4 &transform, uint32_t flags);

	// Compute the vertices and normals.
	void computeVertexAndNormals(float dMinU, float dMaxU, float dMinV, float dMaxV);
	// Generate indices.
	void generateIndices();

	// Compute domain
	Vec2 computeDomain(float x, float y) const
	{
		return Vec2(x * (_maxU - _minU) / (_sampleU - 1) + _minU, y * (_maxV - _minV) / (_sampleV - 1) + _minV);
	}

	// Pure virtual function to generate 3-D from 2-D.
	virtual Vec3 evaluate(const Vec2& domain) const = 0;

	// Diside invert normal vector or not.
	virtual bool invertNormal(const Vec2& domain) const
	{
		return false;
	}
};

#endif