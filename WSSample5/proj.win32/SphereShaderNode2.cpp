#include "SphereShaderNode2.h"

SphereShaderNode2::SphereShaderNode2()
:_time(0.0f),
_rotY(0.0f)
{
	// Need initialize!!!!
}

SphereShaderNode2::~SphereShaderNode2()
{
	CC_SAFE_DELETE_ARRAY(_vertices);
	CC_SAFE_DELETE_ARRAY(_indices);
}

SphereShaderNode2* SphereShaderNode2::shaderNodeWithVertex(const std::string &vert, const std::string& frag,
	Texture2D* texture)
{
	auto node = new SphereShaderNode2();
	node->initWithVertex(vert, frag, texture);
	node->autorelease();

	return node;
}

/**
*	Initialize shaders from files.
*
*	@param vert Vertex shader file name.
*	@param frag Fragment shader file name.
*	@param texture Texture file name.
*	@param frag Fragment shader file name.
*/
bool SphereShaderNode2::initWithVertex(const std::string &vert, const std::string &frag,
	Texture2D* texture)
{
#if CC_ENABLE_CACHE_TEXTURE_DATA
	auto listener = EventListenerCustom::create(EVENT_COME_TO_FOREGROUND, [this](EventCustom* event){
		this->setGLProgramState(nullptr);
		loadShaderVertex(_vertFileName, _fragFileName);
	});

	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
#endif

	_vertFileName = vert;
	_fragFileName = frag;

	int numRings = 4;
	int numSlices = 4;
	float radius = 1;

	loadShaderVertex(vert, frag);

	float rcosphi;			// r * cosƒÓ
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
	for (int i = 0; i < numRings - 1; i++)
	{
		// Compute the angle
		float phi = i * M_PI / numRings;

		// Compute Y coordiante
		y = sinf(phi);
		// Compute cos(phi)
		rcosphi = cosf(phi);

		float v = (float)(i + 1) / numRings; //1.0f - (float)(i + 1) / numRings;

		// Loop for horizontal
		for (int j = 0; j <= numSlices; j++)
		{
			float theta = j * 2 * M_PI / numSlices;
			// Compute X coordinate
			x = cosf(theta) * rcosphi;
			// Compute Z coordinate
			z = sinf(theta) * rcosphi;
			// Compute normal
			Vec3 normal = Vec3(x, y, z);
			normal.normalize();
			/*
			float u = 1.0f - (float)j / numSlices;
			if (u < 0.0f) {
			u = 0.0f;
			}
			*/
			float u = (float)j / numSlices;
			if (u > 1.0f)
			{
				u = 1.0f;
			}

			// Set to the element
			_vertices[n++] = VertexPositionNormalColorTexture(
				radius * x, radius * y, radius * z,
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
		_indices[n++] = (unsigned short)(i + 1);
		_indices[n++] = (unsigned short)(i + 2);
	}

	// Middle triangles
	int m = 1;
	for (int i = 0; i < numRings - 2; i++)
	{
		for (int j = 0; j < numSlices; j++)
		{
			// First triangle
			_indices[n++] = (unsigned short)(m + j);
			_indices[n++] = (unsigned short)(m + numSlices + 1 + j);
			_indices[n++] = (unsigned short)(m + j + 1);
			// Second triangle
			_indices[n++] = (unsigned short)(m + j + 1);
			_indices[n++] = (unsigned short)(m + numSlices + j + 1);
			_indices[n++] = (unsigned short)(m + numSlices + j + 2);
		}
		m += numSlices + 1;
	}
	// Bottom triangles
	unsigned short index = _numVertices - 1 - numSlices - 1;
	for (int i = 0; i < numSlices; i++)
	{
		_indices[n++] = (unsigned short)(_numVertices - 1);
		_indices[n++] = (unsigned short)(index + i + 1);
		_indices[n++] = (unsigned short)(index + i);
	}

	for (int i = 0; i < _numIndices; i += 3) {
		log("%2d %2d %2d\n", _indices[i], _indices[i + 1], _indices[i + 2]);
	}

	_time = 0;
	// Pass the texture
	getGLProgramState()->setUniformTexture("u_texture0", texture);

	scheduleUpdate();

	return true;
}

/**
*	Load shaders from files.
*
*	@param vert Vertex shader file name.
*	@param frag Fragment shader file name.
*/
void SphereShaderNode2::loadShaderVertex(const std::string &vert, const std::string &frag)
{
	auto fileUtiles = FileUtils::getInstance();

	// frag
	auto fragmentFilePath = fileUtiles->fullPathForFilename(frag);
	auto fragSource = fileUtiles->getStringFromFile(fragmentFilePath);

	// vert
	std::string vertSource;
	if (vert.empty()) {
		vertSource = ccPositionTextureColor_vert;
	}
	else {
		std::string vertexFilePath = fileUtiles->fullPathForFilename(vert);
		vertSource = fileUtiles->getStringFromFile(vertexFilePath);
	}

	auto glprogram = GLProgram::createWithByteArrays(vertSource.c_str(), fragSource.c_str());
	auto glprogramstate = GLProgramState::getOrCreateWithGLProgram(glprogram);
	setGLProgramState(glprogramstate);
}

/**
*	Update per a frame.
*
*	@param dt Elapsed time
*/
void SphereShaderNode2::update(float dt)
{
	_time += dt;

}

/**
*	Draw node.
*
*	@param renderer Renderer object
*	@param transform Transform matrix
*	@param flags Flags to draw
*/
void SphereShaderNode2::draw(Renderer *renderer, const Mat4 &transform, uint32_t flags)
{
	_customCommand.init(_globalZOrder);
	_customCommand.func = CC_CALLBACK_0(SphereShaderNode2::onDraw, this, transform, flags);
	renderer->addCommand(&_customCommand);
}

/**
* Draw a primitive.
*
* @param transform Transform matrix
* @param flags Flags to draw
*/
void SphereShaderNode2::onDraw(const Mat4 &transform, uint32_t flags)
{

	// Create world matrix
	Mat4 worldMatrix;
	Mat4::createTranslation(0, 0, 0, &worldMatrix);
	_rotY += 1.0f * M_PI / 180;
	Mat4 rotMatrix;
	Mat4::createRotationY(_rotY, &rotMatrix);
	Mat4 scaleMatrix;
	Mat4::createScale(1, 1, 1, &scaleMatrix);
	Mat4 transMatrix;
	Mat4::createTranslation(0, 0, 0, &transMatrix);

	//	worldMatrix = rotMatrix * transMatrix;
	//	worldMatrix = transMatrix * rotMatrix * scaleMatrix;

	worldMatrix = rotMatrix;

	// Pass the world matrix
	getGLProgramState()->setUniformMat4("u_world", worldMatrix);

	// Pass the view matrix
	getGLProgramState()->setUniformMat4("u_modelView", _camera->getViewMatrix());

	// Pass the projection matrix
	getGLProgramState()->setUniformMat4("u_proj", _camera->getProjectionMatrix());

	auto glProgramState = getGLProgramState();
	// Pass the position parameter to the vertex shader's attribute variable
	glProgramState->setVertexAttribPointer("a_position",
		3, GL_FLOAT, GL_FALSE, 48, _vertices);
	glProgramState->setVertexAttribPointer("a_normal",
		3, GL_FLOAT, GL_FALSE, 48, _vertices + 3);
	glProgramState->setVertexAttribPointer("a_color",
		4, GL_FLOAT, GL_FALSE, 48, _vertices + 6);
	glProgramState->setVertexAttribPointer("a_texCoord",
		2, GL_FLOAT, GL_FALSE, 48, _vertices + 10);
	glProgramState->apply(transform);

	glEnable(GL_DEPTH_TEST);

	// Draw triangles
	glDrawArrays(GL_TRIANGLES, 0, 3);
//	glDrawElements(GL_TRIANGLES, _numIndices, GL_UNSIGNED_SHORT, _indices);

//	CC_INCREMENT_GL_DRAWN_BATCHES_AND_VERTICES(1, 3);
}

void SphereShaderNode2::setCamera(Camera3d* camera)
{
	_camera = camera;
}