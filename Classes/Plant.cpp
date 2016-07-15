#include"Plant.h"
#include"Wood.h"
#include"Random.h"

Plant::Plant() :
LivingEntity(getNewID(), Vec2::ZERO, "bosckage", Vec2::ZERO, Vec2::ZERO, 0, 0, 0, "nilSpreite", 100, 0)
{
	init();
}

Plant::~Plant()
{
	clear();
}

void Plant::init()
{ 
	sb()->removeAllBehaviors();

	_health = 100;
	
	this->_collisionVolume.circle_.push(Circle(10, Point(0, 0), 0, 360));

	_plantSkeleton = SkeletonAnimation::createWithFile("Bosckage.json", "Bosckage.atlas", 0.4f);
	_plantSkeleton->setAnimation(0, "sway_1", false);
	this->_actionTag = false;
	_plantSkeleton->setCompleteListener(CC_CALLBACK_0(Plant::setActionTag,this));
}

bool Plant::clear()
{
	return LivingEntity::clear();
}

void Plant::update()
{
	if (this->_health <= 0)
		die();

	playRandAction();
}

bool Plant::handleMsg(const Telegram& msg)
{
	if (msg.msg_ == animal_beAttacked)
	{
		beAttacked();
		return true;
	}
	return false;
}

void Plant::beAttacked()
{
	auto entry = _plantSkeleton->setAnimation(0, "shake", false);
	
	_actionTag = false;
	_plantSkeleton->setCompleteListener(CC_CALLBACK_0(Plant::setActionTag, this));
	_plantSkeleton->setTrackCompleteListener(entry, CC_CALLBACK_0(Plant::setActionTag, this));
}

void Plant::die()
{
	//产生产品
	auto meat = Wood::create();
	meat->setPosition(this->position());

	//移除当前物体
	this->_plantSkeleton->removeFromParent();
	EMGR->removeEntity(this);
}

void Plant::playRandAction()
{
	if (!_actionTag)
	{
		return;
	}

	int ran = Random::Rand(1, 3);
	_actionTag = false;
	_plantSkeleton->setAnimation(0, String::createWithFormat("sway_%d", ran)->getCString(), false);
	_plantSkeleton->setCompleteListener(CC_CALLBACK_0(Plant::setActionTag, this));
}

