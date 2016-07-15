#include"Crocodile.h"
#include"Random.h"
#include"Cell_Space_Partition.h"
#include"Meat.h"
#include"AnimalOwnedStates.h"

Crocodile::Crocodile():
Animal(
	getNewID(),
	Vec2(Random::Rand(0, WHOLE_MAP_WIDTH), Random::Rand(0, WHOLE_MAP_HEIGHT)),
	string("crocodile"),
	Vec2::ZERO,
	Vec2::ZERO,
	10,
	100,
	40,
	string("nilSprite"),
	100,
	100,
	100,
	Rect(0, 0, 2000, 2000)
)
{
	_fsm = new StateMachine<Crocodile>(this);
	_fsm->setCS(WIH);
	_fsm->setGS(CGS);
	init();
}

Crocodile::~Crocodile()
{
	this->clear();
}

void Crocodile::init()
{
	_hunger = 40;
	_fatigue = 0;

	_attackInterval = 1000;//cpu时间，1000为1s

	_actionTag = true;

	//碰撞体积
	Circle c1(30, Point(-70, 30), 0, 360);
	Circle c2(30, Point(70, 30), 0, 360);
	_collisionVolume.circle_.push(c1);
	_collisionVolume.circle_.push(c2);
	Rect r1(-70, 0, 140, 60);
	_collisionVolume.rect_.push(r1);
	_collisionVolume.totalHeight_ = 60;
	_collisionVolume.totalWidth_ = 230;

	_touchDis = 100;

	_crocodileSkeleton = SkeletonAnimation::createWithFile("crocodile.json", "crocodile.atlas", 0.2f);

	_actionTag = false;
	_crocodileSkeleton->setAnimation(0, "default_1", false);
	_crocodileSkeleton->setCompleteListener(CC_CALLBACK_0(Crocodile::actionCall, this));
}

void Crocodile::attack()
{
	if (!_actionTag)
	{
		return;
	}
	else
	{
		_actionTag = false;
		auto trackEntry=_crocodileSkeleton->setAnimation(0, "bite", false);
		_crocodileSkeleton->setTrackCompleteListener(trackEntry, CC_CALLBACK_0(Crocodile::attackCall, this));
	}
}

void Crocodile::walk()
{
	if (!_actionTag)
	{
		return;
	}
	else
	{
		_actionTag = false;
		auto trackEntry = _crocodileSkeleton->setAnimation(0, "walk", false);
		_crocodileSkeleton->setTrackCompleteListener(trackEntry, CC_CALLBACK_0(Crocodile::actionCall, this));
	}
}

void Crocodile::sleep()
{
	if (!_actionTag)
	{
		return;
	}
	else
	{
		_actionTag = false;
		auto trackEntry = _crocodileSkeleton->setAnimation(0, "sleep", false);
		_crocodileSkeleton->setTrackCompleteListener(trackEntry, CC_CALLBACK_0(Crocodile::actionCall, this));
	}
}

void Crocodile::die()
{
	//产生产品
	auto meat = Meat::create();
	meat->setPosition(this->position());

	//移除当前物体
	this->_crocodileSkeleton->removeFromParent();
	EMGR->removeEntity(this);
}

void Crocodile::hunt()
{
	auto foodEntity = this->sb()->getTarget();
	CollisionVolume targetCv = foodEntity->getCV();

	Circle circle = Circle(_touchDis, _position, 0, 360);

	if (targetCv.intersectsCircle(circle))
	{
		attack();
	}
}

void Crocodile::wander()
{
	walk();
}

bool Crocodile::rest()
{
	sleep();
	
	//睡觉会治疗伤势
	_sickness -= _maxSickness / SICKNESS_RUN_OUT_DURATION / MAIN_FPS;

	_sickness = _sickness > 0 ? _sickness : 0;

	//睡觉会减少疲劳值
	_fatigue -= _maxSickness / FATIGUE_REGENERATION_DURATION / MAIN_FPS;
	
	if (_fatigue<=0)
	{
		_fatigue = 0;
		return true;
	}

	return false;
}

void Crocodile::beAttacked()
{
	_health -= 20;

	_health = _health <= 0 ? 0 : _health;

	_sickness += 10;

	_sickness = _sickness >= _maxSickness ? _maxSickness : _sickness;
}

bool Crocodile::eatFood()
{
	auto arr = CSP->getNeighbors(this, _touchDis);
	MovingEntity* food = NULL;
	for_each(arr.begin(), arr.end(),[&food](MovingEntity* product){
		if (product->isBelongTo("food"))
		{
			food = product;
		}
	});

	MSGDISPATCHER->addMsg(DEFAULT_DELAY, this->id(), food->id(),food_ate, NO_EXTRA_INFO);
	return true;
}

void Crocodile::update()
{
	Animal::update();

	_fsm->update();

	if (this->_health==0)
	{
		this->die();
	}

	//同步朝向，地图上位置的更新将放在可视化地图上来处理
	_crocodileSkeleton->getSkeleton()->flipX = _heading > 0 ? 1 : 0;
}

bool Crocodile::handleMsg(const Telegram& msg)
{
	return _fsm->handleMsg(msg);
}

bool Crocodile::clear()
{
	delete _fsm;
	_fsm = NULL;
	return true;
}