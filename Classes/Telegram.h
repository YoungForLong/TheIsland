#pragma once
#include"TemplateFunctions.h"
#include"cocos2d.h"

struct Telegram
{
	long int dispatchTime_;
	int sender_;
	int receiver_;
	MsgType msg_;
	void* extraInfo_;
	bool operator<(const Telegram& telegram)const
	{
		return (this->dispatchTime_ < telegram.dispatchTime_);
	}
	Telegram(long int dispatchTime, int sender, int receiver, MsgType msg, void* extraInfo):
		dispatchTime_(dispatchTime),
		sender_(sender),
		receiver_(receiver),
		msg_(msg),
		extraInfo_(extraInfo)
	{
	
	}
};

//detail msgs

struct BornInfo
{
	int _type;
	cocos2d::Vec2 position_;
};


