/**
* Node with textures and shader.
*
* Copyright (c) 2014 Takao WADA.
*/
#include "ShaderTextureNode.h"

/**
* Create a node.
*
* @param vert Vertex shader file name.
* @param frag Fragment shader file name.
* @param texture Texture file name.
* @return Created node
*/
ShaderTextureNode* ShaderTextureNode::create(const std::string &vert, const std::string& frag, Texture2D* texture)
{
	auto node = new ShaderTextureNode();
	node->init(vert, frag, texture);
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
bool ShaderTextureNode::init(const std::string &vert, const std::string &frag, Texture2D* texture)
{
#if CC_ENABLE_CACHE_TEXTURE_DATA
	auto listener = EventListenerCustom::create(EVENT_COME_TO_FOREGROUND, [this](EventCustom* event){
		this->setGLProgramState(nullptr);
		loadShaders(_vertFileName, _fragFileName);
	});

	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
#endif
	_texture0 = texture;

	_vertFileName = vert;
	_fragFileName = frag;

	loadShaders(vert, frag);

	_time = 0;
	_resolution = Vec2(1, 1);
	getGLProgramState()->setUniformVec2("resolution", _resolution);

	// Pass the texture to the shader
	getGLProgramState()->setUniformTexture("u_texture0", _texture0);

	scheduleUpdate();

	setContentSize(Size(1, 1));
	setAnchorPoint(Vec2(0.5f, 0.5f));

	return true;
}

ShaderTextureNode::ShaderTextureNode()
{
}

ShaderTextureNode::~ShaderTextureNode()
{
}

/**
 * Update the state per a frame.
 *
 * @param dt Elapsed time
 */
void ShaderTextureNode::update(float dt)
{
	_time += dt;
	// Pass the current time (float value)
	getGLProgramState()->setUniformFloat("u_time", _time);
}

/**
 * Draw a primitive.
 *
 * @param transform Transform matrix
 * @param flags Flags to draw
 */
void ShaderTextureNode::onDraw(const Mat4 &transform, uint32_t flags)
{
	GLfloat vertices[48] = {
		-1, -1, 0, 1, 1, 1, 1, 1,		// Lower-Left
		1, -1, 1, 1, 1, 1, 1, 1,		// Lower-Right
		1, 1, 1, 0, 1, 1, 1, 1,			// Upper-Right
		-1, -1, 0, 1, 1, 1, 1, 1,		// Lower-Left
		1, 1, 1, 0, 1, 1, 1, 1,			// Upper-Right
		-1, 1, 0, 0, 1, 1, 1, 1,		// Upper-Left
	};


	auto glProgramState = getGLProgramState();
	glProgramState->setVertexAttribPointer("a_position", 2, GL_FLOAT, GL_FALSE, 32, vertices);
	glProgramState->setVertexAttribPointer("a_texCoord", 2, GL_FLOAT, GL_FALSE, 32, vertices + 2);
	glProgramState->setVertexAttribPointer("a_color", 4, GL_FLOAT, GL_FALSE, 32, vertices + 4);
	glProgramState->apply(transform);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glDisable(GL_DEPTH_TEST);
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);

	//	Alpha blending
//	glEnable(GL_BLEND);
//	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	glDrawArrays(GL_TRIANGLES, 0, 6);

//	glDisable(GL_BLEND);

	CC_INCREMENT_GL_DRAWN_BATCHES_AND_VERTICES(1, 6);
}
