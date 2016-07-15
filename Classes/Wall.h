#ifndef _MY_WALL_H_
#define _MY_WALL_H_

#include"cocos2d.h"

using namespace std;

class Telegram;

struct Seg
{
	Seg(Point p1, Point p2):
		_from(p1), _to(p2)
	{

	}
#define eps 1e-6
	//static math functions
	static int sgn(double x)
	{
		return x<-eps ? -1 : (x>eps);
	}

	static double Cross(const Point& p1, const Point& p2, const Point& p3, const Point& p4)
	{
		return (p2.x - p1.x)*(p4.y - p3.y) - (p2.y - p1.y)*(p4.x - p3.x);
	}

	static double Area(const Point& p1, const Point& p2, const Point& p3)
	{
		return Cross(p1, p2, p1, p3);
	}

	static double fArea(const Point& p1, const Point& p2, const Point& p3)
	{
		return fabs(Area(p1, p2, p3));
	}

	static bool Meet(const Point& p1, const Point& p2, const Point& p3, const Point& p4)
	{
		return max(min(p1.x, p2.x), min(p3.x, p4.x)) <= min(max(p1.x, p2.x), max(p3.x, p4.x))
			&& max(min(p1.y, p2.y), min(p3.y, p4.y)) <= min(max(p1.y, p2.y), max(p3.y, p4.y))
			&& sgn(Cross(p3, p2, p3, p4) * Cross(p3, p4, p3, p1)) >= 0
			&& sgn(Cross(p1, p4, p1, p2) * Cross(p1, p2, p1, p3)) >= 0;
	}

	static Point Inter(const Point& p1, const Point& p2, const Point& p3, const Point& p4)
	{
		double s1 = fArea(p1, p2, p3), s2 = fArea(p1, p2, p4);
		return Point((p4.x*s1 + p3.x*s2) / (s1 + s2), (p4.y*s1 + p3.y*s2) / (s1 + s2));
	}

	static double PointToSegDist(double x, double y, double x1, double y1, double x2, double y2)
	{
		double cross = (x2 - x1) * (x - x1) + (y2 - y1) * (y - y1);
		if (cross <= 0) return sqrt((x - x1) * (x - x1) + (y - y1) * (y - y1));

		double d2 = (x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1);
		if (cross >= d2) return sqrt((x - x2) * (x - x2) + (y - y2) * (y - y2));

		double r = cross / d2;
		double px = x1 + (x2 - x1) * r;
		double py = y1 + (y2 - y1) * r;
		return sqrt((x - px) * (x - px) + (py - y1) * (py - y1));
	}
	
	static double PointToSegDist(Vec2 point, Seg line)
	{
		return PointToSegDist(point.x, point.y, line._from.x, line._from.y, line._to.x, line._to.y);
	}

	Point _from;
	Point _to;
};


#endif