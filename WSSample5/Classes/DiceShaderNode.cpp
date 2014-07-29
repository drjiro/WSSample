/**
 * Node for a cubic dice.
 *
 * Copyright (c) 2014 Takao WADA.
 */
#include "DiceShaderNode.h"

// Vertices
static GLfloat s_vertices[] = {
	//  x      y      z    Nx    Ny    Nz     R     G     B     A     U      V
	-1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,				// 1
	-1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.25f,
	1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.25f, 0.0f,
	1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.25f, 0.0f,
	-1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.25f,
	1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.25f, 0.25f,

	-1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.25f, 0.0f,				// 2
	-1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.25f, 0.25f,
	1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.5f, 0.0f,
	1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.5f, 0.0f,
	-1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.25f, 0.25f,
	1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.5f, 0.25f,

	1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.5f, 0.0f,				// 3
	1.0f, -1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.5f, 0.25f,
	1.0f, 1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.75f, 0.0f,
	1.0f, 1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.75f, 0.0f,
	1.0f, -1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.5f, 0.25f,
	1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.75f, 0.25f,

	-1.0f, 1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.25f,				// 4
	-1.0f, -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.5f,
	-1.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.25f, 0.25f,
	-1.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.25f, 0.25f,
	-1.0f, -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.5f,
	-1.0f, -1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.25f, 0.5f,

	1.0f, 1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.25f, 0.25f,				// 5
	1.0f, -1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.25f, 0.5f,
	-1.0f, 1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.5f, 0.25f,
	-1.0f, 1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.5f, 0.25f,
	1.0f, -1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.25f, 0.5f,
	-1.0f, -1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.5f, 0.5f,

	-1.0f, -1.0f, 1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.5f, 0.25f,				// 6
	-1.0f, -1.0f, -1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.5f, 0.5f,
	1.0f, -1.0f, 1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.75f, 0.25f,
	1.0f, -1.0f, 1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.75f, 0.25f,
	-1.0f, -1.0f, -1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.5f, 0.5f,
	1.0f, -1.0f, -1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.75f, 0.5f
};

/**
 * Create a node.
 *
 * @param vert Vertex shader file name.
 * @param frag Fragment shader file name.
 * @param texture Texture file name.
 * @return Created node 
 */
DiceShaderNode* DiceShaderNode::create(const std::string &vert, const std::string& frag,
	Texture2D* texture)
{
	auto node = new DiceShaderNode();
	node->setTexture0(texture);
	node->init(vert, frag);
	node->autorelease();

	return node;
}

/**
 * Initialize shaders from files.
 *
 * @param vert Vertex shader file name.
 * @param frag Fragment shader file name.
 * @param texture Texture file name.
 * @return if succcess, resturn true
 */
bool DiceShaderNode::init(const std::string &vert, const std::string &frag)
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

	// Pass the texture
	getGLProgramState()->setUniformTexture("u_texture0", _texture0);

	scheduleUpdate();

	return true;
}

DiceShaderNode::DiceShaderNode()
{
}

DiceShaderNode::~DiceShaderNode()
{
}

/**
 * Update the state per a frame.
 *
 * @param dt Elapsed time
 */
void DiceShaderNode::update(float dt)
{
	_time += dt;

	//_scale3d.x = sin(_time);
	_position3d.x = sin(_time);
	_position3d.y = cos(_time);
}

/**
 * Draw a primitive.
 *
 * @param transform Transform matrix
 * @param flags Flags to draw 
 */
void DiceShaderNode::onDraw(const Mat4 &transform, uint32_t flags)
{
	setCommon3dEnvironment();

	auto glProgramState = getGLProgramState();
	// Pass the position parameter to the vertex shader's attribute variable
	glProgramState->setVertexAttribPointer("a_position",
		3, GL_FLOAT, GL_FALSE, 48, s_vertices);
	glProgramState->setVertexAttribPointer("a_normal",
		3, GL_FLOAT, GL_FALSE, 48, s_vertices + 3);
	glProgramState->setVertexAttribPointer("a_color",
		4, GL_FLOAT, GL_FALSE, 48, s_vertices + 6);
	glProgramState->setVertexAttribPointer("a_texCoord",
		2, GL_FLOAT, GL_FALSE, 48, s_vertices + 10);
	//	glProgramState->apply(transform);
	glProgramState->apply(transform);

	glEnable(GL_DEPTH_TEST);
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);

	// Draw triangles
	glDrawArrays(GL_TRIANGLES, 0, 36);

//	CC_INCREMENT_GL_DRAWN_BATCHES_AND_VERTICES(1, 36);
}