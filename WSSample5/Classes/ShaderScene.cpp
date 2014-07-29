/*
* Scene using shaders.
*
* Copyright (C) 2014 Wada Takao.
*/
#include "ShaderScene.h"
#include "ModelShaderNode.h"

#include "NewParametricShaderNode.h"
#include "TorusShaderNode.h"
#include "ParametricSphereShaderNode.h"
#include "KleinBottleShaderNode.h"
#include "SphereShaderNode.h"
#include "DiceShaderNode.h"
#include "NewShaderNode.h"
#include "ShaderTextureNode.h"

USING_NS_CC;

Scene* ShaderScene::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = ShaderScene::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

/**
* On "init" you need to initialize your instance
*
* @ return if success, return true
*/
bool ShaderScene::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Layer::init())
	{
		return false;
	}
	// Set the clear color
	glClearColor(0.6f, 0.8f, 1.0f, 1.0f);

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	// Get window size
	auto size = Director::getInstance()->getWinSize();

	// Add a 2-D node to the scene /////////////////////////////////////
	// Load sprite
	auto airportSprite = Sprite::create("Images/airport.png");
	auto airportTexture = airportSprite->getTexture();

	// Background
	//auto planeNode = ShaderTextureNode::create("Shaders/simple.vsh", "Shaders/example_lensFlare.fsh", airportTexture);
	//auto planeNode = ShaderTextureNode::create("Shaders/simple.vsh", "Shaders/greyScale.fsh", airportTexture);
	auto planeNode = ShaderTextureNode::create("Shaders/simple.vsh", "Shaders/simple.fsh", airportTexture);
	// Add a background
	addChild(planeNode);

	// Add a 3-D node to the scene /////////////////////////////////////
	// Create a new perspective camera.
	_camera = new Camera3d(size.width, size.height);
	_camera->setEye(0, 0, 4);

	// Create a new light.
	_light = new Light();
	_light->setDirection(-1, -1, -1);

	// create a node with vertices and shaders
	auto diceSprite = Sprite::create("Images/dice.png");
	auto diceTexture = diceSprite->getTexture();
	auto diceNode = DiceShaderNode::create(
		"Shaders/vertexcolor.vsh", "Shaders/vertexcolor.fsh", diceTexture);
	// Set the camera to the node
	diceNode->setCamera(_camera);
	diceNode->setLight(_light);
	// Move right
	diceNode->setPosition(0, 0, 0);
	//addChild(diceNode);

	// create a node with vertices and shaders
	auto baseSprite = Sprite::create("Images/basetex.png");
	auto baseTexture = baseSprite->getTexture();
	auto sphereShaderNode = SphereShaderNode::create(
		"Shaders/blinn-phong.vsh", "Shaders/blinn-phong.fsh", baseTexture);
	// Set the camera to the node
	sphereShaderNode->setCamera(_camera);
	sphereShaderNode->setLight(_light);
	// Move left
	sphereShaderNode->setPosition(0, 0, 0);
	//addChild(sphereShaderNode);

	// create a node with vertices and shaders
	auto sphereShader2Node = SphereShaderNode::create(
		"Shaders/perpixel.vsh", "Shaders/perpixel.fsh", baseTexture);
	// Set the camera to the node
	sphereShader2Node->setCamera(_camera);
	sphereShader2Node->setLight(_light);
	// Move right
	sphereShader2Node->setPosition(0, 0, 0);
	//addChild(sphereShader2Node);

	// Create normal map
	auto normalSprite = Sprite::create("Images/normalmap.png");
	auto normalTexture = normalSprite->getTexture();

	ParametricSphereShaderNode* parametricSphereShaderNode = ParametricSphereShaderNode::create(
		"Shaders/normalmap.vsh", "Shaders/normalmap.fsh");
	parametricSphereShaderNode->setCamera(_camera);
	parametricSphereShaderNode->setLight(_light);
	parametricSphereShaderNode->setTexture0(baseTexture);
	parametricSphereShaderNode->setTexture1(normalTexture);
	// Set the position and size
	//addChild(parametricSphereShaderNode);

	KleinBottleShaderNode* kleinShaderNode = KleinBottleShaderNode::create(
		"Shaders/normalmap.vsh", "Shaders/normalmap.fsh");
	kleinShaderNode->setCamera(_camera);
	kleinShaderNode->setLight(_light);

	kleinShaderNode->setTexture0(baseTexture);
	kleinShaderNode->setTexture1(normalTexture);
	addChild(kleinShaderNode);

	TorusShaderNode* torusShaderNode = TorusShaderNode::create(
		"Shaders/normalmap.vsh", "Shaders/normalmap.fsh");
	torusShaderNode->setCamera(_camera);
	torusShaderNode->setLight(_light);
	torusShaderNode->setTexture0(baseTexture);
	torusShaderNode->setTexture1(normalTexture);
	// Set the position and size
	//addChild(torusShaderNode);

	NewParametricShaderNode* newParametricShaderNode = NewParametricShaderNode::create(
		//	"Shaders/vertexsin.vsh", "Shaders/vertexsin.fsh");
		//"Shaders/alpha.vsh", "Shaders/alpha.fsh");
				"Shaders/toon.vsh", "Shaders/toon.fsh");
	newParametricShaderNode->setCamera(_camera);
	newParametricShaderNode->setLight(_light);
	newParametricShaderNode->setTexture0(airportTexture);
	// Set the position and size
//	addChild(newParametricShaderNode);

	return true;
}

ShaderScene::ShaderScene()
: _camera(nullptr),
_light(nullptr)
{
}

ShaderScene::~ShaderScene()
{
	if (_camera != nullptr) {
		delete _camera;
	}
	if (_light != nullptr) {
		delete _light;
	}
}