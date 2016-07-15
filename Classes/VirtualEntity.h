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
	virtual bool handleMsg(const Telegram& msg) { return false; }//���е���ʵ�嶼��Ҫ��д�����ڴ�����Ϣ

	virtual bool clear() { return false; }//��������ʵ�嶼Ҫ���ã������ͷ��ڴ�

	virtual void update() {}//������Ҫ��д�ú�����ʵ���������¹���

	bool isinclude(VirtualEntity* other)//�Ƿ�����ĳʵ��
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

	bool isBelongTo(string otherType)//�Ƿ�����ĳ��
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
	static SimpleTree<string>* _all_entity_types;//���е�ʵ�����ͱ������������У��������Ի�ð�����ϵ
};



