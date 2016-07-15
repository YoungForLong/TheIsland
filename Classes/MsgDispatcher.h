#pragma once

#pragma warning(disable 4768)
#include<set>
#include<assert.h>
#include<algorithm>
#include<functional>
#include<iostream>
#include<time.h>
#include"Singleton.h"
#include"telegram.h"
#include"VirtualEntity.h"
#include"Entitymanenger.h"
#include"TemplateFunctions.h"


class MsgDispatcher :public Singleton<MsgDispatcher>
{
private:
	~MsgDispatcher(){}
	MsgDispatcher(const MsgDispatcher& msgDispatcher)
	{
		assert(this != &msgDispatcher&&"error:copy source is the same object");

		std::for_each(
			this->_msgQueue.cbegin(),
			msgDispatcher.getQueue().cbegin(),
			[this](Telegram tel)
		{
			this->_msgQueue.insert(tel);
		}
		);
	}
	inline MsgDispatcher& operator=(const MsgDispatcher& msgDispatcher)
	{
		assert(this != &msgDispatcher&&"error:copy source is the same object");
		
		std::for_each(
			this->_msgQueue.cbegin(),
			msgDispatcher.getQueue().cbegin(),
			[this](Telegram tel)
		{
			this->_msgQueue.insert(tel);
		}
		);
		return *this;
	}
private:
	std::set<Telegram> _msgQueue;//ordered by dispatchTime
protected:
	void discharge(VirtualEntity*, const Telegram&);
public:
	MsgDispatcher(){}
public:
#define DEFAULT_DELAY 0.0f 

#define NO_EXTRA_INFO NULL

public:
	void addMsg(long int tiemdelay, int sender, int receiver, int msg, void* extra);
public:
	void dispatchMsg();//executed in MainLoop
	std::set<Telegram> getQueue() const
	{
		return _msgQueue;
	}
};


#define MSGDISPATCHER MsgDispatcher::instance()
