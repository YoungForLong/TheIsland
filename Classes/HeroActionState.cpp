#include"HeroActionState.h"
#include"Hero.h"

bool HeroActionRunning::onMessage(Hero* owner, const Telegram& msg)
{
	if (msg.msg_ == hero_standStill)
	{
		owner->actionFSM()->changeState(HeroActionStop::instance());
		return true;
	}

	return false;
}


bool HeroActionStop::onMessage(Hero* owner, const Telegram& msg)
{
	if (msg.msg_ == hero_moveUp ||
		msg.msg_ == hero_moveRight ||
		msg.msg_ == hero_moveDown ||
		msg.msg_ == hero_moveLeft)
	{
		owner->runAction(Hero::actionType::action_walk);
		owner->actionFSM()->changeState(HeroActionRunning::instance());
		return true;
	}
	if (msg.msg_ == hero_pickOrAttack)
	{
		owner->runAction(Hero::actionType::action_attack);
		owner->actionFSM()->changeState(HeroActionRunning::instance());
		return true;
	}
	

	return false;
}