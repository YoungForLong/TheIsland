#include"SteeringBehaviors.h"
#include"MovingEntity.h"
#include"TemplateFunctions.h"
#include"Random.h"
#include"EntityManenger.h"
#include"Cell_Space_Partition.h"
#include"Wall.h"

SteeringBehaviors::SteeringBehaviors(MovingEntity* owner):
_ownerMovingEntity(owner),
_behaviorFlags(0),
_dBoxLength(10),
_weightCohesion(0.6),
_weightAlignment(1.0),
_weightSeparation(1.0),
_weightArrive(1.0),
_weightObstacleAvoidance(10.0),
_weightWander(1.0),
_sightDis(200.0),
_deceleration(normal),
_target(NULL),
_wanderDistance(wanderDis),
_wanderJitter(wanderJitter),
_wanderRadius(wanderRadius),
_weightSeek(1.0),
_weightFlee(1.0),
_weightPursuit(1.0),
_weightOffsetPursuit(1.0),
_weightHide(8.0),
_weightEvade(0.01),
_wanderTarget(Vec2::ZERO)
{

}

Vec2 SteeringBehaviors::seek(const Vec2 target)
{
	Vec2 desiredVelocity = (target - _ownerMovingEntity->position()).getNormalized()*_ownerMovingEntity->maxSpeed();

	return (desiredVelocity - _ownerMovingEntity->velocity());
}

Vec2 SteeringBehaviors::flee(const Vec2 target)
{
	Vec2 desiredVelocity = (_ownerMovingEntity->position() - target).getNormalized()*_ownerMovingEntity->maxSpeed();

	return (desiredVelocity - _ownerMovingEntity->velocity());
}

Vec2 SteeringBehaviors::arrive(const Vec2 target, Deceleration dece)
{
	Vec2 toTarget = target - _ownerMovingEntity->position();

	double distance = toTarget.getLength();

	if (distance > 0.00001)
	{
		double speed = distance / (double)dece*decelerationWeaker;
		speed = std::min(_ownerMovingEntity->maxSpeed(), speed);

		Vec2 desiredVelocity = toTarget / distance*speed;

		return (desiredVelocity - _ownerMovingEntity->velocity());
	}
	else
		return Vec2::ZERO;
}

Vec2 SteeringBehaviors::pursuit(MovingEntity* evader)
{
	Vec2 toEvader = evader->position() - _ownerMovingEntity->position();

	//dk how to descripe this angle,×·×ÙÕßºÍÌÓÍöÕß³¯ÏòµÄ¼Ð½Ç
	double relativeHeading = _ownerMovingEntity->heading().dot(evader->heading());

	if (toEvader.dot(_ownerMovingEntity->heading()) > 0/*are they face towards each-other*/
		&& (relativeHeading < -0.95))
	{
		return seek(evader->position());
	}
	//ahead time
	double lookAheadTime = toEvader.getLength() / (_ownerMovingEntity->maxSpeed() + evader->speed());

	return seek(evader->position() + evader->velocity()*lookAheadTime);
}

Vec2 SteeringBehaviors::getHidePosition(MovingEntity* obstacle, const Vec2 target)
{
	double dis = obstacle->getBoundingRadius() + disFromBoundary;

	Vec2 target2Obstacle = (obstacle->position() - target).getNormalized();

	return (dis*target2Obstacle + obstacle->position());
}

//Vec2 SteeringBehaviors::hide(MovingEntity* target, const std::vector<MovingEntity*> obstacles)
//{
//	if (!target)
//	{
//		return Vec2::ZERO;
//	}
//	//out of sight
//	bool isHide = false;
//	for_each(obstacles.cbegin(), obstacles.cend(), [this, target, &isHide](MovingEntity* obs){
//		if (Seg::PointToSegDist(obs->position(), Seg(_ownerMovingEntity->position(), target->position())) < obs->getBoundingRadius())
//			isHide = true;
//	});
//	if (isHide)
//		return Vec2::ZERO;
//	if (Vec2(_ownerMovingEntity->position() - target->position()).getLengthSq() > _sightDis*_sightDis)
//		return Vec2::ZERO;
//
//	double disSq2Closest = max_double;
//	Vec2 closestHidingSpot = Vec2::ZERO;
//
//	std::for_each(obstacles.cbegin(), obstacles.cend(), [this, target, &disSq2Closest, &closestHidingSpot](MovingEntity* eachObstacle)
//	{
//		Vec2 hidingSpot = getHidePosition(eachObstacle, target->position());
//
//		double disSq = (hidingSpot - _ownerMovingEntity->position()).getLengthSq();
//
//		if (disSq < disSq2Closest)
//		{
//			disSq2Closest = disSq;
//
//			closestHidingSpot = hidingSpot;
//		}
//	}
//	);
//	//end for_each
//
//	if (disSq2Closest == max_double)
//	{
//		return evade(target);
//	}
//
//	return arrive(closestHidingSpot, fast);
//}


Vec2 SteeringBehaviors::separation(std::vector<MovingEntity*> neighbors)
{
	Vec2 steeringForce = Point::ZERO;

	std::for_each(neighbors.cbegin(), neighbors.cend(), [this, &steeringForce](MovingEntity* neighbor){
		if (neighbor != _target&&neighbor != _ownerMovingEntity&&neighbor->type()==_ownerMovingEntity->type())
		{
			Vec2 awayFromNeighbor = _ownerMovingEntity->position() - neighbor->position();
			/*awayFromNeighbor += Vec2(0.00001, 0.00001);*/
			/*steeringForce += awayFromNeighbor.getNormalized() / awayFromNeighbor.getLength()*normalSeparationForce;*/
			steeringForce += awayFromNeighbor / awayFromNeighbor.getLengthSq()*normalSeparationForce;
		}
	});
	return steeringForce;
}

Vec2 SteeringBehaviors::alignment(std::vector<MovingEntity*> neighbors)
{
	Vec2 averageHeading = Vec2::ZERO;

	std::for_each(neighbors.cbegin(), neighbors.cend(), [this, &averageHeading](MovingEntity* neighbor){
		if (neighbor != _target&&neighbor->type() == _ownerMovingEntity->type())
		{
			averageHeading += neighbor->heading();
		}
	});
	if (neighbors.size() != 0)
	{
		averageHeading = averageHeading / (double)neighbors.size();
	}
	else
	{
		return Vec2::ZERO;
	}

	return (averageHeading - _ownerMovingEntity->heading());
}

Vec2 SteeringBehaviors::cohesion(std::vector<MovingEntity*> neighbors)
{
	//we could consider the weight each neighbor owned
	Vec2 averagePosition;
	std::for_each(neighbors.cbegin(), neighbors.cend(), [this, &averagePosition](MovingEntity* neighbor){
		if (neighbor != _target&&neighbor->type() == _ownerMovingEntity->type())
		{
			averagePosition += neighbor->position();
		}
	});

	if (neighbors.size() != 0)
	{
		averagePosition = averagePosition / (double)neighbors.size();
	}
	else
	{
		return Vec2::ZERO;
	}

	return seek(averagePosition);
}
Vec2 SteeringBehaviors::evade(MovingEntity* pursuer)
{
	if (!pursuer)
	{
		return Vec2::ZERO;
	}

	Vec2 toPursuer = pursuer->position() - _ownerMovingEntity->position();

	double lookAheadTime = toPursuer.getLength() / (_ownerMovingEntity->maxSpeed() + pursuer->speed());

	return flee(pursuer->position() + pursuer->velocity()*lookAheadTime);
}

Vec2 SteeringBehaviors::wander()
{
	//random move
	_wanderTarget += Vec2(Random::Rand(-1 * wanderJitter, wanderJitter),
		Random::Rand(-1 * wanderJitter, wanderJitter)) / 40.0f;
	//project it onto the circle
	_wanderTarget = _wanderTarget.getNormalized()*wanderRadius*_ownerMovingEntity->getBoundingRadius();
	//add jitter
	Vec2 targetOnLocal = _wanderTarget + Vec2(wanderDis, 0);

	Vec2 targetOnWorld = _ownerMovingEntity->convertToWorldSpace(targetOnLocal) + _ownerMovingEntity->position();

	return targetOnWorld - _ownerMovingEntity->position();
}

Vec2 SteeringBehaviors::obstacleAvoidance(std::vector<MovingEntity*>& obstacles)
{
	_dBoxLength = minDetectionBoxLength*(1 + _ownerMovingEntity->speed() / _ownerMovingEntity->maxSpeed());

	//nearest intersection dis alone axis x
	double NIOL_x = max_double;

	//nearest intersection position on local
	Vec2 NIOL_po = Vec2::ZERO;

	//nearest obstacle
	MovingEntity* NObstacle = NULL;

	//entities within view range
	//std::vector<MovingEntity*> neighborObstacles = EMGR->getNeighbors(_ownerMovingEntity, EMGR->getVecByType(1), _dBoxLenth);
	std::vector<MovingEntity*> neighborObstacles = obstacles;

	//find the point of the force
	std::for_each(neighborObstacles.begin(), neighborObstacles.end(), [this, &NIOL_x, &NIOL_po, &NObstacle](MovingEntity* obstacle)
	{
		Vec2 poOnLocal = _ownerMovingEntity->convertToNodeSpace(obstacle->position());

		//tips: better to use "do while(0)" struct
		if (poOnLocal.x >= 0)
		{
			double expandedRadius = obstacle->getBoundingRadius() + _ownerMovingEntity->getBoundingRadius();

			if (fabs(poOnLocal.y) < expandedRadius)
			{
				//find out the intersections
				double intersectionX = poOnLocal.x - (expandedRadius*expandedRadius - (poOnLocal.y)*(poOnLocal.y));

				//just determined by the positive axis X
				if (intersectionX <= 0)
				{
					intersectionX = poOnLocal.x + (expandedRadius*expandedRadius - (poOnLocal.y)*(poOnLocal.y));
				}

				if (intersectionX < NIOL_x)
				{
					NIOL_x = intersectionX;

					NIOL_po = poOnLocal;

					NObstacle = obstacle;
				}
			}
		}
	}
	);

	//caculate the force
	Vec2 steeringForce = Vec2::ZERO;

	//is obstacle exist
	if (NObstacle)
	{
		//no matter on the x or y, the closer, the stronger the for shouble be

		//the effect on x
		double multiplier = 1.0 + (_dBoxLength - NIOL_po.x) / _dBoxLength;
		//the effect on y
		steeringForce.y = (NObstacle->getBoundingRadius() - NIOL_po.y)*multiplier;

		//apply a braking force
		steeringForce.x = (NObstacle->getBoundingRadius() - NIOL_po.x)*brakingWeight;
	}

	//convert the force from local to world space
	return _ownerMovingEntity->convertToWorldSpace(steeringForce);
}

Vec2 SteeringBehaviors::calculate()
{
	//reset the force
	_totalSF = Vec2::ZERO;

	//get neighbors only once
	//space partition is always opening

	_totalSF = calculatePrioritizedWighted(CSP->getNeighbors(_ownerMovingEntity, _sightDis));

	return _totalSF;
}

bool SteeringBehaviors::accumulateForce(Vec2& runningTot, Vec2 forceToAdd)
{
	double magnitude = runningTot.getLength();

	double magnitudeRemainning = _ownerMovingEntity->maxForce() - magnitude;

	if (magnitude < 0.0)
		return false;

	if (magnitude < magnitudeRemainning)
	{
		runningTot += forceToAdd;
	}
	else
	{
		runningTot += forceToAdd.getNormalized()*magnitudeRemainning;
	}

	return true;
}


Vec2 SteeringBehaviors::calculatePrioritizedWighted(vector<MovingEntity*> neighbors)
{
	Vec2 force = Vec2::ZERO;


	if (on(OBSTACLE_AVOIDANCE))
	{
		
		force = obstacleAvoidance(neighbors) * _weightObstacleAvoidance;

		if (!accumulateForce(_totalSF, force)) return _totalSF;
	}

	//if (on(HIDE))
	//{
	//	/*assert(_target1 && "Hide target not assigned");*/

	//	force = hide(_target, neighbors) * _weightHide;

	//	if (!accumulateForce(_totalSF, force)) return _totalSF;
	//}

	if (on(EVADE))
	{
		force = evade(_target) * _weightEvade;

		if (!accumulateForce(_totalSF, force)) return _totalSF;
	}


	/*if (on(FLEE))//we never use flee merely
	{
	force = flee(_ownerMovingEntity->World()->Crosshair()) * m_dWeightFlee;

	if (!AccumulateForce(m_vSteeringForce, force)) return m_vSteeringForce;
	}*/



	//these next three can be combined for flocking behavior (wander is
	//also a good behavior to add into this mix)
	if (on(SEPARATION))
	{
		force = separation(neighbors) * _weightSeparation;

		if (!accumulateForce(_totalSF, force)) return _totalSF;
	}

	if (on(ALLIGNMENT))
	{
		force = alignment(neighbors) * _weightAlignment;

		if (!accumulateForce(_totalSF, force)) return _totalSF;
	}

	if (on(COHESION))
	{
		force = cohesion(neighbors) * _weightCohesion;

		if (!accumulateForce(_totalSF, force)) return _totalSF;
	}

	if (on(SEEK))
	{
		force = seek(_poTarget) * _weightSeek;

		if (!accumulateForce(_totalSF, force)) return _totalSF;
	}


	if (on(ARRIVE))
	{
		force = arrive(_poTarget, _deceleration) * _weightArrive;

		if (!accumulateForce(_totalSF, force)) return _totalSF;
	}

	if (on(WANDER))
	{
		force = wander() * _weightWander;

		if (!accumulateForce(_totalSF, force)) return _totalSF;
	}

	if (on(PURSUIT))
	{
		assert(_target && "pursuit target not assigned");

		force = pursuit(_target) * _weightPursuit;

		if (!accumulateForce(_totalSF, force)) return _totalSF;
	}

	/*if (on(OFFSET_PURSUIT))
	{
		assert(_target && "pursuit target not assigned");
		assert(!_offset.isZero() && "No offset assigned");

		force = offsetPursuit(_target, _offset);

		if (!accumulateForce(_totalSF, force)) return _totalSF;
	}*/

	return _totalSF;
}
