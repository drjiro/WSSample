/**
* Node for a sphere.
*
* Copyright (c) 2014 Takao WADA.
*/
#include "SphereShaderNode.h"

/**
* Create a node.
*
* @param vert Vertex shader file name.
* @param frag Fragment shader file name.
* @param texture Texture file name.
* @return Created node
*/
SphereShaderNode* SphereShaderNode::create(const std::string &vert, const std::string& frag,
	Texture2D* texture)
{
	auto node = new SphereShaderNode();
	node->setTexture0(texture);

	node->init(vert, frag);
	node->autorelease();

	return node;
}

/**
 *	Initialize shaders from files.
 *
 *	@param vert Vertex shader file name.
 *	@param frag Fragment shader file name.
*/
bool SphereShaderNode::init(const std::string &vert, const std::string &frag)
{
#if CC_ENABLE_CACHE_TEXTURE_DATA
	auto listener = EventListenerCustom::create(EVENT_COME_TO_FOREGROUND, [this](EventCustom* event){
		this->setGLProgramState(nullptr);
		loadShaders(_vertFileName, _fragFileName);
	});

	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
#endif

	_vertFileName = vert;
	_fragFileName = frag;

	loadShaders(vert, frag);

	// Number of rings (horizontal)
	int numRings = 16;

	// Number of slices (virtical)
	int numSlices = 32;

	// radius
	float radius = 1;

	loadShaders(vert, frag);

	float rcosphi;			// r * cos(phi)
	float x, y, z;

	// Compute number of vertices
	int _numVertices = (numRings - 1) * (numSlices + 1) + 2;
	// Compute number of faces
	int _numFaces = (numRings - 2) * (numSlices * 2) + numSlices * 2;
	// Compute number of indices
	_numIndices = _numFaces * 3;

	// Allocate memory
	_vertices = new VertexPositionNormalColorTexture[_numVertices];

	// Top
	_vertices[0] = VertexPositionNormalColorTexture(
		0.0f, radius, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.5f, 0.0f);

	int n = 1;
	// Loop for vertical
	for (int i = 1; i < numRings; i++)
	{
		// Compute the angle
		float phi = M_PI / 2 - i * M_PI / numRings;

		// Compute Y coordiante
		y = radius * sinf(phi);
		// Compute cos(phi)
		rcosphi = radius * cosf(phi);

		float v = (float)i / numRings; //1.0f - (float)(i + 1) / numRings;

		// Loop for horizontal
		for (int j = 0; j <= numSlices; j++)
		{
			float theta = j * 2 * M_PI / numSlices;
			// Compute X coordinate
			x = rcosphi * cosf(theta);
			// Compute Z coordinate
			z = rcosphi * sinf(theta);
			// Compute normal
			Vec3 normal = Vec3(x, y, z);
			normal.normalize();
			float u = 1.0f - (float)j / numSlices;
			if (u < 0.0f) {
				u = 0.0f;
			}
			/*
			float u = (float)j / numSlices;
			if (u > 1.0f)
			{
				u = 1.0f;
			}
			*/

			// Set to the element
			_vertices[n++] = VertexPositionNormalColorTexture(
				x, y, z,
				normal.x, normal.y, normal.z, u, v);
		}
	}

	// Bottom
	_vertices[n] = VertexPositionNormalColorTexture(
		0.0f, -radius, 0.0f,
		0.0f, -1.0f, 0.0f,
		0.5f, 1.0f);

	for (int i = 0; i < _numVertices; i++) {
		log("[%d] Pos=(%.2f %.2f %.2f) N=(%.2f %.2f %.2f) uv=(%.2f %.2f)\n", i,
			_vertices[i].position[0], _vertices[i].position[1], _vertices[i].position[2],
			_vertices[i].normal[0], _vertices[i].normal[1], _vertices[i].normal[2],
			_vertices[i].texCoord[0], _vertices[i].texCoord[1]);
	}


	// Create index buffer
	_indices = new GLushort[_numIndices];

	// Top triangles
	n = 0;
	for (int i = 0; i < numSlices; i++)
	{
		_indices[n++] = (unsigned short)0;
		_indices[n++] = (unsigned short)(i + 2);
		_indices[n++] = (unsigned short)(i + 1);
	}

	// Middle triangles
	int m = 1;
	for (int i = 0; i < numRings - 2; i++)
	{
		for (int j = 0; j < numSlices; j++)
		{
			// First triangle
			_indices[n++] = (unsigned short)(m + j);
			_indices[n++] = (unsigned short)(m + j + 1);
			_indices[n++] = (unsigned short)(m + numSlices + 1 + j);
			// Second triangle
			_indices[n++] = (unsigned short)(m + j + 1);
			_indices[n++] = (unsigned short)(m + numSlices + j + 2);
			_indices[n++] = (unsigned short)(m + numSlices + j + 1);
		}
		m += numSlices + 1;
	}
	// Bottom triangles
	unsigned short index = _numVertices - 1 - numSlices - 1;
	for (int i = 0; i < numSlices; i++)
	{
		_indices[n++] = (unsigned short)(_numVertices - 1);
		_indices[n++] = (unsigned short)(index + i);
		_indices[n++] = (unsigned short)(index + i + 1);
	}

	for (int i = 0; i < _numIndices; i += 3) {
		log("%2d %2d %2d\n", _indices[i], _indices[i + 1], _indices[i + 2]);
	}

	// Pass the texture
	getGLProgramState()->setUniformTexture("u_texture0", _texture0);

	scheduleUpdate();

	return true;
}

/**
 * Constructor.
 */
SphereShaderNode::SphereShaderNode()
: _vertices(nullptr),
_indices(nullptr)
{
}

/**
 * Delete arrays.
 */
SphereShaderNode::~SphereShaderNode()
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
* Update the state per a frame.
*
* @param dt Elapsed time
*/
void SphereShaderNode::update(float dt)
{
	_time += dt;

	_rotation3d.y += M_PI / 180;
}

/**
 * Draw a primitive.
 *
 * @param transform Transform matrix
 * @param flags Flags to draw
 */
void SphereShaderNode::onDraw(const Mat4 &transform, uint32_t flags)
{
	setCommon3dEnvironment();

	auto glProgramState = getGLProgramState();
	// Pass the position parameter to the vertex shader's attribute variable
	glProgramState->setVertexAttribPointer("a_position",
		3, GL_FLOAT, GL_FALSE, 48, _vertices[0].position);
	glProgramState->setVertexAttribPointer("a_normal",
		3, GL_FLOAT, GL_FALSE, 48, _vertices[0].normal);
	glProgramState->setVertexAttribPointer("a_color",
		4, GL_FLOAT, GL_FALSE, 48, _vertices[0].color);
	glProgramState->setVertexAttribPointer("a_texCoord",
		2, GL_FLOAT, GL_FALSE, 48, _vertices[0].texCoord);
	glProgramState->apply(transform);

	glEnable(GL_DEPTH_TEST);
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);

	// Draw triangles using indices
	glDrawElements(GL_TRIANGLES, _numIndices, GL_UNSIGNED_SHORT, _indices);

	//	CC_INCREMENT_GL_DRAWN_BATCHES_AND_VERTICES(1, 3);
}
/**
* Draw a 3-D model.
*/
void SphereShaderNode::setCommon3dEnvironment()
{
	// Create world matrix
	Mat4 transMatrix;
	Mat4::createTranslation(_position3d.x, _position3d.y, _position3d.z, &transMatrix);
	Mat4 rotXMatrix;
	Mat4::createRotationX(_rotation3d.x, &rotXMatrix);
	Mat4 rotYMatrix;
	Mat4::createRotationY(_rotation3d.y, &rotYMatrix);
	Mat4 rotZMatrix;
	Mat4::createRotationZ(_rotation3d.z, &rotZMatrix);
	Mat4 scaleMatrix;
	Mat4::createScale(_scale3d.x, _scale3d.y, _scale3d.z, &scaleMatrix);

	Mat4 transMatrix2;
	Mat4::createTranslation(3, 0, 0, &transMatrix2);

	// Compute transformation Scaling->Roataion->Translation
	Mat4 worldMatrix;
	worldMatrix = transMatrix * rotYMatrix * rotXMatrix * rotZMatrix
		* scaleMatrix * transMatrix2;

	// Pass the world matrix
	getGLProgramState()->setUniformMat4("u_worldMatrix", worldMatrix);

	// Pass the view matrix
	getGLProgramState()->setUniformMat4("u_viewMatrix", _camera->getViewMatrix());

	// Pass the projection matrix
	getGLProgramState()->setUniformMat4("u_projectionMatrix", _camera->getProjectionMatrix());

	// Pass the inverse - transpose matrix of model-view matrix to transform normals
	Mat4 modelViewMatrixIT = _camera->getViewMatrix() * worldMatrix;
	modelViewMatrixIT.inverse();
	modelViewMatrixIT.transpose();
	getGLProgramState()->setUniformMat4("u_normalMatrix", modelViewMatrixIT);

	// Pass the direction of a light
	getGLProgramState()->setUniformVec3("u_lightDirection0", _light->getDirection());

	// Pass the eye position
	getGLProgramState()->setUniformVec3("u_eye", _camera->getEye());

	// Pass the texture
	if (_texture0 != nullptr)
	{
		getGLProgramState()->setUniformTexture("u_texture0", _texture0);
	}

	// Pass the normal map
	if (_texture1 != nullptr)
	{
		getGLProgramState()->setUniformTexture("u_texture1", _texture1);
	}
}
