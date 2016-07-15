#ifndef _HERO_ACTION_STATE_H_
#define _HERO_ACTION_STATE_H_
#include"State.h"
#include"Singleton.h"

using namespace YYH_State;
class Hero;

class HeroActionRunning :public State<Hero>,public Singleton < HeroActionRunning >
{
public:
	HeroActionRunning() {}

	virtual void enter(Hero* owner) {}

	virtual void execute(Hero* owner) {}

	virtual void exit(Hero* owner) {}

	virtual bool onMessage(Hero* owner, const Telegram& msg);
};


class HeroActionStop :public State<Hero>, public Singleton < HeroActionStop >
{
public:
	HeroActionStop() {}

	virtual void enter(Hero* owner) {}

	virtual void execute(Hero* owner) {}

	virtual void exit(Hero* owner) {}

	virtual bool onMessage(Hero* owner, const Telegram& msg);
};

#endif
