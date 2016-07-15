#pragma once
//一个空间划分器

#include<algorithm>
#include"cocos2d.h"
#include"MovingEntity.h"
#include"Singleton.h"

using namespace cocos2d;

struct Cell
{
	std::list<MovingEntity*> members_;
	Rect box_;
	Cell(Rect new_box) :box_(new_box){}
};

class CellSpacePartition:public Singleton<CellSpacePartition>
{
private:
	//all cells
	std::vector<Cell> _cells;

	//world space 
	Rect _spaceRect;

	//cell nums
	int _cellNumX;
	int _cellNumY;

	//cell space
	double _cellSizeX;
	double _cellSizeY;

protected:
	int positionToIndex(const Vec2)const;

public:
#define map_size Rect(0,0,WHOLE_MAP_WIDTH,WHOLE_MAP_HEIGHT)
#define cell_number_x 10
#define cell_number_y 10

public:
	void init(Rect, int,int);
	void addEntity(MovingEntity*);
	void updateEntity(MovingEntity*, Vec2);
	std::vector<MovingEntity*> getNeighbors(MovingEntity*, double);
	//std::vector<MovingEntity*> getEnermies(MovingEntity*, double);
	void clear();
	const std::vector<Cell> getCells(Vec2, double);
	static CellSpacePartition* instance();
};

#define CSP CellSpacePartition::instance()

