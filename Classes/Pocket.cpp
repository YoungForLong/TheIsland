#include"Pocket.h"

Pocket::Pocket():
VirtualEntity(POCKET_ID,"pocket")
{
	this->setVisible(false);
	this->setTouchEnabled(false);
	_isOpen = false;
}

Pocket::~Pocket()
{

}

bool Pocket::init()
{
	do
	{
		if (!setCache())
			break;
		if (_isOpen)
			break;
		if (!touchListener())
			break;

		//bgImg
		_nullPocket = Sprite::createWithSpriteFrameName("empty_Pocket.png");
		_nullPocket->setAnchorPoint(Point::ZERO);
		_nullPocket->setPosition(300, 50);
		this->addChild(_nullPocket, 0);

		//close btn
		_closeBtn = Sprite::createWithSpriteFrameName("persimmon_btn.png");
		_closeBtn->setScale(0.4);
		_closeBtn->setPosition(690,440);
		this->addChild(_closeBtn, 10);

		//items
		int order = 1;
		int count = 0;
		auto parentPosition = _nullPocket->getPosition();
		while (count < _productsList.size())
		{
			auto sp = _productsList.at(count)->getSprite();
			sp->setAnchorPoint(Point::ZERO);
			int y = order / 4;
			int x = order % 4;
			sp->setPosition(((3 - y) * 100 + parentPosition.y), ((x - 1) * 100 + parentPosition.x));
			this->addChild(sp, 10);
			order++;
			count++;
		}
	
		//schedule
		this->schedule(schedule_selector(Pocket::scheduleFunc), 1/UI_LAYER_FPS);

		return true;
	} while (0);

	return false;
}

bool Pocket::setCache()//test
{
	auto path = FileUtils::getInstance()->fullPathForFilename("empty_Pocket.png");
	auto sf = SpriteFrame::create(path, Rect(0, 0, 400, 400));
	SpriteFrameCache::getInstance()->addSpriteFrame(sf, "empty_Pocket.png");

	auto path1 = FileUtils::getInstance()->fullPathForFilename("persimmon_btn.png");
	auto sf1 = SpriteFrame::create(path1, Rect(0, 0, 150, 150));
	SpriteFrameCache::getInstance()->addSpriteFrame(sf1, "persimmon_btn.png");
	return true;
}

void Pocket::openPocket()
{
	this->setVisible(true);
	this->setTouchEnabled(true);
	refresh();
	_isOpen = true;
}

void Pocket::closePocket()
{
	this->setVisible(false);
	this->setTouchEnabled(false);
	_isOpen = false;
}

bool Pocket::addItems(MovingEntity* item)
{
	_productsList.push_back(item);
	return true;
}

bool Pocket::touchListener()
{
	_eventListener = EventListenerTouchOneByOne::create();
	_eventListener->onTouchBegan = [&](Touch* touch, Event* event)
	{
		_touchPoint.setPoint(touch->getLocation().x, touch->getLocation().y);
		_isTouched = true;
		return true;
	};
	_eventListener->onTouchEnded = [&](Touch* touch, Event* event)
	{
		_isTouched = false;
	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(_eventListener, this);

	return true;
}

void Pocket::removeTouchListener()
{
	_eventDispatcher->removeEventListener(_eventListener);
}

void Pocket::selectCloseBtn()//to be contined!
{
	if (_isTouched)
	{
		if (_closeBtn->getBoundingBox().containsPoint(_touchPoint))
		{
			this->closePocket();
		}
	}
}

void Pocket::scheduleFunc(float fl)
{
	selectCloseBtn();
}

void Pocket::refresh()
{
	//先删除再添加所有的
	this->removeAllChildren();

	//bgImg
	_nullPocket = Sprite::createWithSpriteFrameName("empty_Pocket.png");
	_nullPocket->setAnchorPoint(Point::ZERO);
	_nullPocket->setPosition(300, 50);
	this->addChild(_nullPocket, 0);

	//close btn
	_closeBtn = Sprite::createWithSpriteFrameName("persimmon_btn.png");
	_closeBtn->setScale(0.4);
	_closeBtn->setPosition(690, 440);
	this->addChild(_closeBtn, 10);

	//items
	int order = 1;
	int count = 0;
	auto parentPosition = _nullPocket->getPosition();
	while (count < _productsList.size())
	{
		//test
		auto pro = _productsList.at(count);
		Sprite* sp;
		if (pro->type() == "wood")
		{
			sp = Sprite::create("wood_2.png");
			sp->setScale(0.4f);
		}
		else if (pro->type() == "meat")
		{
			sp = Sprite::create("meat.png");
			sp->setScale(0.4f);
		}
		sp->setAnchorPoint(Point::ZERO);
		int y = order / 4;
		int x = order % 4;
		sp->setPosition(((3 - y) * 100 + parentPosition.y), ((x - 1) * 100 + parentPosition.x));
		this->addChild(sp, 10);
		order++;
		count++;
	}
}

bool Pocket::clear()
{
	_productsList.clear();
	_productsSprites.clear();
	return true;
}

bool Pocket::handleMsg(const Telegram& msg)
{
	if (msg.msg_ == hero_openPocket)
	{
		this->openPocket();
		return true;
	}

	return false;
}