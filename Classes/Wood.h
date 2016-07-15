#pragma once

#include"MovingEntity.h"


class Wood :public MovingEntity
{
public:
	Wood() :
		MovingEntity(getNewID(), Vec2::ZERO, "wood", Vec2::ZERO, Vec2::ZERO, 0, 0, 0, "wood.png")
	{

	}
	virtual void init()
	{
		this->_collisionVolume.rect_.push(Rect(-80, 0, 160, 20));
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

	ADD_CREATE(Wood);

	static int getNewID()
	{
		return _woodID++;
	}
private:
	static int _woodID;

};



