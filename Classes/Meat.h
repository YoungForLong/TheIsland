#pragma once

#include"MovingEntity.h"

class Meat:public MovingEntity
{
public:
	Meat():
	MovingEntity(getNewID(),Vec2::ZERO,"meat",Vec2::ZERO,Vec2::ZERO,0,0,0,"meat.png")
	{
		
	}
	virtual void init()
	{
		this->_collisionVolume.circle_.push(Circle(20, Vec2(0, 0), 0, 360));
	}

	virtual bool handleMsg(const Telegram& msg)
	{
		if (msg.msg_ == product_picked)
		{
			this->_sprite->removeFromParent();
			return true;
		}
		return false;
	}

	ADD_CREATE(Meat);

	static int getNewID()
	{
		return _meatID++;
	}
private:
	static int _meatID;

};


