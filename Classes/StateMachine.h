#pragma once
#include<assert.h>
#include"State.h"


using namespace std;
using namespace YYH_State;

template<class Type>
class StateMachine
{
private:
	State<Type>* _currentState;
	State<Type>* _previousState;
	State<Type>* _globalState;
	Type* _owner;
public:
	StateMachine(Type*);
	~StateMachine();
public:
	void setCS(State<Type>*);
	void setPS(State<Type>*);
	void setGS(State<Type>*);
	State<Type>* getCS() const;
	State<Type>* getPS() const;
	State<Type>* getGS() const;
public:
	void update() const;

	bool handleMsg(const Telegram&);

	void changeState(State<Type>*);

	bool isStateOn(State<Type>& State)const;

	void revert2PreviousState();
};

template<class Type>
StateMachine<Type>::StateMachine(Type* owner) :_currentState(NULL),
_previousState(NULL),
_globalState(NULL),
_owner(owner)
{

}

template<class Type>
StateMachine<Type>::~StateMachine()
{

}

template<class Type>
void StateMachine<Type>::setCS(State<Type>* cs)
{
	_currentState = cs;
}

template<class Type>
void StateMachine<Type>::setGS(State<Type>* gs)
{
	_globalState = gs;
}

template<class Type>
void StateMachine<Type>::setPS(State<Type>* ps)
{
	_previousState = ps;
}

template<class Type>
State<Type>* StateMachine<Type>::getCS()const
{
	return _currentState;
}

template<class Type>
State<Type>* StateMachine<Type>::getGS()const
{
	return _globalState;
}

template<class Type>
State<Type>* StateMachine<Type>::getPS()const
{
	return _previousState;
}

template<class Type>
void StateMachine<Type>::update()const
{
	if (_globalState)
		_globalState->execute(_owner);
	if (_currentState)
		_currentState->execute(_owner);
}

template<class Type>
bool StateMachine<Type>::handleMsg(const Telegram& tel)
{
	if (_currentState&&_currentState->onMessage(_owner, tel))
		return true;
	if (_globalState&&_globalState->onMessage(_owner, tel))
		return true;

	return false;
}

template<class Type>
void StateMachine<Type>::changeState(State<Type>* newState)
{
	assert(newState&&"a null State,tring to changeState falied");
	_previousState = _currentState;
	_currentState->exit(_owner);
	_currentState = newState;
	_currentState->enter(_owner);
}

template<class Type>
bool StateMachine<Type>::isStateOn(State<Type>& State)const
{
	return typeid(*_currentState) == typeid(State);
}

template<class Type>
void StateMachine<Type>::revert2PreviousState()
{
	changeState(_previousState);
}
