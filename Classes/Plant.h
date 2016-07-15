#pragma once
#include"LivingEntity.h"
#include<spine\spine-cocos2dx.h>
#include"spine\spine.h"

using namespace spine;

class Plant :public LivingEntity
{
public:
	Plant();
	virtual ~Plant();
	virtual void init();
	ADD_CREATE(Plant);
public:
	virtual bool clear();
	virtual void update();
	virtual bool handleMsg(const Telegram& msg);
public:
	void beAttacked();
	void die();
	void playRandAction();

	void setActionTag(){
		this->_actionTag = true;
	}

	void healthChange()
	{
		this->_health -= 20;
		_health = _health < 0 ? 0 : _health;
	}
public:
	SkeletonAnimation* skeleton() { return _plantSkeleton; }
private:
	SkeletonAnimation* _plantSkeleton;
	bool _actionTag;

	const string _product = "wood";
};




