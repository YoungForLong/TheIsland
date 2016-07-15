#pragma once

#include"MovingEntity.h"
#include"spine\spine.h"
#include<spine\spine-cocos2dx.h>
#include"MsgDispatcher.h"
#include"HeroActionState.h"
#include"StateMachine.h"

using namespace spine;

class Hero :public MovingEntity
{
public:
	enum actionType
	{
		action_walk,
		action_standstill,
		action_attack
	};
	Hero();
	~Hero();
	ADD_CREATE(Hero);
	virtual void init();
public://system funcs
	virtual void update();
	virtual bool handleMsg(const Telegram& msg);
public://strs&gtrs
	SkeletonAnimation* skeleton() { return _heroSkeleton; }
	double totalHealth();//总体的健康状态
	StateMachine<Hero>* actionFSM() { return _actionFSM; }
public://behaviors
	bool pickOrAttack();
	void pick();
	void attack();
	void stand();
	void beAttacked();
	void move(MoveDirection direction);
	void naturallyUpdate();
	
public://call back
	void runAction(actionType);

	void attackCall()
	{
		const int attackDelay = 1000;//攻击完成需要1000cpu时间，也就是1s
		auto aimEntity = sb()->getTarget();

		MSGDISPATCHER->addMsg(attackDelay, this->id(), aimEntity->id(), animal_beAttacked, NO_EXTRA_INFO);
	}
private:
	double _health;
	double _fullHealth;
	double _sickness;//debuf
	double _maxSickness;//fatal sickness,die immediately
	double _hunger;
	double _fullHunger;
	SkeletonAnimation* _heroSkeleton;
	double _touchDis;
	StateMachine<Hero>* _actionFSM;
};

