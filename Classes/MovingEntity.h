#pragma once

#include"VirtualEntity.h"
#include"CollisionVolume.h"
#include"SteeringBehaviors.h"
#include"TemplateFunctions.h"
#include"EntityManenger.h"

#define CREATE_ENTITY(__ENTITYTYPE__) ([](){ return __ENTITYTYPE__::create();})


//检查是否是完整类（避免抽象类被实现从而导致错误）,然后使用工厂函数创建类
#define ADD_CREATE(__TYPE__) \
	static __TYPE__* create() \
{ \
if (IsAbstract<__TYPE__>::Result) \
{ \
	assert(0 && "abstract class"); \
	return NULL; \
} \
	__TYPE__ *pRet = new(std::nothrow) __TYPE__(); \
	EMGR->registerEntity(pRet); \
if (pRet) \
	{ \
	return pRet; \
	} \
		else \
		{ \
		delete pRet; \
		pRet = NULL; \
		return NULL; \
		} \
}

class MovingEntity :public VirtualEntity
{
protected:
	Vec2 _position;
	Sprite* _sprite;
	Vec2 _velocity;//速度
	Vec2 _heading;//朝向
	Vec2 _side;//侧向，与朝向垂直
	double _mass;//质量
	double _maxSpeed;
	double _maxForce;
	CollisionVolume _collisionVolume;//碰撞体积
	SteeringBehaviors* _sb;//运动控制器

protected:
	bool _updateOn;
public:
	MovingEntity(int ID, Vec2 position, string entityType, Vec2 ve, Vec2 he, double ma, double mS, double mF, std::string filepath) :
		VirtualEntity(ID, entityType),
		_position(position),
		_velocity(ve),
		_heading(he),
		_mass(ma),
		_maxSpeed(mS),
		_maxForce(mF)
	{
		_sb = new SteeringBehaviors(this);
		_side = _heading.getPerp();
		_sprite = Sprite::create(filepath);
		_updateOn = true;
		init();
	}

	virtual ~MovingEntity();

	virtual void init() {}//针对碰撞体积的初始化

public://gtrs&strs

	void stop() { _velocity = Vec2::ZERO; }

	Vec2& position() { return _position; }
	void setPosition(const Vec2 po) { _position = po; }


	const Vec2 velocity()const { return _velocity; }
	void setVelocity(const Vec2 v){ _velocity = v; }


	Vec2 heading()const { return _heading; }
	void setHeading(const Vec2);


	Vec2 side()const { return _side; }
	//side is caculated from heading,cannot set side here


	double mass()const { return _mass; }
	void setMass(const double ma) { _mass = ma; }


	const double maxSpeed()const { return _maxSpeed; }
	void setMaxSpeed(const double v) { _maxSpeed = v; }


	const double maxForce()const { return _maxForce; }
	void setMaxForce(const double f) { _maxForce = f; }


	const double speed()const { return _velocity.getLength(); }
	bool isSpeedMaxedOut()const { return _maxSpeed*_maxSpeed >= _velocity.getLengthSq(); }
	const double speedSq() { return _velocity.getLengthSq(); }

	virtual double getBoundingRadius()const
	{
		return 0.5*std::max(_sprite->getBoundingBox().size.height,
			_sprite->getBoundingBox().size.width);
	}

	Sprite* getSprite() { return _sprite; }

	CollisionVolume getCV()
	{
		CollisionVolume CV;
		int count = 0;
		while (count < _collisionVolume.circle_.lenth())
		{
			Circle c1 = _collisionVolume.circle_[count];
			CV.circle_.push(Circle(c1.radius_, c1.center_ + _position, 0, 360));
			count++;
		}
		count = 0;
		while (count < _collisionVolume.rect_.lenth())
		{
			Rect r1 = _collisionVolume.rect_[count];
			CV.rect_.push(Rect(r1.origin.x + _position.x, r1.origin.y + _position.y, r1.size.width, r1.size.height));
			count++;
		}

		return CV;
	}

	SteeringBehaviors* sb() { return _sb; }

	void setUpdateOn(bool tag) { _updateOn = tag; }

	bool isUpdateOn()const { return _updateOn; }

public://space convert
	Vec2 convertToWorldSpace(Vec2 point)
	{
		//该死的数学！ ~TAT`
		double hx = this->_heading.x;
		hx += 0.00001;
		double hy = this->_heading.y;
		hy += 0.00001;
		double x0 = point.x;
		x0 += 0.00001;
		double y0 = point.y;
		y0 += 0.00001;

		//math function:
		/*

		a:本地坐标与世界坐标夹角，即heading
		b:向量的本地坐标
		c:向量的世界坐标

		a+b-c=90

		tan(a)+tan(b)         1
		_____________  = - ________
		1-tan(a)tan(b)      tan(c)

		*/

		Vec2 temp = Vec2::ZERO;

		temp.x = (hy / hx) + (y0 / x0);

		temp.y = (hy / hx) + (y0 / x0) - 1;

		temp.getNormalized();

		return (temp*point.getLength());
	}

	Vec2 convertToNodeSpace(Vec2 point)
	{
		//该死的数学！ ~TAT`
		double hx = this->_heading.x;
		hx += 0.00001;
		double hy = this->_heading.y;
		hy += 0.00001;
		double x0 = point.x;
		x0 += 0.00001;
		double y0 = point.y;
		y0 += 0.00001;

		//math function:
		/*

		a:本地坐标与世界坐标夹角，即heading
		b:向量的本地坐标
		c:向量的世界坐标

		a+b-c=90

		tan(a)+tan(b)         1
		_____________  = - ________
		1-tan(a)tan(b)      tan(c)

		*/

		Vec2 temp = Vec2::ZERO;

		temp.x = (hy / hx) - (y0 / x0);

		temp.y = (hy / hx) + (y0 / x0) + 1;

		temp.getNormalized();

		return (temp*point.getLength());
	}

public:
	virtual bool clear();
	
	virtual void update();

};

