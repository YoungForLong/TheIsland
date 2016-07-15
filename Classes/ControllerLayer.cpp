#include"ControllerLayer.h"
#include"Circle.h"
#include"MsgDispatcher.h"

Point ControllerLayer::_touchPoint = Point::ZERO;
bool ControllerLayer::_isTouched = false;

ControllerLayer::ControllerLayer() :
VirtualEntity(CONTROLLER_ID,"controller"),
_btnArr(NULL)
{
	_pocketOpenCount = 0;
	_pocketOpenTag = false;
	_selectedBtn = 1000;
}

ControllerLayer::~ControllerLayer()
{

}

bool ControllerLayer::init()
{
	do
	{
		if (!Layer::init())
			return false;
		if (!setCache())
			return false;
		if (!setStyle())
			return false;
	} while (0);

	touchListener();

	this->schedule(schedule_selector(ControllerLayer::scheduleFunc), 1 / CONTROLLER_FPS);//帧数必须比RealMap小，否则会造成消息队列事件堆叠
	

	return true;
}

bool ControllerLayer::setStyle()
{
	//btn_move

	_btnArr.pushBack(Sprite::createWithSpriteFrameName("btn_move_bg.png"));//0
	_btnArr.pushBack(Sprite::createWithSpriteFrameName("btn_move_uw.png"));//1
	_btnArr.pushBack(Sprite::createWithSpriteFrameName("btn_move_rw.png"));//2
	_btnArr.pushBack(Sprite::createWithSpriteFrameName("btn_move_dw.png"));//3
	_btnArr.pushBack(Sprite::createWithSpriteFrameName("btn_move_lw.png"));//4
	_btnArr.pushBack(Sprite::createWithSpriteFrameName("btn_move_us.png"));//5
	_btnArr.pushBack(Sprite::createWithSpriteFrameName("btn_move_rs.png"));//6
	_btnArr.pushBack(Sprite::createWithSpriteFrameName("btn_move_ds.png"));//7
	_btnArr.pushBack(Sprite::createWithSpriteFrameName("btn_move_ls.png"));//8
	
	_btnArr.at(0)->setZOrder(BTN_Z_ORDER);

	for (int i = 1; i < 5; i++)
	{
		_btnArr.at(i)->setZOrder(BTN_Z_ORDER + i);
		_btnArr.at(i)->setVisible(false);
	}
	for (int j = 5; j < 9; j++)
	{
		_btnArr.at(j)->setZOrder(BTN_FLOAT_Z_ORDER-1);
		_btnArr.at(j)->setVisible(false);
	}

	for (int k = 0; k < 9; k++)
	{
		_btnArr.at(k)->setAnchorPoint(Point::ZERO);
		_btnArr.at(k)->setPosition(50,50);
	}

	//btn_move end

	//btn_attack
	_btnArr.pushBack(Sprite::createWithSpriteFrameName("btn_attack_pressed.png"));//9
	_btnArr.pushBack(Sprite::createWithSpriteFrameName("btn_attack_halo.png"));//10
	_btnArr.at(9)->setZOrder(BTN_FLOAT_Z_ORDER);
	_btnArr.at(10)->setZOrder(BTN_Z_ORDER);
	_btnArr.at(9)->setAnchorPoint(Point::ZERO);
	_btnArr.at(9)->setPosition(818, 50);
	_btnArr.at(10)->setAnchorPoint(Point::ZERO);
	_btnArr.at(10)->setPosition(818, 50);

	//btn_attack end

	//btn_items
	_btnArr.pushBack(Sprite::createWithSpriteFrameName("btn_items_empty.png"));//11
	_btnArr.pushBack(Sprite::createWithSpriteFrameName("btn_items_halo.png"));//12
	_btnArr.pushBack(Sprite::createWithSpriteFrameName("items_backpack.png"));//13
	_btnArr.at(11)->setZOrder(BTN_FLOAT_Z_ORDER);
	_btnArr.at(12)->setZOrder(BTN_Z_ORDER);
	_btnArr.at(13)->setZOrder(BTN_Z_ORDER + 1);
	
	_btnArr.pushBack(Sprite::createWithSpriteFrameName("btn_items_empty.png"));//14
	_btnArr.pushBack(Sprite::createWithSpriteFrameName("btn_items_halo.png"));//15
	_btnArr.at(14)->setZOrder(BTN_FLOAT_Z_ORDER);
	_btnArr.at(15)->setZOrder(BTN_Z_ORDER);

	_btnArr.pushBack(Sprite::createWithSpriteFrameName("btn_items_empty.png"));//16
	_btnArr.pushBack(Sprite::createWithSpriteFrameName("btn_items_halo.png"));//17
	_btnArr.at(16)->setZOrder(BTN_FLOAT_Z_ORDER);
	_btnArr.at(17)->setZOrder(BTN_Z_ORDER);

	for (int n = 11; n < 18; n++)
	{
		_btnArr.at(n)->setAnchorPoint(Point::ZERO);
	}
	_btnArr.at(11)->setPosition(694, 50);
	_btnArr.at(12)->setPosition(694, 50);
	_btnArr.at(13)->setPosition(694, 50);

	_btnArr.at(14)->setPosition(756, 162);
	_btnArr.at(15)->setPosition(756, 162);

	_btnArr.at(16)->setPosition(868,226);
	_btnArr.at(17)->setPosition(868, 226);

	// btn_items end

	for (int m = 0; m < 18; m++)
	{
		this->addChild(_btnArr.at(m));
	}

	return true;
}

void ControllerLayer::touchListener()
{
	//触摸监控
	_eventListener = EventListenerTouchOneByOne::create();
	_eventListener->onTouchBegan = [&](Touch* touch, Event* event)
	{
		_touchPoint.setPoint(touch->getLocation().x, touch->getLocation().y);
		_isTouched = true;
		return true;
	};
	_eventListener->onTouchEnded = [&](Touch* touch, Event* event)
	{
		if (_selectedBtn==0)
			onMoveEnd();
		_isTouched = false;
	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(_eventListener, this);
}

void ControllerLayer::removeTouchListener()
{
	_eventDispatcher->removeEventListener(_eventListener);
}

bool ControllerLayer::setCache()
{
	Texture2D::setDefaultAlphaPixelFormat(Texture2D::PixelFormat::RGBA4444);
	Texture2D* texture = Director::getInstance()->getTextureCache()->addImage("btn_Apart.png");
	auto spriteBatchNode = SpriteBatchNode::createWithTexture(texture);
	this->addChild(spriteBatchNode);
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("btn_Apart.plist",
		"btn_Apart.png");
	return true;
}

//Image* ControllerLayer::getNewImg(Sprite* img)
//{
//	auto sp = Sprite::createWithSpriteFrame(img->getSpriteFrame());
//	sp->setAnchorPoint(Point::ZERO);
//	auto render = RenderTexture::create(sp->getContentSize().width, sp->getContentSize().height, Texture2D::PixelFormat::RGBA8888);
//	//插入render队列
//	sp->retain();
//	render->beginWithClear(0.0f, 0.0f, 0.0f, 0.0f);
//	sp->visit();
//	render->end();
//	auto spppp = render->getSprite();
//	//执行渲染
//	Director::getInstance()->getRenderer()->render();
//	//render->newImage()->saveToFile((FileUtils::getInstance()->getWritablePath() + "alpha.png"), false);//文件调试
//	//pRender->clear(0, 0, 0, 0);
//	spppp->autorelease();
//	return render->newImage();
//}

void ControllerLayer::selectButton()
{
	//先全部按钮还原
	if (!_isTouched)
	{
		_btnArr.at(1)->setVisible(false);
		_btnArr.at(2)->setVisible(false);
		_btnArr.at(3)->setVisible(false);
		_btnArr.at(4)->setVisible(false);
		_btnArr.at(10)->setVisible(false);
		_btnArr.at(12)->setVisible(false);
	}

	if (_isTouched)
	{
		Circle c1(100, Point(150, 150), 45, 135);
		Circle c2(100, Point(150, 150), 315, 45);
		Circle c3(100, Point(150, 150), 225, 315);
		Circle c4(100, Point(150, 150), 135, 225);
		Circle c10(61, Point(879, 111), 0, 360);
		Circle c11(41, Point(735, 91), 0, 360);
		if (c1.containsPoint(_touchPoint))
		{
			_btnArr.at(1)->setVisible(true);
			move(UP);
			_selectedBtn = 0;
			return;
		}
		if (c2.containsPoint(_touchPoint))
		{
			_btnArr.at(2)->setVisible(true);
			move(RIGHT);
			_selectedBtn = 0;
			return;
		}
		if (c3.containsPoint(_touchPoint))
		{
			_btnArr.at(3)->setVisible(true);
			move(DOWN);
			_selectedBtn = 0;
			return;
		}
		if (c4.containsPoint(_touchPoint))
		{
			_btnArr.at(4)->setVisible(true);
			move(LEFT);
			_selectedBtn = 0;
			return;
		}
		if (c10.containsPoint(_touchPoint))
		{
			_btnArr.at(10)->setVisible(true);
			pickOrAttack();
			_selectedBtn = 0;
			return;
		}
		if (c11.containsPoint(_touchPoint))
		{
			
			_btnArr.at(12)->setVisible(true);
			openPocket();
			_selectedBtn = 2;
			return;
		}
	}
}

void ControllerLayer::move(const MoveDirection direction)
{
	int temp = ILLEGAL_INT;

	switch (direction)
	{
	case UP:
		temp = 0;
		break;
	case RIGHT:
		temp = 1;
		break;
	case DOWN:
		temp = 2;
		break;
	case LEFT:
		temp = 3;
		break;
	default:
		break;
	}

	assert(temp != ILLEGAL_INT&&"ERROR: Wrong direction");

	MSGDISPATCHER->addMsg(DEFAULT_DELAY, id(), HERO_ID, temp, NO_EXTRA_INFO);
}

void ControllerLayer::onMoveEnd()
{
	MSGDISPATCHER->addMsg(DEFAULT_DELAY, id(), HERO_ID, hero_standStill, NO_EXTRA_INFO);
}

void ControllerLayer::pickOrAttack()
{
	MSGDISPATCHER->addMsg(DEFAULT_DELAY, id(), HERO_ID, hero_pickOrAttack, NO_EXTRA_INFO);
}

void ControllerLayer::pause()
{
	MSGDISPATCHER->addMsg(DEFAULT_DELAY, id(), GAME_HANDLLER_ID, game_pause, NO_EXTRA_INFO);
}

void ControllerLayer::openPocket()
{
	MSGDISPATCHER->addMsg(DEFAULT_DELAY, id(), HERO_ID, game_pause, NO_EXTRA_INFO);
}

void ControllerLayer::scheduleFunc(float fl)
{
	//每一帧判断按钮事件
	selectButton();
}

