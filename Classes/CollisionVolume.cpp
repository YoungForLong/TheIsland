#include"CollisionVolume.h"

bool CollisionVolume::containsPoint(Point point)
{
	int count = 0;
	while (count < circle_.lenth())
	{
		if (circle_[count].containsPoint(point))
			return true;
		count++;
	}

	count = 0;
	while (count < rect_.lenth())
	{
		if (rect_[count].containsPoint(point))
			return true;
		count++;
	}

	return false;
}

bool CollisionVolume::intersectsCV(CollisionVolume CV)
{
	int count = 0;
	while (count < circle_.lenth())
	{
		int count1 = 0;
		while (count1 < CV.circle_.lenth())
		{
			if (this->circle_[count].isIntersectsCircle(CV.circle_[count1]))
			{
				return true;
			}
			count1++;
		}
		count1 = 0;
		while (count1 < CV.rect_.lenth())
		{
			if (this->circle_[count].isIntersectsRect(CV.rect_[count1]))
			{
				return true;
			}
			count1++;
		}
		count++;
	}

	count = 0;
	while (count < rect_.lenth())
	{
		int count1 = 0;
		while (count1 < CV.circle_.lenth())
		{
			if (CV.circle_[count1].isIntersectsRect(this->rect_[count]))
			{
				return true;
			}
			count1++;
		}
		
		count1 = 0;
		while (count1 < CV.rect_.lenth())
		{
			if (CV.rect_[count1].intersectsRect(this->rect_[count]))
			{
				return true;
			}
			count1++;
		}
		count++;
	}

	return false;

}

bool CollisionVolume::intersectsCircle(Circle& circle)
{
	int count = 0;
	while (count < rect_.lenth())
	{
		if (circle.isIntersectsRect(rect_[count]))
		{
			return true;
		}
		count++;
	}

	count = 0;
	while (count < circle_.lenth())
	{
		if (circle.isIntersectsCircle(circle_[count]))
		{
			return true;
		}
		count++;
	}

	return false;
}
