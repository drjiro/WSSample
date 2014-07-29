#include "ModelShaderNode.h"

ModelShaderNode::ModelShaderNode()
: _time(0.0f),
_rotY(0.0f),
_sprite3d(nullptr)
{
}

ModelShaderNode::~ModelShaderNode()
{
}

/**
 *	Initialize.
 *
 *	@param vert Vertex shader file name
 *	@param frag Fragment shader file name
 *	@param objFilePath Model file path
 *	@param textureFilePath Texture file path
 */
ModelShaderNode* ModelShaderNode::shaderNodeWithFile(const std::string &vert,
													const std::string& frag,
													const std::string& objFilePath,
													const std::string &textureFilePath)
{
	auto node = new ModelShaderNode();
	node->createFromObjFileAndTexture(vert, frag, objFilePath, textureFilePath);
	node->autorelease();

	return node;
}

/**
*	Create node data from the file.
*
*	@param vert Vertex shader file name
*	@param frag Fragment shader file name
*	@param objFilePath Model file path
*	@param textureFilePath Texture file path
*/
bool ModelShaderNode::createFromObjFileAndTexture(const std::string &vert,
												const std::string &frag,
												const std::string& objFilePath,
												const std::string &textureFilePath)
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

	loadShaderFile(vert, frag);

	_time = 0;
	// Pass the texture
//	getGLProgramState()->setUniformTexture("u_texture0", texture);

	// シェーダー付き３Dとスプライトオブジェクトを作成する。
	_sprite3d = Sprite3D::create(objFilePath);
	if (_sprite3d)
	{
		// ３DスプライトはAuto release
//		_sprite3d->autorelease();
		_sprite3d->retain();
		// ３Dスプライトにテクスチャを設定する。
		_sprite3d->setTexture(textureFilePath);
	}

	scheduleUpdate();

	return true;
}

/**
 *	Load shaders from the file.
 *
 *	@param vert Vertex shader file name
 *	@param frag Fragment shader file name
 */
void ModelShaderNode::loadShaderFile(const std::string &vert, const std::string &frag)
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
 *	Update model data.
 */
void ModelShaderNode::update(float dt)
{
	_time += dt;

}

/**
 *	Set the position.
 */
void ModelShaderNode::setPosition(const Vec3 &newPosition)
{
	_position3d = newPosition;
}

/**
 *	Draw a model
 */
void ModelShaderNode::draw(Renderer *renderer, const Mat4 &transform, uint32_t flags)
{
	_customCommand.init(_globalZOrder);
	_customCommand.func = CC_CALLBACK_0(ModelShaderNode::onDraw, this, transform, flags);
	renderer->addCommand(&_customCommand);
}

/**
* Draw a mesh.
*
* @param transform Transform matrix
* @param flags Flags to draw
*/
void ModelShaderNode::onDraw(const Mat4 &transform, uint32_t flags)
{
	// Create world matrix
	Mat4 worldMatrix;
	Mat4::createTranslation(4, 0, 0, &worldMatrix);
	_rotY += 1.0f * M_PI / 180;
	Mat4 rotMatrix;
	Mat4::createRotationY(_rotY, &rotMatrix);
	Mat4 scaleMatrix;
	Mat4::createScale(1.0f, 1.0f, 1.0f, &scaleMatrix);
	Mat4 transMatrix;
	Mat4::createTranslation(0, 0, 0, &transMatrix);

	//	worldMatrix = rotMatrix * transMatrix;
	worldMatrix = transMatrix * rotMatrix;

	// Pass the world matrix
	getGLProgramState()->setUniformMat4("u_world", worldMatrix);

	// Pass the view matrix
	getGLProgramState()->setUniformMat4("u_modelView", camera->getViewMatrix());

	// Pass the projection matrix
	getGLProgramState()->setUniformMat4("u_proj", camera->getProjectionMatrix());

	auto mesh = _sprite3d->getMesh();
	long offset = 0;
	for (auto i = 0; i < mesh->getMeshVertexAttribCount(); i++)
	{
		auto meshvertexattrib = mesh->getMeshVertexAttribute(i);

		_glProgramState->setVertexAttribPointer(s_attributeNames[meshvertexattrib.vertexAttrib],
			meshvertexattrib.size,
			meshvertexattrib.type,
			GL_FALSE,
			mesh->getVertexSizeInBytes(),
			(void*)offset);
		offset += meshvertexattrib.attribSizeBytes;
	}
	//draw
	{
		glEnable(GL_CULL_FACE);
		glCullFace(GL_FRONT);
		glEnable(GL_DEPTH_TEST);
//		Color4F color(_sprite3d->getDisplayedColor());
//		color.a = _sprite3d->getDisplayedOpacity() / 255.0f;

//		_glProgramState->setUniformVec4("u_color", Vec4(color.r, color.g, color.b, color.a));

		auto mesh = _sprite3d->getMesh();
		glBindBuffer(GL_ARRAY_BUFFER, mesh->getVertexBuffer());
		_glProgramState->apply(transform);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->getIndexBuffer());
		glDrawElements((GLenum)mesh->getPrimitiveType(), (GLsizei)mesh->getIndexCount(), (GLenum)mesh->getIndexFormat(), 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glDisable(GL_DEPTH_TEST);
		glCullFace(GL_BACK);
		glDisable(GL_CULL_FACE);
		CC_INCREMENT_GL_DRAWN_BATCHES_AND_VERTICES(1, mesh->getIndexCount());
	}

	CC_INCREMENT_GL_DRAWN_BATCHES_AND_VERTICES(1, 36);
}

void ModelShaderNode::setCamera(Camera3d* camera)
{
	this->camera = camera;
}