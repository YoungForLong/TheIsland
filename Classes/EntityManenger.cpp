#include"Entitymanenger.h"
#include<assert.h>
#include<algorithm>
VirtualEntity* EntityManenger::getEntityByID(int id)
{
	std::map<int, VirtualEntity*>::const_iterator iter = this->_entityMap.find(id);
	/*assert((iter == _entityMap.end()) && "illegle ID");*/
	return iter->second;
}

void EntityManenger::removeEntity(VirtualEntity* entity)
{
	this->_entityMap.erase(_entityMap.find(entity->id()));

	delete entity;
	entity = NULL;
}

void EntityManenger::registerEntity(VirtualEntity* entity)
{
	this->_entityMap.insert(std::make_pair(entity->id(), entity));
}

std::vector<VirtualEntity*> EntityManenger::getVecByType(string type)
{
	std::vector<VirtualEntity*> vec;

	for_each(_entityMap.cbegin(), _entityMap.cend(), [type,&vec](std::pair<int,VirtualEntity*> entity_pair)
	{
		if (entity_pair.second->type() == type)
		{
			vec.push_back(entity_pair.second);
		}
	}
	);

	return vec;
}

void EntityManenger::updateAll()
{
	for (auto iter = _entityMap.begin(); iter != _entityMap.end(); iter--)//µ¹×Å±éÀú
	{
		(iter->second)->update();
	}
}