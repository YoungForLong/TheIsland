#include"LivingEntity.h"

int LivingEntity::_livingthingsID = 100;//��100��1000��ʼ�����ʵ���id

LivingEntity::LivingEntity(int ID, Vec2 position, string entityType, Vec2 ve, Vec2 he, double ma, double mS, double mF, std::string filepath, int fullHealth, int maxSickness) :
MovingEntity(ID, position, entityType, ve, he, ma, mS, mF, filepath),
_fullHealth(fullHealth),
_maxSickness(maxSickness)
{

}

int LivingEntity::getNewID()
{
	return _livingthingsID++;
}
