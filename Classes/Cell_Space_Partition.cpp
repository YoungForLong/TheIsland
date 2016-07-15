#include"Cell_Space_Partition.h"


CellSpacePartition* CellSpacePartition::instance()
{
	if (NULL == _instance){
		_instance = new CellSpacePartition;
		_instance->init(map_size, cell_number_x, cell_number_y);
	}
	return _instance;
}

void CellSpacePartition::init(Rect spaceSize, int cellNumX, int cellNumY)
{
	_spaceRect = spaceSize;
	_cellNumX = cellNumX;
	_cellNumY = cellNumY;

	_cellSizeX = _spaceRect.size.width / cellNumX;
	_cellSizeY = _spaceRect.size.height / cellNumY;

	for (int y = 0; y<_cellNumY; y++)
	{
		for (int x = 0; x<_cellNumX; x++)
		{
			double left = x * _cellSizeX;
			double bot = y * _cellSizeY;
			_cells.push_back(Cell(Rect(left, bot, _cellSizeX, _cellSizeY)));
		}
	}
}

void CellSpacePartition::addEntity(MovingEntity* entity)
{
	assert(entity&&
		"NULL entity");
	this->_cells.at(positionToIndex(entity->position())).members_.push_back(entity);
}

int CellSpacePartition::positionToIndex(const Vec2 position)const
{
	assert((position.x<_spaceRect.getMaxX()) && (position.y < _spaceRect.getMaxY()) &&
		"outside the world");

	int index = (int)(position.x / _cellSizeX) + (int)(position.y / _cellSizeY)*_cellNumX;
	return index;
}

void CellSpacePartition::updateEntity(MovingEntity* entity, Vec2 old_position)
{
	assert(entity&&
		"NULL entity");

	int old_idx = positionToIndex(old_position);

	int cur_idx = positionToIndex(entity->position());

	if (old_idx == cur_idx)
		return;
	else
	{
		this->_cells.at(old_idx).members_.remove(entity);
		this->_cells.at(cur_idx).members_.push_back(entity);
	}
}

std::vector<MovingEntity*> CellSpacePartition::getNeighbors(MovingEntity* entity, double radius)
{
	Vec2 position = entity->position();

	Vec2 temp = Vec2(position - Vec2(radius, radius));

	//first step:get cells intersecting the view circle
	std::vector<Cell> queryRects;
	queryRects = getCells(position, radius);
	std::vector<MovingEntity*> neighbors;

	//caculate if the rect is contained by circle
	std::for_each(queryRects.cbegin(), queryRects.cend(), [&entity,&neighbors, position, radius, this](Cell cell){
		for (auto iter = cell.members_.cbegin(); iter != cell.members_.end(); iter++)
		{
			Vec2 toNeighbor = (*iter)->position() - position;
			if (toNeighbor.getLengthSq() < (radius*radius)&&(*iter)!=entity)
			{
				neighbors.push_back(*iter);
			}
		}
	});

	return neighbors;
}

//std::vector<MovingEntity*> CellSpacePartition::getEnermies(MovingEntity* entity, double radius)
//{
//	Vec2 position = entity->position();
//
//	Vec2 temp = Vec2(position - Vec2(radius, radius));
//
//	//first step:get cells intersecting the view circle
//	std::vector<Cell> queryRects;
//	queryRects = getCells(position, radius);
//	std::vector<MovingEntity*> neighbors;
//
//	//caculate if the rect is contained by circle
//	std::for_each(queryRects.cbegin(), queryRects.cend(), [&entity, &neighbors, position, radius, this](Cell cell){
//		for (auto iter = cell.members_.cbegin(); iter != cell.members_.end(); iter++)
//		{
//			Vec2 toNeighbor = (*iter)->position() - position;
//			if (toNeighbor.getLengthSq() < (radius*radius))
//			{
//				bool tag = false;
//				auto enermyType = 
//				for (auto iter2 = enermyType.cbegin(); iter2 != enermyType.cend(); iter2++)
//				{
//					if (*iter2 == ((Fish*)(*iter))->getFishType())
//						tag = true;
//				}
//				if (tag)
//					neighbors.push_back(*iter);
//			}
//		}
//	});
//
//	return neighbors;
//}

void CellSpacePartition::clear()
{
	for_each(_cells.begin(), _cells.end(), [this](Cell eachCell){
		eachCell.members_.clear();
	});

	_cells.clear();
}

const std::vector<Cell> CellSpacePartition::getCells(Vec2 centre, double radius)
{
	Vec2 LBpoint = Vec2(centre.x - radius, centre.y - radius);
	Vec2 RTpoint = Vec2(centre.x + radius, centre.y + radius);

	int row, col, rowTo, colTo;

	row = LBpoint.x / _cellSizeX;
	rowTo = RTpoint.x / _cellSizeX;

	col = LBpoint.y / _cellSizeY;
	colTo = RTpoint.y / _cellSizeY;

	std::vector<Cell> tempVec;

	for (int i = row; i < (rowTo + 1); i++)
	{
		for (int j = col; j < (colTo+1); j++)
		{
			tempVec.push_back(_cells.at(j*_cellNumX + i));
		}
	}

	return tempVec;
}