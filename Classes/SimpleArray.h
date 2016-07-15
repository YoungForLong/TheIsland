#pragma once
#include<stdlib.h>

using namespace std;

template<class Type, int MaxLenth>
class SimpleArray
{
public:
	SimpleArray();
	~SimpleArray();
	bool push(Type);
	Type& pop();//pop出来的元素不能为地址，因为地址已经置空
	bool clear();
	Type& at(int);//from zero
	bool isEmpty();
	bool isFull();//这个的意思是是否还有一个元素的空间
	inline int lenth()
	{
		return _currentLenth;
	}
	inline Type operator[](int number_)
	{
		return this->at(number_);
	}
private:
	Type* _elementPtr;
	const int _maxLenth;
	int _currentLenth;
	int _currentRoom;
};

template<class Type, int MaxLenth>
SimpleArray<Type, MaxLenth>::SimpleArray() :
 _maxLenth(MaxLenth)
{
	_currentLenth = 0;
	_currentRoom = MaxLenth;
	_elementPtr = (Type*)malloc(sizeof(Type)*_currentRoom);
}

template<class Type, int MaxLenth>
SimpleArray<Type, MaxLenth>::~SimpleArray()
{
	
}

template<class Type, int MaxLenth>
bool SimpleArray<Type, MaxLenth>::push(Type element_)
{
	if (MaxLenth < _currentLenth)
	{
		return false;
	}
	if (this->isFull())
	{
		auto lastLenth = _currentRoom;
		_currentRoom += 10;
		auto pRet = (Type*)malloc(sizeof(Type)*(_currentRoom));//多分配10个元素的空间
		for (int i = 0; i <lastLenth; i++)
		{
			pRet[i] = _elementPtr[i];
		}
		free(_elementPtr);
		_elementPtr = pRet;
	}
	_elementPtr[_currentLenth] = element_;
	_currentLenth++;
	return true;
}

template<class Type, int MaxLenth>
Type& SimpleArray<Type, MaxLenth>::pop()
{
	assert(!isEmpty());
	_currentLenth--;
	return _elementPtr[_currentLenth];
}

template<class Type, int MaxLenth>
Type& SimpleArray<Type, MaxLenth>::at(int number)
{
	assert(!(number<0 || number>_currentRoom - 1));
	return _elementPtr[number];
}

template<class Type, int MaxLenth>
bool SimpleArray<Type, MaxLenth>::isEmpty()
{
	if (_currentLenth == 0)
	{
		return true;
	}
	return false;
}

template<class Type, int MaxLenth>
bool SimpleArray<Type, MaxLenth>::isFull()
{
	if (_currentLenth == _currentRoom)
	{
		return true;
	}
	return false;
}

template<class Type,int MaxLenth>
bool SimpleArray<Type, MaxLenth>::clear()
{
	if (!_elementPtr)
	{
		free(_elementPtr);
		_elementPtr = NULL;
	}
	auto ret = (Type*)malloc(sizeof(Type)*(_currentRoom));
	_elementPtr = ret;
	_currentLenth = 0;
	return true;
}

