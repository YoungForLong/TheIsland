#pragma once

#include"cocos2d.h"
#include"VirtualMap.h"
#include"TemplateFunctions.h"
#include"Hero.h"

using namespace cocos2d;

//from 0 to 401 are entities' Z-ORDERs
typedef std::function<void()> VisionMapCB;//回调函数对象

class Hero;

class VisionMap:public Layer
{
public:
	VisionMap();
	~VisionMap();
	virtual bool init();
	CREATE_FUNC(VisionMap);
	virtual bool setCache();
	bool drawMap();
	void clearCache();
	void scheduleFunc(float fl);
	void getVisionEntity();
protected:
	static Point _lastHeroPosition;
private:
	Sprite* _displayedMap[4];
	bool _firstDraw;
	static unsigned int _timmingCount;
	Hero* _hero;
};


