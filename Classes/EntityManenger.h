#pragma once

//#pragma warning (disable:4786)

#include<map>
#include<string>
#include"VirtualEntity.h"
#include"Singleton.h"
#include"TemplateFunctions.h"
#include<functional>

class EntityManenger:public Singleton<EntityManenger>
{
private:
	std::map<int, VirtualEntity*> _entityMap;
	EntityManenger(const EntityManenger&);
	EntityManenger& operator=(const EntityManenger&);//cell space partition object
public:
	EntityManenger()
	{
		
	}

	//ID manengement
	void registerEntity(VirtualEntity*);
	VirtualEntity* getEntityByID(int);
	void removeEntity(VirtualEntity*);
	
	//location manengement
	std::vector<VirtualEntity*> getVecByType(string);

	void updateAll();

	vector<VirtualEntity*> getAllEntity() 
	{ 
		vector < VirtualEntity* > entityArr;

		for (auto iter = _entityMap.begin(); iter != _entityMap.end(); iter++)
		{
			entityArr.push_back(iter->second);
		}

		return entityArr;
	}

};

#define EMGR EntityManenger::instance()

#define HERO (EMGR->getEntityByID(HERO_ID))
