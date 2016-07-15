#include"Hero.h"
#include"Cell_Space_Partition.h"
#include"MsgDispatcher.h"

Hero::Hero():
MovingEntity(HERO_ID,Vec2(500,250),"hero",Vec2::ZERO,Vec2::ZERO,2,100,10,"nilSprite"),
_health(100),
_fullHealth(100),
_sickness(0),
_maxSickness(100),
_hunger(40),
_fullHunger(100),
_touchDis(100)
{
	_actionFSM = new StateMachine<Hero>(this);

	_actionFSM->setCS(HeroActionStop::instance());

	_actionFSM->setGS(NULL);

	init();
}

Hero::~Hero()
{
	clear();
}

void Hero::init()
{
	//碰撞体积
	this->_collisionVolume.circle_.push(Circle(30.0f, Point(0, 0), 0.0f, 360.0f));
	this->_collisionVolume.totalWidth_ = 30.0f;
	this->_collisionVolume.totalHeight_ = 30.0f;
	this->_collisionVolume.anchorPoint_ = mid_mid;

	//骨骼动画
	_heroSkeleton = SkeletonAnimation::createWithFile("Hero_side.json", "Hero_side.atlas", 0.4f);
	
	_heroSkeleton->getSkeleton()->flipX = 0;//default flipX =left
	_heroSkeleton->setDebugSlotsEnabled(false);
	_heroSkeleton->setPosition(500, 250);

	this->sb()->obstacleAvoidanceOn();
}

bool Hero::handleMsg(const Telegram& msg)
{

	/*
	hero_moveUp			= 0x0000,
	hero_moveRight		= 0x0001,
	hero_moveDown		= 0x0002,
	hero_moveLeft		= 0x0003,
	hero_pickOrAttack	= 0x0004,
	hero_standStill		= 0x0005,
	hero_openPocket		= 0x0006,
	animal_attack		= 0x0012
	*/
	if (msg.msg_ == hero_moveUp)
	{
		move(UP);
		return true;
	}

	if (msg.msg_ == hero_moveRight)
	{
		move(RIGHT);
		return true;
	}

	if (msg.msg_ == hero_moveDown)
	{
		move(DOWN);
		return true;
	}

	if (msg.msg_ == hero_moveLeft)
	{
		move(LEFT);
		return true;
	}

	if (msg.msg_ == hero_pickOrAttack)
	{
		pickOrAttack();
		return true;
	}

	if (msg.msg_ == hero_standStill)
	{
		stand();
		return true;
	}

	if (msg.msg_ == animal_attack)
	{
		beAttacked();
		return true;
	}

	return false;
}

double Hero::totalHealth()
{
	double result = 100;

	double hungerWeight = (_hunger / (_fullHunger / 2));
	double sicknessWeight = 1 - (_sickness / _maxSickness);
	double healthWeight = _health / _fullHealth;

	result *= (hungerWeight > 1 ? 1 : hungerWeight);
	result *= sicknessWeight;
	result *= healthWeight;

	return result;
}

bool Hero::pickOrAttack()
{
	//如果没有当前仇恨目标，将会攻击（/采集）最近的实体
	auto enermies = CSP->getNeighbors(this, this->sb()->sightDis());

	if (enermies.size() == 0)
	{
		return false;
	}

	int minCount = 0;
	int count = 0;
	while (count < enermies.size())
	{
		if (enermies.at(count)->position().getDistanceSq(this->position())
			< enermies.at(minCount)->position().getDistanceSq(this->position()))
		{
			minCount = count;
		}
		count++;
	}

	sb()->setTarget((enermies.at(minCount)));
	sb()->pursuitOn(sb()->getTarget());

	Circle heroTouchCircle(_touchDis, position(), 0, 360);

	if (sb()->getTarget()->getCV().intersectsCircle(heroTouchCircle))
	{
		sb()->pursuitOff();//如果移动到了，就停止攻击了
		if (sb()->getTarget()->isBelongTo("product"))//如果是产品就捡起来，是活物就攻击它
		{
			pick();
			return true;
		}
		else
		{
			attack();
			return true;
		}
	}
	return false;
}

void Hero::pick()
{
	MSGDISPATCHER->addMsg(DEFAULT_DELAY, this->id(), sb()->getTarget()->id(), product_picked, NO_EXTRA_INFO);

	MSGDISPATCHER->addMsg(100, this->id(), POCKET_ID, product_picked, sb()->getTarget());
}

void Hero::attack()
{
	Telegram msg(DEFAULT_DELAY, HERO_ID, HERO_ID, hero_pickOrAttack, NO_EXTRA_INFO);

	_actionFSM->handleMsg(msg);

	attackCall();
}

void Hero::stand()
{
	Telegram msg(DEFAULT_DELAY, HERO_ID, HERO_ID, hero_standStill, NO_EXTRA_INFO);

	_actionFSM->handleMsg(msg);

	this->sb()->removeAllBehaviors();
	this->stop();
}

void Hero::beAttacked()
{
	_health -= 20;

	_health = _health <= 0 ? 0 : _health;

	_sickness += 10;

	_sickness = _sickness >= _maxSickness ? _maxSickness : _sickness;
}

void Hero::move(MoveDirection direction)
{
	Vec2 goal = position();
	switch (direction)
	{
	case UP:
		goal += Vec2(0, 10);
		break;
	case RIGHT:
		goal += Vec2(10, 0);
		break;
	case DOWN:
		goal += Vec2(0, -10);
		break;
	case LEFT:
		goal += Vec2(-10, 0);
		break;
	default:
		return;
	}

	sb()->arriveOn(goal);

	//动画
	Telegram msg(DEFAULT_DELAY, HERO_ID, HERO_ID, hero_moveUp, NO_EXTRA_INFO);

	_actionFSM->handleMsg(msg);
}

void Hero::naturallyUpdate()
{
	_hunger -= _fullHunger / HUNGER_EXHAUSTED_DURATION / MAIN_FPS;

	_hunger = _hunger < 0 ? 0 : _hunger;

	if (_hunger <= 1)
	{
		//饥饿时每秒-1的生命值
		_health -= 1 / MAIN_FPS;
	}

	if (_sickness > 0)
	{
		//最坏的情况下，每秒-2的生命值
		_health -= 2 * _sickness / _maxSickness / MAIN_FPS;
	}

	if (_health <= 0)
	{
		MSGDISPATCHER->addMsg(DEFAULT_DELAY, this->id(), GAME_HANDLLER_ID, hero_die, NO_EXTRA_INFO);
	}


}

void Hero::update()
{
	naturallyUpdate();
	MovingEntity::update();
}

void Hero::runAction(actionType type)
{
	if (type==action_walk)
	{
		_heroSkeleton->setAnimation(0, "walk", true);
	}
	else if (type == action_standstill)
	{
		_heroSkeleton->setAnimation(0, "stand", true);
	}
	else if (type == action_attack)
	{
		_heroSkeleton->setAnimation(0, "attack", true);
	}

}