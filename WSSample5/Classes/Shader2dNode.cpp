/*
 * Shader node for 2-D.
 *
 * Copyright (C) 2014 Wada Takao.
 */
#include "Shader2dNode.h"

/**
* Constructor.
*/
Shader2dNode::Shader2dNode()
:_center(Vec2(0.0f, 0.0f))
, _resolution(Vec2(0.0f, 0.0f))
{
}

/**
* Destructor.
*/
Shader2dNode::~Shader2dNode()
{
}

/**
* Create an instance with shaders.
*
* @param vert Vertex shader file name.
* @param frag Fragment shader file name.
*/
Shader2dNode* Shader2dNode::create(const std::string &vert, const std::string& frag)
{
	auto node = new Shader2dNode();
	node->init(vert, frag);
	node->autorelease();

	return node;
}

/**
* Initialize shaders from files.
*
* @param vert Vertex shader file name.
* @param frag Fragment shader file name.
*/
bool Shader2dNode::init(const std::string &vert, const std::string &frag)
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

	scheduleUpdate();

	setContentSize(Size(1, 1));
	setAnchorPoint(Vec2(0.5f, 0.5f));


	return true;
}


/**
* Update per frames.
*
* @param dt Elapsed time
*/
void Shader2dNode::update(float dt)
{
	_time += dt;

	// Pass the current time (float value)
	getGLProgramState()->setUniformFloat("u_time", _time);
}


/**
* Set new position.
*
* @param newPosition New position
*/
void Shader2dNode::setPosition(const Vec2 &newPosition)
{
	Node::setPosition(newPosition);
	auto position = getPosition();
	_center = Vec2(position.x * CC_CONTENT_SCALE_FACTOR(), position.y * CC_CONTENT_SCALE_FACTOR());
	getGLProgramState()->setUniformVec2("center", _center);
}

/**
* Draw a primitive.
*
* @param transform Transform matrix
* @param flags Flags to draw
*/
void Shader2dNode::onDraw(const Mat4 &transform, uint32_t flags)
{
	// Vertices (x,y) x 3
	GLfloat vertices[18] = {
		0, 0, 1.0f, 0.0f, 1.0f, 1.0f,
		0.5f, 0, 0.0f, 1.0f, 1.0f, 1.0f,
		0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f
	};

	auto glProgramState = getGLProgramState();
	// Pass the position parameter to the vertex shader's attribute variable
	glProgramState->setVertexAttribPointer("a_position",
		2, GL_FLOAT, GL_FALSE, 24, vertices);
	glProgramState->setVertexAttribPointer("a_color",
		4, GL_FLOAT, GL_FALSE, 24, vertices + 2);
	glProgramState->apply(transform);

	// Draw a triangle
	glDrawArrays(GL_TRIANGLES, 0, 3);

	CC_INCREMENT_GL_DRAWN_BATCHES_AND_VERTICES(1, 3);
}
