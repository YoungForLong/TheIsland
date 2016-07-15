#pragma once

#include"StateMachine.h"
#include"LivingEntity.h"
#include"EntityManenger.h"



class Animal :public LivingEntity
{
public:
	Animal(int ID, Vec2 position, string entityType, Vec2 ve, Vec2 he, double ma, double mS, double mF, std::string filepath, int fullHealth, int maxSickness,int maxFatigue,Rect habitat);
	virtual ~Animal();

public://system funcs
	virtual bool clear();
	virtual void update();
	virtual void naturallyUpdate();
//public://behaviors
//	virtual void hunt() = 0;
//	virtual void wander() = 0;
//	virtual bool rest() = 0;
//	virtual void beAttacked() = 0;
//	virtual bool eatFood() = 0;
public://gtrs&strs
	const int				hunger()const		{ return _hunger; }
	void					setHunger(int h)	{ _hunger = h; }
	SimpleTree<string>*		foodChain()			{ return _foodChain; }
	Rect					habitat()const		{ return _habitat; }
	bool					shouldHunt()		{ return _hunger < HUNGRY_LIMBO; }
	bool					isTired()			{ return _fatigue > FATIGUE_LIMBO; }
	long int				attackInterval()	{ return _attackInterval; }

protected://animal attributes
	double _hunger;
	double _fullHunger;
	double _fatigue;
	double _maxFatigue;
	Rect _habitat;//¡Ïµÿ
	double _attackInterval;
	static SimpleTree<string>* _foodChain;
};
