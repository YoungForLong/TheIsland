#pragma once
#include"Animal.h"
#include <spine\spine-cocos2dx.h>
#include "spine\spine.h"
#include"MsgDispatcher.h"

using namespace spine;


class Crocodile :public Animal
{
public:
	Crocodile();
	~Crocodile();
	virtual void init();
	ADD_CREATE(Crocodile);

public://system funcs
	virtual bool clear();
	virtual void update();
	virtual bool handleMsg(const Telegram& msg);
	/*virtual void naturallyUpdate();*/

public://behaviors
	void hunt();
	void wander();
	bool rest();
	void beAttacked();
	bool eatFood();
	
public://actions
	void attack();
	void walk();
	void sleep();
	void die();

public://call_funcs
	void actionCall()
	{
		this->_actionTag = true;
	}

	void attackCall()
	{
		actionCall();
		MSGDISPATCHER->addMsg(DEFAULT_DELAY, this->id(), this->sb()->getTarget()->id(), animal_attack, NO_EXTRA_INFO);
	}
public:
	virtual double getBoundingRadius()const
	{
		return 0.5*std::max(_crocodileSkeleton->getBoundingBox().size.height,
			_crocodileSkeleton->getBoundingBox().size.width);
	}

	SkeletonAnimation* skeleton() { return _crocodileSkeleton; }

	StateMachine<Crocodile>* FSM() { return _fsm; }

private:
	SkeletonAnimation* _crocodileSkeleton;
	double _touchDis;
	bool _actionTag;//动画过程中置false
	StateMachine<Crocodile>* _fsm;
	const string product = "meat";
};



