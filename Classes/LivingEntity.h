#pragma once

#include"MovingEntity.h"

class LivingEntity :public MovingEntity/* ����Ĺ�������,������ */
{
public:
	LivingEntity(int ID, Vec2 position, string entityType, Vec2 ve, Vec2 he, double ma, double mS, double mF, std::string filepath,int fullHealth,int maxSickness);
	static int getNewID();
protected:
	double _health;
	double _fullHealth;
	double _age;//����ʱ�䣬��ʱ���ã��Ժ�����
	double _sickness;//debuf
	double _maxSickness;//fatal sickness,die immediately
protected:
	static int _livingthingsID;
};

