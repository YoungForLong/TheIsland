#pragma once

//状态全部为单例，被所有动物对象共享

#include"State.h"
#include"Singleton.h"


class Crocodile;

using namespace YYH_State;

//全局状态
class CrocodileGlobalState :public State < Crocodile >, public Singleton < CrocodileGlobalState >
{
public:
	CrocodileGlobalState(){}

	virtual void enter(Crocodile* owner);

	virtual void execute(Crocodile* owner);

	virtual void exit(Crocodile* owner);

	virtual bool onMessage(Crocodile* onwer, const Telegram& msg);
};

#define CGS CrocodileGlobalState::instance()

//游逛状态，在领地范围内
class WanderInHabitat :public State < Crocodile >, public Singleton < WanderInHabitat >
{
public:
	WanderInHabitat(){}

	virtual void enter(Crocodile* owner);

	virtual void execute(Crocodile* owner);

	virtual void exit(Crocodile* owner);

	virtual bool onMessage(Crocodile* onwer, const Telegram& msg);
};

#define WIH WanderInHabitat::instance()

//捕食状态，会攻击附近的单位
class Prey :public State < Crocodile >, public Singleton < Prey >
{
public:
	Prey(){}

	virtual void enter(Crocodile* owner);

	virtual void execute(Crocodile* owner);

	virtual void exit(Crocodile* owner);

	virtual bool onMessage(Crocodile* onwer, const Telegram& msg);
};

#define PR Prey::instance()

//休息状态
class RestStill :public State < Crocodile >, public Singleton < RestStill >
{
public:
	RestStill(){}

	virtual void enter(Crocodile* owner);

	virtual void execute(Crocodile* owner);

	virtual void exit(Crocodile* owner);

	virtual bool onMessage(Crocodile* onwer, const Telegram& msg);
};

#define RS RestStill::instance()


//吃东西
class EatFood :public State < Crocodile >, public Singleton < EatFood >
{
public:
	EatFood(){}

	virtual void enter(Crocodile* owner);

	virtual void execute(Crocodile* owner);

	virtual void exit(Crocodile* owner);

	virtual bool onMessage(Crocodile* onwer, const Telegram& msg);
};

#define EF EatFood::instance()

