#ifndef _GAME_SCENE_H_
#define _GAME_SCENE_H_
#include"ControllerLayer.h"
#include"cocos2d.h"
#include"VisionMap.h"
using namespace cocos2d;

class GameScene : public Scene
{
public:
	GameScene();
	~GameScene();
	virtual bool init();
	CREATE_FUNC(GameScene);
private:
};

#endif