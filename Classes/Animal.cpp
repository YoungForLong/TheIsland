#include"Animal.h"

SimpleTree<string>* Animal::_foodChain = new SimpleTree<string>();


Animal::Animal(int ID, Vec2 position, string entityType, Vec2 ve, Vec2 he, double ma, double mS, double mF, std::string filepath, int fullHealth, int maxSickness,int maxFatigue,Rect habitat) :
LivingEntity(ID,position,entityType,ve,he,ma,mS,mF,filepath,fullHealth,maxSickness),
_habitat(habitat),
_maxFatigue(maxFatigue)
{
	
}

Animal::~Animal()
{
	clear();
}

bool Animal::clear()
{
	return MovingEntity::clear();
}

void Animal::naturallyUpdate()
{
	_hunger -= _fullHunger / HUNGER_EXHAUSTED_DURATION / MAIN_FPS;

	_hunger = _hunger < 0 ? 0 : _hunger;

	if (_hunger <= 1)
	{
		//饥饿时每秒-1的生命值
		_health -= 1 / MAIN_FPS;
	}

	if (_sickness > 0)
	{
		//最坏的情况下，每秒-2的生命值
		_health -= 2 * _sickness / _maxSickness / MAIN_FPS;
	}
}

void Animal::update()
{
	LivingEntity::update();
}
