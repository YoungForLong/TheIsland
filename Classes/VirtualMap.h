#pragma once

#include"cocos2d.h"
#include"PngQuery.h"
#include"Cell_Space_Partition.h"
#include"VirtualEntity.h"


using namespace cocos2d;


class VirtualMap:public VirtualEntity,public Singleton<VirtualMap>
{
public:
	VirtualMap();
	~VirtualMap();
public:
	virtual void update() {}
	virtual bool handleMsg(const Telegram& msg){ return false; }
	virtual bool clear() { return true; }
public://inlitialize map data
	bool loadPlantIndex();//from plant.xml
	bool loadAnimalIndex();//from animal.xml
	bool setCache();//set Map sprite cache
protected://handle imgs
	int getAlpha(Point, Image*);//图片上的位置和图片在地图上的位置
};

#define VMAP VirtualMap::instance()





