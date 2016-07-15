#include"MovingEntity.h"
#include"Cell_Space_Partition.h"

MovingEntity::~MovingEntity()
{
	this->clear();
}

bool MovingEntity::clear()
{
	delete _sb;
	_sb = NULL;
	return true;
}

void MovingEntity::update()
{
	if (!isUpdateOn())
	{
		return;
	}
	//a=f/m
	//dv=a*t
	//ds=v*t
	const double fps = 40.0f;
	double timeElapsed = 1 / fps;

	Vec2 previousPos = this->position();

	Vec2 steeringForce = _sb->calculate();

	Vec2 acceleration = steeringForce / _mass;

	_velocity += acceleration*timeElapsed;

	if (_velocity.getLengthSq() > _maxSpeed*_maxSpeed)
	{
		_velocity = _velocity.getNormalized()*_maxSpeed;
	}

	_position += _velocity*timeElapsed;

	//while outside of the Rect(0,0,1000,1000)

	if (_position.x > WHOLE_MAP_WIDTH)
	{
		_position.x -= WHOLE_MAP_WIDTH;
	}
	if (_position.x < 0)
	{
		_position.x += WHOLE_MAP_WIDTH;
	}
	if (_position.y > WHOLE_MAP_HEIGHT)
	{
		_position.y -= WHOLE_MAP_HEIGHT;
	}
	if (_position.y < 0)
	{
		_position.y += WHOLE_MAP_HEIGHT;
	}

	if (_velocity.getLengthSq() > 0.00000001)
	{
		_heading = _velocity.getNormalized();

		_side = _heading.getPerp();
	}

	//update CSP
	CSP->updateEntity(this, previousPos);
}