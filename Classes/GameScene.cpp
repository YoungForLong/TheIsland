#include"GameScene.h"
#include"Pocket.h"

GameScene::GameScene()
{
}

GameScene::~GameScene()
{
}

bool GameScene::init()
{
	if (!Scene::init())
	{
		return false;
	}
	auto map = new VisionMap();
	if (map->init())
	{
		this->addChild(map, -1);
	}
	else
		return false;
	auto controller = ControllerLayer::create();
	this->addChild(controller, 0);

	auto pocket = Pocket::create();
	this->addChild(pocket,1);

	return true;
}