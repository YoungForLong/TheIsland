#pragma once
#include"cocos2d.h"
#include"SimpleArray.h"
#include"Circle.h"

using namespace cocos2d;
//碰撞体积结构体,由圆形和方形或小方格拼接

enum centreAlign
{
	mid_mid,//正中心
	mid_bottom,//底部中心
	left_bottom//左下角
};
class CollisionVolume
{
public:
	//此处的位置都是指相对于锚点的位置
	SimpleArray<Rect, 20> rect_;
	SimpleArray<Circle, 20> circle_;
	float totalWidth_;
	float totalHeight_;
	bool containsPoint(Point);
	bool intersectsCV(CollisionVolume);
	bool intersectsCircle(Circle& circle);
	centreAlign anchorPoint_;//锚点
};

