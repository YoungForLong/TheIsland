#pragma once
#include"cocos2d.h"
#include"SimpleArray.h"
#include"Circle.h"

using namespace cocos2d;
//��ײ����ṹ��,��Բ�κͷ��λ�С����ƴ��

enum centreAlign
{
	mid_mid,//������
	mid_bottom,//�ײ�����
	left_bottom//���½�
};
class CollisionVolume
{
public:
	//�˴���λ�ö���ָ�����ê���λ��
	SimpleArray<Rect, 20> rect_;
	SimpleArray<Circle, 20> circle_;
	float totalWidth_;
	float totalHeight_;
	bool containsPoint(Point);
	bool intersectsCV(CollisionVolume);
	bool intersectsCircle(Circle& circle);
	centreAlign anchorPoint_;//ê��
};

