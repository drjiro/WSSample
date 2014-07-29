/*
* Shader node.
*
* Copyright (C) 2014 Wada Takao.
*/
#include "ShaderNode.h"

/**
 * Constructor.
 */
ShaderNode::ShaderNode()
: _time(0.0f)
, _texture0(nullptr)
, _texture1(nullptr)
, _position3d(0,0,0)
, _rotation3d(0, 0, 0)
, _scale3d(1, 1, 1)
{
}

/**
* Destructor.
*/
ShaderNode::~ShaderNode()
{
}

/**
 *	Load and compile, link shader programs from file.
 *
 *	@param vert Vertex shader file name.
 *	@param frag Fragment shader file name.
 */
void ShaderNode::loadShaders(const std::string &vert, const std::string &frag)
{
	auto fileUtiles = FileUtils::getInstance();

	// Fragment shader
	auto fragmentFilePath = fileUtiles->fullPathForFilename(frag);
	auto fragSource = fileUtiles->getStringFromFile(fragmentFilePath);

	// Vertex shader
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
 * Update per frames.
 *
 * @param dt Elapsed time
 */
void ShaderNode::update(float dt)
{
	_time += dt;

	// Pass the current time (float value)
	getGLProgramState()->setUniformFloat("u_time", _time);
}

/**
 * Draw node.
 *
 * @param renderer Renderer object
 * @param transform Transform matrix
 * @param flags Flags to draw
 */
void ShaderNode::draw(Renderer *renderer, const Mat4 &transform, uint32_t flags)
{
	_customCommand.init(_globalZOrder);
	_customCommand.func = CC_CALLBACK_0(ShaderNode::onDraw, this, transform, flags);
	renderer->addCommand(&_customCommand);
}

/**
 * Draw a 3-D model.
 */
void ShaderNode::setCommon3dEnvironment()
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

	// Compute transformation Scaling->Roataion->Translation
	Mat4 worldMatrix;
	worldMatrix = transMatrix * rotYMatrix * rotXMatrix * rotZMatrix * scaleMatrix;

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
