#pragma once
#include"cocos2d.h"
class Random
{
public:
	static double Rand(int min, int max)
	{
		srand(time(0)*_seedFactor);
		_seedFactor *= 3;
		_seedFactor %= 1000;

		int randNumber = rand();

		int temp = randNumber % int(max - min );

		double reminder = double(randNumber / int(1000))*double(0.001);

		double result = temp + min + reminder;


		return result;
	}
private:
	static int _seedFactor;
};


