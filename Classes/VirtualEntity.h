#pragma once
#include"SimpleTree.h"
#include"Telegram.h"

using namespace std;

class VirtualEntity
{
public://ctors
	VirtualEntity(int id, string type) :
		_id(id),
		_type(type)
	{

	}

	
	virtual ~VirtualEntity()
	{

	}
	
public:
	virtual bool handleMsg(const Telegram& msg) { return false; }//所有单例实体都需要重写，用于处理消息

	virtual bool clear() { return false; }//所有虚拟实体都要调用，用于释放内存

	virtual void update() {}//部分需要重写该函数以实现主动更新功能

	bool isinclude(VirtualEntity* other)//是否属于某实体
	{
		vector<string> myTypeTrackEntry = _all_entity_types->track(_type);

		for (auto iter = myTypeTrackEntry.begin(); iter != myTypeTrackEntry.end(); iter++)
		{
			if ((*iter) == other->type())
			{
				return true;
			}
		}

		return false;
	}

	bool isBelongTo(string otherType)//是否属于某类
	{
		vector<string> myTypeTrackEntry = _all_entity_types->track(_type);

		for (auto iter = myTypeTrackEntry.begin(); iter != myTypeTrackEntry.end(); iter++)
		{
			if ((*iter) == otherType)
			{
				return true;
			}
		}

		return false;
	}

	static SimpleTree<string>* typeTree()
	{
		return _all_entity_types;
	}

public://gtrs&strs
	const int id() { return _id; }
	const string type() { return _type; }

protected:
	int _id;
	string _type;
	static SimpleTree<string>* _all_entity_types;//所有的实体类型保存在类型树中，访问它以获得包含关系
};



