#include "NewShaderNode.h"

NewShaderNode::NewShaderNode()
:_center(Vec2(0.0f, 0.0f))
, _resolution(Vec2(0.0f, 0.0f))
, _time(0.0f)
{
}

NewShaderNode::~NewShaderNode()
{
}

NewShaderNode* NewShaderNode::create(const std::string &vert, const std::string& frag,
	Texture2D* texture)
{
	auto node = new NewShaderNode();
	node->init(vert, frag, texture);
	node->autorelease();

	return node;
}

bool NewShaderNode::init(const std::string &vert, const std::string &frag,
	Texture2D* texture)
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

	_time = 0;
	_resolution = Vec2(1, 1);
	getGLProgramState()->setUniformVec2("resolution", _resolution);
	// Add line
	getGLProgramState()->setUniformTexture("u_texture0", texture);

	scheduleUpdate();

	setContentSize(Size(1, 1));
	setAnchorPoint(Vec2(0.5f, 0.5f));


	return true;
}

void NewShaderNode::loadShaders(const std::string &vert, const std::string &frag)
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

void NewShaderNode::update(float dt)
{
	_time += dt;
}

void NewShaderNode::setPosition(const Vec2 &newPosition)
{
	Node::setPosition(newPosition);
	auto position = getPosition();
	_center = Vec2(position.x * CC_CONTENT_SCALE_FACTOR(), position.y * CC_CONTENT_SCALE_FACTOR());
	getGLProgramState()->setUniformVec2("center", _center);
}

void NewShaderNode::draw(Renderer *renderer, const Mat4 &transform, uint32_t flags)
{
	_customCommand.init(_globalZOrder);
	_customCommand.func = CC_CALLBACK_0(NewShaderNode::onDraw, this, transform, flags);
	renderer->addCommand(&_customCommand);
}

/**
* Draw a primitive.
*
* @param transform Transform matrix
* @param flags Flags to draw
*/
void NewShaderNode::onDraw(const Mat4 &transform, uint32_t flags)
{
	// Vertices (x,y) x 3
	GLfloat vertices[48] = {
		//  x     y     R     G     B     A     U     V
		0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.25f, 0.5f,
		0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.5f, 0.5f,
		0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.5f, 0.25f,
		0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.5f, 0.25f,
		0.0f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.25f, 0.25f,
		0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.25f, 0.5f
	};

	auto glProgramState = getGLProgramState();
	// Pass the position parameter to the vertex shader's attribute variable
	glProgramState->setVertexAttribPointer("a_position",
		2, GL_FLOAT, GL_FALSE, 32, vertices);
	glProgramState->setVertexAttribPointer("a_color",
		4, GL_FLOAT, GL_FALSE, 32, vertices + 2);
	glProgramState->setVertexAttribPointer("a_texCoord",
		2, GL_FLOAT, GL_FALSE, 32, vertices + 6);
	glProgramState->apply(transform);

	// Draw a triangle
	glDrawArrays(GL_TRIANGLES, 0, 6);

	CC_INCREMENT_GL_DRAWN_BATCHES_AND_VERTICES(1, 6);
}
