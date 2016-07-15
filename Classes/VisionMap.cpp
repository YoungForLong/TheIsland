#include"VisionMap.h"
#include"Crocodile.h"
#include"Plant.h"
#include"Wood.h"
#include"Meat.h"
#include"EntityManenger.h"
#include<xtree>

Point VisionMap::_lastHeroPosition = Point::ZERO;
unsigned int VisionMap::_timmingCount = 0;

VisionMap::VisionMap()
{
	VMAP;//强制调用构造函数,执行地图的初始化
	VMAP->loadPlantIndex();
	VMAP->loadAnimalIndex();

	_hero = Hero::create();
	_firstDraw = false;
	_lastHeroPosition = _hero->position();
	_hero->skeleton()->setPosition(HERO_LOCATION_SCREEN);
}

VisionMap::~VisionMap()
{

}

bool VisionMap::init()
{
	if (!Layer::init())
	{
		return false;
	}
	if (!setCache())
	{
		return false;
	}

	this->schedule(schedule_selector(VisionMap::scheduleFunc), 1 / MAIN_FPS);

	getVisionEntity();

	this->addChild(_hero->skeleton(), 400-_hero->position().y);
	//end test
	return true;
}

bool VisionMap::setCache()
{
	//normal map Img
	for (int i = 0; i < 4; i++)
	{
		Texture2D* texture = Director::getInstance()->getTextureCache()->addImage(String::createWithFormat("Environment_%d.png", i)->getCString());
		auto spriteBatchNode = SpriteBatchNode::createWithTexture(texture);
		this->addChild(spriteBatchNode);
		SpriteFrameCache::getInstance()->addSpriteFramesWithFile(String::createWithFormat("Environment_%d.plist", i)->getCString(),
			String::createWithFormat("Environment_%d.png", i)->getCString());
	}
	//end

	//testRect
	/*Texture2D* texture = Director::getInstance()->getTextureCache()->addImage("testRect.png");
	auto spriteBatchNode = SpriteBatchNode::createWithTexture(texture);
	this->addChild(spriteBatchNode);
	auto testSp = SpriteFrame::createWithTexture(Director::getInstance()->getTextureCache()->getTextureForKey("testRect.png"),Rect(0,0,10,10));
	SpriteFrameCache::getInstance()->addSpriteFrame(testSp, "testRect.png");

	for (int i = 0; i < 401; i++)
	{
		for (int j = 0; j < 401; j++)
		{
			if (globalMap->getLandIndex()[i][j].landType_ != DRY_LAND)
			{
				auto sp = Sprite::createWithSpriteFrameName("testRect.png");
				sp->setAnchorPoint(Point::ZERO);
				sp->setPosition(Point(i*RECT_SIZE, j*RECT_SIZE ));
				this->addChild(sp, 10000);
			}
		}
	}*/
	//testEnd
	return true;
}

bool VisionMap::drawMap()
{
	int col = (int)(_hero->position().x - HERO_LOCATION_SCREEN.x) / IMG_WIDTH;
	int row = (int)(_hero->position().y- HERO_LOCATION_SCREEN.y) / IMG_HEIGHT;
	int colRemainder = (int)(_hero->position().x - HERO_LOCATION_SCREEN.x) % IMG_WIDTH;
	int rowRemainder = (int)(_hero->position().y - HERO_LOCATION_SCREEN.y) % IMG_HEIGHT;

	if (!_firstDraw)
	{
		_displayedMap[0] = Sprite::createWithSpriteFrameName(String::createWithFormat("environment_%d_%d.png", row, col)->getCString());
		_displayedMap[1] = Sprite::createWithSpriteFrameName(String::createWithFormat("environment_%d_%d.png", row + 1, col)->getCString());
		_displayedMap[2] = Sprite::createWithSpriteFrameName(String::createWithFormat("environment_%d_%d.png", row + 1, col + 1)->getCString());
		_displayedMap[3] = Sprite::createWithSpriteFrameName(String::createWithFormat("environment_%d_%d.png", row, col + 1)->getCString());
		for (int i = 0; i < 4; i++)
		{
			this->addChild(_displayedMap[i],-100);
			_displayedMap[i]->setAnchorPoint(Point::ZERO);
		}
		_firstDraw = true;
	}
	//if (_lastHeroPosition.x + IMG_WIDTH <= _hero->getPosition().x || _hero->getPosition().x <= _lastHeroPosition.x - IMG_WIDTH ||
	//	_lastHeroPosition.y + IMG_HEIGHT <= _hero->getPosition().y || _hero->getPosition().y <= _lastHeroPosition.y - IMG_HEIGHT)
	//{
		
	/*	_lastHeroPosition = _hero->getPosition();
	}*/

	_displayedMap[0]->setDisplayFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName((String::createWithFormat("environment_%d_%d.png", row, col)->getCString())));
	_displayedMap[1]->setDisplayFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName((String::createWithFormat("environment_%d_%d.png", row + 1, col)->getCString())));
	_displayedMap[2]->setDisplayFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName((String::createWithFormat("environment_%d_%d.png", row + 1, col + 1)->getCString())));
	_displayedMap[3]->setDisplayFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName((String::createWithFormat("environment_%d_%d.png", row, col + 1)->getCString())));

	_displayedMap[0]->setPosition(-1 * colRemainder, -1 * rowRemainder);
	_displayedMap[1]->setPosition(-1 * colRemainder, IMG_HEIGHT - rowRemainder - 1);
	_displayedMap[2]->setPosition(IMG_WIDTH - colRemainder - 1, IMG_HEIGHT - rowRemainder - 1);
	_displayedMap[3]->setPosition(IMG_WIDTH - colRemainder - 1, -1 * rowRemainder);
//end normal map

	//hero
	_hero->skeleton()->setZOrder(400 - _hero->position().y);
	//end

	getVisionEntity();

	return true;
}

void VisionMap::scheduleFunc(float fl)
{
	//vision
	this->drawMap();
	
	//logic
	EMGR->updateAll();

	MSGDISPATCHER->dispatchMsg();
	
}

void VisionMap::getVisionEntity()
{
	auto children = this->getChildren();
	for (auto iter = children.begin(); iter != children.end(); iter++)
	{
		if (!Rect(-IMG_WIDTH, -IMG_HEIGHT, 2 * IMG_WIDTH, 2 * IMG_HEIGHT).containsPoint((*iter)->getPosition()))
		{
			(*iter)->removeFromParent();
		}
	}

	auto entityArr = EMGR->getAllEntity();

	for_each(entityArr.begin(),entityArr.end(),[this](VirtualEntity* entity){
		if (entity->type() == "crocodile")
		{
			auto croco = (Crocodile*)entity;
			auto skeleton = croco->skeleton();
			skeleton->setPosition(
				croco->position() - _hero->position() + HERO_LOCATION_SCREEN
				);
			skeleton->setZOrder(400 - croco->position().y);
			if (skeleton->getBoundingBox().intersectsRect(Rect(0,0,IMG_WIDTH,IMG_HEIGHT)))//图片与摄像机相交
			{
				if (skeleton->getParent()!=this)
					this->addChild(skeleton);
			}
		}
		else if (entity->type() == "plant")
		{
			Plant* plant = (Plant*)entity;
			auto skeleton = plant->skeleton();

			skeleton->setPosition(
				plant->position() - _hero->position() + HERO_LOCATION_SCREEN
				);

			skeleton->setZOrder(400 - plant->position().y);
			if (skeleton->getBoundingBox().intersectsRect(Rect(0, 0, IMG_WIDTH, IMG_HEIGHT)))//图片与摄像机相交
			{
				if (skeleton->getParent() != this)
					this->addChild(skeleton);
			}
		}
		else if (entity->type() == "wood")
		{
			auto wood = (Wood*)entity;
			auto sp = wood->getSprite();

			sp->setPosition(
				wood->position() - _hero->position() + HERO_LOCATION_SCREEN
				);

			sp->setZOrder(400 - wood->position().y);
			if (sp->getBoundingBox().intersectsRect(Rect(0, 0, IMG_WIDTH, IMG_HEIGHT)))//图片与摄像机相交
			{
				if (sp->getParent() != this)
					this->addChild(sp);
			}
		}
		else if (entity->type() == "meat")
		{
			auto meat = (Meat*)entity;
			auto sp = meat->getSprite();

			sp->setPosition(
				meat->position() - _hero->position() + HERO_LOCATION_SCREEN
				);

			sp->setZOrder(400 - meat->position().y);
			if (sp->getBoundingBox().intersectsRect(Rect(0, 0, IMG_WIDTH, IMG_HEIGHT)))//图片与摄像机相交
			{
				if (sp->getParent() != this)
					this->addChild(sp);
			}
		}
	});

	
}

