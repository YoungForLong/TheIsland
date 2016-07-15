#pragma once
#include"cocos2d.h"

using namespace cocos2d;

class MovingEntity;

class SteeringBehaviors
{
private:
	enum behavior_type
	{
		NONE				= 0x00000,
		SEEK				= 0x00002,
		FLEE				= 0x00004,
		ARRIVE				= 0x00008,
		WANDER				= 0x00010,
		COHESION			= 0x00020,
		SEPARATION			= 0x00040,
		ALLIGNMENT			= 0x00080,
		OBSTACLE_AVOIDANCE	= 0x00100,
		PURSUIT				= 0x00800,
		EVADE				= 0x01000,
		HIDE				= 0x04000,
		FLOCK				= 0x08000,
		OFFSET_PURSUIT		= 0x10000,
	};

	enum Deceleration
	{
		slow = 3,
		normal = 2,
		fast = 1
	};

public://ctors
	SteeringBehaviors(MovingEntity*);
	~SteeringBehaviors() {}
	
private://behavior attributes

	Vec2 _totalSF;//经过计算得出的合力，用于输出

	MovingEntity* _ownerMovingEntity;

	Deceleration _deceleration;//为arrive方法提供减速参数

	MovingEntity* _target;//目标，同时只能为攻击目标，逃离目标或者群体的领导者

	//weight of all behaviors，操控力的权值
	double        _weightSeparation;
	double        _weightCohesion;
	double        _weightAlignment;
	double        _weightWander;
	double        _weightObstacleAvoidance;
	double        _weightSeek;
	double        _weightFlee;
	double        _weightArrive;
	double        _weightPursuit;
	double        _weightOffsetPursuit;
	double        _weightHide;
	double        _weightEvade;

	Vec2 _wanderTarget;

	double _wanderRadius;//半径，和抖动共同控制转动方向

	double _wanderDistance;//距离，变相地代表了随机运动的力大小

	double _wanderJitter;//抖动，控制每帧的转动半径

	double _dBoxLength;//检测盒半径

	//hide attrs
	double _sightDis;

	//multi States
	int _behaviorFlags;

	//offset
	Vec2 _offset;

	//positionTarget
	Vec2 _poTarget;

private:
	Vec2 getHidePosition(MovingEntity*, const Vec2);

	/*   -----------------------------------------------------------------------

	Behavior Functions

	-----------------------------------------------------------------------   */

	/*---------------------------single method---------------------------*/
	Vec2 seek(const Vec2);

	Vec2 flee(const Vec2);

	Vec2 arrive(const Vec2, Deceleration);

	Vec2 pursuit(MovingEntity*);

	Vec2 evade(MovingEntity*);

	Vec2 wander();

	Vec2 interpose(MovingEntity*, MovingEntity*);

	/*---------------------------obstacle method---------------------------*/
	Vec2 obstacleAvoidance(std::vector<MovingEntity*>&);

	/*Vec2 hide(MovingEntity*,std::vector<MovingEntity*>);*/

	/*---------------------------group method---------------------------*/
	//Vec2 offsetPursuit(MovingEntity*, const Vec2);

	Vec2 separation(std::vector<MovingEntity*>);

	Vec2 alignment(std::vector<MovingEntity*>);

	Vec2 cohesion(std::vector<MovingEntity*>);
	//返回加速度
public:
	/*-------------------------- combine force --------------------------*/

	Vec2 calculate();

	Vec2 calculatePrioritizedWighted(std::vector<MovingEntity*>);

	bool accumulateForce(Vec2&, Vec2);//larger the Priority of steering force is,larger the effect is

	/*----------------------------- on ---------------------------------*/

	bool on(behavior_type type){ return (_behaviorFlags&type) == type; }

	void removeAllBehaviors()	{ _behaviorFlags = 0; }

	void seekOn(const Vec2 target){ _behaviorFlags |= SEEK; _poTarget = target; }
	void fleeOn(const Vec2 target){ _behaviorFlags |= FLEE; _poTarget = target; }
	void arriveOn(const Vec2 target){ _behaviorFlags |= ARRIVE; _poTarget = target; }
	void wanderOn(){ _behaviorFlags |= WANDER; }
	void pursuitOn(MovingEntity* ve){ _behaviorFlags |= PURSUIT; _target = ve; }
	void evadeOn(MovingEntity* ve){ _behaviorFlags |= EVADE; _target = ve; }
	void cohesionOn(){ _behaviorFlags |= COHESION; }
	void separationOn(){ _behaviorFlags |= SEPARATION; }
	void alignmentOn(){ _behaviorFlags |= ALLIGNMENT; }
	void obstacleAvoidanceOn(){ _behaviorFlags |= OBSTACLE_AVOIDANCE; }
	void hideOn(MovingEntity* v){ _behaviorFlags |= HIDE; _target = v; }
	void offsetPursuitOn(MovingEntity* v1, const Vec2 offset){ _behaviorFlags |= OFFSET_PURSUIT; _offset = offset; _target = v1; }
	void flockingOn(){ cohesionOn(); alignmentOn(); separationOn(); wanderOn(); }

	/*----------------------------- off --------------------------------*/
	void fleeOff()  { if (on(FLEE))   _behaviorFlags ^= FLEE; }
	void seekOff()  { if (on(SEEK))   _behaviorFlags ^= SEEK; }
	void arriveOff(){ if (on(ARRIVE)) _behaviorFlags ^= ARRIVE; }
	void wanderOff(){ if (on(WANDER)) _behaviorFlags ^= WANDER; }
	void pursuitOff(){ if (on(PURSUIT)) _behaviorFlags ^= PURSUIT; }
	void evadeOff(){ if (on(EVADE)) _behaviorFlags ^= EVADE; }
	void cohesionOff(){ if (on(COHESION)) _behaviorFlags ^= COHESION; }
	void separationOff(){ if (on(SEPARATION)) _behaviorFlags ^= SEPARATION; }
	void alignmentOff(){ if (on(ALLIGNMENT)) _behaviorFlags ^= ALLIGNMENT; }
	void obstacleAvoidanceOff(){ if (on(OBSTACLE_AVOIDANCE)) _behaviorFlags ^= OBSTACLE_AVOIDANCE; }
	void hideOff(){ if (on(HIDE)) _behaviorFlags ^= HIDE; }
	void offsetPursuitOff(){ if (on(OFFSET_PURSUIT)) _behaviorFlags ^= OFFSET_PURSUIT; }
	void flockingOff(){ cohesionOff(); alignmentOff(); separationOff(); wanderOff(); }

	/*----------------------------- isOn --------------------------------*/
	bool isFleeOn(){ return on(FLEE); }
	bool isSeekOn(){ return on(SEEK); }
	bool isArriveOn(){ return on(ARRIVE); }
	bool isWanderOn(){ return on(WANDER); }
	bool isPursuitOn(){ return on(PURSUIT); }
	bool isEvadeOn(){ return on(EVADE); }
	bool isCohesionOn(){ return on(COHESION); }
	bool isSeparationOn(){ return on(SEPARATION); }
	bool isAlignmentOn(){ return on(ALLIGNMENT); }
	bool isObstacleAvoidanceOn(){ return on(OBSTACLE_AVOIDANCE); }
	bool isHideOn(){ return on(HIDE); }
	bool isOffsetPursuitOn(){ return on(OFFSET_PURSUIT); }

	//strs&gtrs
	void setTarget(MovingEntity* target)
	{
		this->_target = target;
	}

	MovingEntity* getTarget()
	{
		return _target;
	}

	double sightDis()const { return _sightDis; }
};



