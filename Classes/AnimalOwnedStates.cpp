#include"AnimalOwnedStates.h"
#include"Crocodile.h"
#include"EntityManenger.h"
#include"Cell_Space_Partition.h"

/*------全局状态 CrocodileGlobalState------*/

void CrocodileGlobalState::enter(Crocodile* onwer)
{
	//nil
}

void CrocodileGlobalState::execute(Crocodile* owner)
{
	owner->naturallyUpdate();
}

void CrocodileGlobalState::exit(Crocodile* owner)
{
	
}

bool CrocodileGlobalState::onMessage(Crocodile* owner, const Telegram& msg)
{
	if (msg.msg_ == animal_beAttacked)
	{
		owner->sb()->setTarget((MovingEntity*)(EMGR->getEntityByID(msg.sender_)));
		owner->beAttacked();
		owner->FSM()->changeState(PR);
		return true;
	}

	if (msg.msg_ == time_night)
	{
		owner->FSM()->changeState(RS);
	}
	return false;
}

/*------游荡WanderInHabitat------*/

void WanderInHabitat::enter(Crocodile* owner)
{
	owner->sb()->wanderOn();

	Vec2 po = (owner->habitat().origin + Vec2(owner->habitat().getMaxX(), owner->habitat().getMaxY())) / 2;
	owner->sb()->arriveOn(po);

	owner->sb()->obstacleAvoidanceOn();

	owner->sb()->flockingOn();

	/*owner->sb()->hideOn();*/
}

void WanderInHabitat::execute(Crocodile* owner)
{
	owner->wander();
	if (owner->shouldHunt())
	{
		owner->FSM()->changeState(PR);
	}
	if (owner->isTired())
	{
		owner->FSM()->changeState(RS);
	}
}

void WanderInHabitat::exit(Crocodile* owner)
{
	owner->sb()->removeAllBehaviors();
}

bool WanderInHabitat::onMessage(Crocodile* owner, const Telegram& msg)
{
	return false;
}

/*------捕食行为Prey------*/
void Prey::enter(Crocodile* owner)
{
	//添加运动行为

	owner->sb()->obstacleAvoidanceOn();

	owner->sb()->pursuitOn(owner->sb()->getTarget());
}

void Prey::execute(Crocodile* owner)
{
	owner->hunt();
}

void Prey::exit(Crocodile* owner)
{
	owner->sb()->removeAllBehaviors();
}

bool Prey::onMessage(Crocodile* owner, const Telegram& msg)
{
	if (msg.msg_ == animal_dided)
	{
		owner->FSM()->changeState(EF);
		/*owner->setGain((MovingEntity*)EMGR->getEntityByID(msg.sender_)->)*/
		return true;
	}

	return false;
}


/*------休息状态RestStill-------*/

void RestStill::enter(Crocodile* owner)
{

}

void RestStill::execute(Crocodile* owner)
{
	if (owner->rest())
	{
		owner->FSM()->changeState(WIH);
	}
}

void RestStill::exit(Crocodile* owner)
{
	owner->sb()->removeAllBehaviors();
}

bool RestStill::onMessage(Crocodile* owner, const Telegram& msg)
{
	return false;
}


/*------吃东西EatFood------*/
void EatFood::enter(Crocodile* owner)
{
	
}

void EatFood::execute(Crocodile* owner)
{
	if (owner->eatFood())
	{
		owner->FSM()->changeState(WIH);
	}
}

void EatFood::exit(Crocodile* owner)
{
	owner->sb()->removeAllBehaviors();
}

bool EatFood::onMessage(Crocodile* owner, const Telegram& msg)
{
	return false;
}

