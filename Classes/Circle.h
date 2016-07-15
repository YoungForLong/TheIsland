#pragma once
#include"cocos2d.h"
#include"math.h"

using namespace cocos2d;
#define PI 3.1415926


class Circle
{
public:
	Circle(float radius,Point center,float startAngle,float endAngle)
	{
		radius_ = radius;
		center_ = center;
		startAngle_ = startAngle;
		endAngle_ = endAngle;
	}

	Circle(const Circle& other)
	{
		this->setCircle(other.radius_, other.center_, other.startAngle_, other.endAngle_);
	}

	void setCircle(float radius, Point center, float sA, float eA)
	{
		radius_ = radius;
		center_ = center;
		startAngle_ = sA;
		endAngle_ = eA;
	}

	bool containsPoint(Point& point)
	{
		int y = point.y - center_.y;
		int x = point.x - center_.x;
		float distance = abs(sqrt(y*y + x*x));
		if (distance < radius_)
		{
			float angle = 0;

			//calculate the angle
			if (x >= 0 && y > 0)
			{
				angle = acos(x / distance) * 180 / PI;
			}
			else if (x < 0 && y >= 0)
			{
				angle = 180 - acos(abs(x / distance)) * 180 / PI;
			}
			else if (x < 0 && y < 0)
			{
				angle = 180 + acos(abs(x / distance)) * 180 / PI;
			}
			else if (x >= 0 && y < 0)
			{
				angle = 360 - acos(abs(x / distance)) * 180 / PI;
			}

			if (startAngle_ >= endAngle_)
			{
				if ((angle < 360 && angle >= startAngle_) || (angle >= 0 && angle < endAngle_))
				{
					return true;
				}
				else return false;
			}
			else
			{
				if (angle >= startAngle_&&angle < endAngle_)
				{
					return true;
				}
				else return false;
			}
		}
		else return false;
	}

	static Circle toTangency(Circle c1,Circle c2)//相切
	{
		float x1 = c1.center_.x;
		float y1 = c1.center_.y;
		float r1 = c1.radius_;
		float x2 = c2.center_.x;
		float y2 = c2.center_.y;
		float r2 = c2.radius_;
		
		float y0 = y2 - abs
			(
			sqrt(
			(r1 + r2)*(r1 + r2) /
			(1 + (x2 - x1)*(x2 - x1) / (y2 - y1)*(y2 - y1))
			)
			);
		float x0 = x2 - (x2 - x1)*(y2 - y0) / (y2 - y1);
		
		Circle c3(
			c1.radius_,
			Point(x0, y0),
			0, 360
			);

		return c3;
	}

	bool isTangency(Circle c1,Circle c2)
	{
		float distance = abs(sqrt((c2.center_.y - c1.center_.y)*((c2.center_.y - c1.center_.y) + (c2.center_.x - c1.center_.x)*(c2.center_.x - c1.center_.x))));
		return distance < (c1.radius_ + c2.radius_) ? true : false;
	}

	bool isIntersectsCircle(Circle circle)
	{
		auto dis = this->center_.Point::distance(circle.center_);

		if (dis < (circle.radius_ + this->radius_))
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	bool isIntersectsRect(Rect rect)//面积重叠
	{
		//互相包含
		//圆包含点
		Point a(rect.getMinX(), rect.getMinY());
		Point b(rect.getMaxX(), rect.getMinY());
		Point c(rect.getMaxX(), rect.getMaxY());
		Point d(rect.getMinX(), rect.getMaxY());
		if (this->containsPoint(a) && this->containsPoint(b) && this->containsPoint(c) && this->containsPoint(d))
		{
			return true;
		}
		//矩形包含圆
		float minA = rect.size.width >= rect.size.height ? rect.size.height : rect.size.width;//较短的边
		if (rect.containsPoint(this->center_) && minA >= this->radius_)
		{
			return true;
		}

		//相交
		Rect NRrect(rect.origin.x + this->radius_, rect.origin.y + this->radius_, rect.size.width - 2 * this->radius_, rect.size.height - 2 * this->radius_);//+R矩形
		Rect PRrect(rect.origin.x - this->radius_, rect.origin.y - this->radius_, rect.size.width + 2 * this->radius_, rect.size.height + 2 * this->radius_);//-R矩形

		if (NRrect.containsPoint(this->center_) || PRrect.containsPoint(this->center_))
		{
			return true;
		}
		
		return false;
	}

	bool isIntersectsTinyRect(Point point)//坐标为Point的小方格
	{
		Rect rect(point.x, point.y, 10, 10);
		if (isIntersectsRect(rect))
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	bool operator ==(Circle& c1)
	{
		return ((radius_ == c1.radius_)&&(center_ == c1.center_)&&(startAngle_ == c1.startAngle_)&&(endAngle_ = c1.endAngle_ ))? true : false;
	}

	static void  eccentic(Point p0/*centre point*/, Point& p1/*outside point*/,float dis)//离心运动
	{
		float d = p1.distance(p0);
		float cos = (p1.x - p0.x) / d;
		float sin = (p1.y - p0.y) / d;
		p1.x += dis*cos;
		p1.y += dis*sin;
	}

	float radius_;
	Point center_;
	float startAngle_;//regard 3 o'clock as the zero angle&anti-clock-wise the positive direction
	float endAngle_;
	//more...
};

#define ORIGN_CIRCLE Circle(0,Point(0,0),0,0)
#define ORIGN_RECT Rect(0,0,0,0)
