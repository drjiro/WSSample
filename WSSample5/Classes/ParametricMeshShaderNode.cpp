/*
* Parameteric surface builder.
*
* Refs.
* - Power VR Graphics SDK example "Shaders".
* - "iPhone 3D Programming Developing Graphical Applications with OpenGL ES" by
* Philip Rideout
*
* Copyright (C) 2014 Wada Takao.
*/
#include "ParametricMeshShaderNode.h"

/**
* Build a parametric surface.
*
* @param sampleU Sampling U parameter
* @param sampleV Sampling V parameter
*/
ParametricMeshShaderNode::ParametricMeshShaderNode(int sampleU, int sampleV)
: _vertices(nullptr),
_indices(nullptr),
_minU(0),
_maxU(0),
_minV(0),
_maxV(0),
_sampleU(sampleU),
_sampleV(sampleV),
_scale(1)
{
}

/**
*	Initialize shaders from files.
*
*	@param vert Vertex shader file name.
*	@param frag Fragment shader file name.
*/
bool ParametricMeshShaderNode::init(const std::string &vert, const std::string &frag)
{
	_vertFileName = vert;
	_fragFileName = frag;

	// Load and compile shader programs.
	loadShaders(vert, frag);

	// Pass the texture
	if (_texture0 != nullptr)
	{
		getGLProgramState()->setUniformTexture("u_texture0", _texture0);
	}

	scheduleUpdate();

	return true;
}

/**
* Delete buffers.
*/
ParametricMeshShaderNode::~ParametricMeshShaderNode()
{
	if (_vertices != nullptr)
	{
		delete[] _vertices;
		_vertices = nullptr;
	}

	if (_indices != nullptr)
	{
		delete[] _indices;
		_indices = nullptr;
	}
}

/**
* Compute the vertices and normals.
*
* @param minU minimum U parameter
* @param maxU maximum U parameter
* @param minV minimum V parameter
* @param maxV maximum V parameter
*/
void ParametricMeshShaderNode::computeVertexAndNormals(float minU, float maxU, float minV, float maxV)
{
	_numVertices = _sampleU * _sampleV;
	_vertices = new VertexPositionNormalTextureTangent[_numVertices];

	this->_minU = minU;
	this->_maxU = maxU;
	this->_minV = minV;
	this->_maxV = maxV;

	//	this->scale = maxU - minU;

	int n = 0;
	for (int j = 0; j < _sampleV; j++)
	{
		for (int i = 0; i < _sampleU; i++)
		{
			Vec2 domain = computeDomain((float)i, (float)j);
			Vec3 range = evaluate(domain);

			// Compute positions
			{
				_vertices[n].position[0] = range.x;
				_vertices[n].position[1] = range.y;
				_vertices[n].position[2] = range.z;
			}

			// Compute texture coordinates
			{
				_vertices[n].texCoord[0] = (float)i / (_sampleU - 1);
				_vertices[n].texCoord[1] = 1 - (float)j / (_sampleV - 1);
			}

			// Compute normal vectors
			{
				float s = i;
				float t = j;
				// If the normal vector doesn't exist, move a bit.
				if (i == 0)
					s += 0.01f;
				if (i == _sampleU - 1)
					s -= 0.01f;
				if (j == 0)
					t += 0.01f;
				if (j == _sampleV - 1)
					t -= 0.01f;

				Vec3 p = evaluate(computeDomain(s, t));
				Vec3 u = evaluate(computeDomain(s + 0.01f, t)) - p;
				Vec3 v = evaluate(computeDomain(s, t + 0.01f)) - p;

				Vec3 normal;
				Vec3::cross(u, v, &normal);
				normal.normalize();
				if (invertNormal(domain))
					normal = -normal;
				_vertices[n].normal[0] = normal.x;
				_vertices[n].normal[1] = normal.y;
				_vertices[n].normal[2] = normal.z;
			}

			// Compute tangent vectors
			{
				float s = i;
				float t = j;
				Vec3 p = evaluate(computeDomain(s, t));
				Vec3 u = evaluate(computeDomain(s + 0.01f, t)) - p;
				if (invertNormal(domain))
				{
					u = -u;
				}
				_vertices[n].tangent[0] = u.x;
				_vertices[n].tangent[1] = u.y;
				_vertices[n].tangent[2] = u.z;
			}
			n++;
		}
	}
}

/**
* Generate indices.
*/
void ParametricMeshShaderNode::generateIndices()
{
	// Compute the number of indices
	_numIndices = (_sampleU - 1) * (_sampleV - 1) * 2 * 3;
	_indices = new GLushort[_numIndices];

	int vertex = 0;
	for (int j = 0; j < _sampleV - 1; j++)
	{
		for (int i = 0; i < _sampleU - 1; i++)
		{
			int next = (i + 1) % _sampleU;
			_indices[(j * (_sampleU - 1) + i) * 6 + 0] = vertex + i; // (unsigned short)(j * _sampleU + i);
			_indices[(j * (_sampleU - 1) + i) * 6 + 1] = vertex + next; // (unsigned short)(j * _sampleU + (i + 1));
			_indices[(j * (_sampleU - 1) + i) * 6 + 2] = vertex + i + _sampleU; // (unsigned short)((j + 1) * _sampleU + (i + 1));
			_indices[(j * (_sampleU - 1) + i) * 6 + 3] = vertex + next; // (unsigned short)(j * _sampleU + i);
			_indices[(j * (_sampleU - 1) + i) * 6 + 4] = vertex + next + _sampleU; // (unsigned short)((j + 1) * _sampleU + (i + 1));
			_indices[(j * (_sampleU - 1) + i) * 6 + 5] = vertex + i + _sampleU; // (unsigned short)((j + 1) * _sampleU + i);
		}
		vertex += _sampleU;
	}

	for (int i = 0; i < _numIndices; i += 3) {
		log("%2d %2d %2d\n", _indices[i], _indices[i + 1], _indices[i + 2]);
	}
}

/**
* Get the number of faces.
*
* @return Number of faces
*/
int ParametricMeshShaderNode::getNumFaces()
{
	return (_sampleU - 1) * (_sampleV - 1) * 2;
}

/**
* Draws the model
*
* @param transform Transform matrix
* @param flags Flags to draw
*/
void ParametricMeshShaderNode::onDraw(const Mat4 &transform, uint32_t flags)
{
	setCommon3dEnvironment();

	auto glProgramState = getGLProgramState();
	// Pass the position parameter to the vertex shader's attribute variable
	glProgramState->setVertexAttribPointer("a_position",
		3, GL_FLOAT, GL_FALSE, 44, _vertices[0].position);
	glProgramState->setVertexAttribPointer("a_normal",
		3, GL_FLOAT, GL_FALSE, 44, _vertices[0].normal);
	glProgramState->setVertexAttribPointer("a_texCoord",
		2, GL_FLOAT, GL_FALSE, 44, _vertices[0].texCoord);
	glProgramState->setVertexAttribPointer("a_tangent",
		3, GL_FLOAT, GL_FALSE, 44, _vertices[0].tangent);
	glProgramState->apply(transform);

	glEnable(GL_DEPTH_TEST);
	glCullFace(GL_BACK);
	//	glCullFace(GL_FRONT);
	glEnable(GL_CULL_FACE);
	//	glDisable(GL_CULL_FACE);

	// Draw triangles using indices
	glDrawElements(GL_TRIANGLES, _numIndices, GL_UNSIGNED_SHORT, _indices);

	//	CC_INCREMENT_GL_DRAWN_BATCHES_AND_VERTICES(1, 3);
}
