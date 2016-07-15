#pragma once

//抽象类的判断
template <typename T>
struct IsAbstract
{
	typedef char SmallType;
	typedef int LargeType;

	template <typename U>
	static char Test(U(*)[1]);
	template <typename U>
	static int Test(...);

	const static bool Result = sizeof(Test<T>(NULL)) == sizeof(LargeType);
};

//basic delarations
#define MAIN_FPS 60

//IDs
#define HERO_ID 1
#define CONTROLLER_ID 2
#define GAME_HANDLLER_ID 3
#define VIRTUAL_MAP_ID 4
#define POCKET_ID 5


//steering behavior declarations

#define decelerationWeaker 0.3f
#define minObstacleRadius 10.0f
#define	maxObstacleRadius 30.0f
#define minDetectionBoxLength 40.0f
#define brakingWeight 0.2f
#define halfPI 1.570796f
#define max_double 10000.0f
#define disFromBoundary 30.0f
#define normalSeparationForce 50.0f
#define wanderDis 100.0f
#define wanderRadius 1.2f
#define wanderJitter 200
#define wayPointSeekDis 20.0f

//controller declarations
enum MoveDirection{
	UP,
	RIGHT,
	DOWN,
	LEFT
};

#define MAX_BTN_SIZE 200
#define BTN_Z_ORDER -10
#define BTN_FLOAT_Z_ORDER 0
#define CONTROLLER_FPS 40
#define ILLEGAL_INT -1000

//msg declarations
enum MsgType{
	hero_moveUp			= 0x0000,
	hero_moveRight		= 0x0001,
	hero_moveDown		= 0x0002,
	hero_moveLeft		= 0x0003,
	hero_pickOrAttack	= 0x0004,
	hero_standStill		= 0x0005,
	hero_openPocket		= 0x0006,
	game_pause			= 0x0007,
	animal_beAttacked	= 0x0008,
	animal_eatFood		= 0x0009,
	animal_dided		= 0x0010,//仇恨目标死亡
	time_night			= 0x0011,
	animal_attack		= 0x0012,
	product_picked		= 0x0013,
	food_ate			= 0x0014,
	hero_die			= 0x0015
};

//virtual map declarations
#define WHOLE_MAP_WIDTH 4000
#define WHOLE_MAP_HEIGHT 4000
#define MAX_ANIMAL_NUMBER 20
#define MAX_LAND_NUMBER 20
#define MAX_PLANT_NUMBER 60

//vision map declarations
#define IMG_WIDTH 1000
#define IMG_HEIGHT 500
#define RECT_SIZE 10
#define HERO_LOCATION_SCREEN Point(500,250)
#define MAP_Z_ORDER 0
#define CONTROLLER_Z_ORDER 100

//living things delarations
#define HUNGER_EXHAUSTED_DURATION 100
#define HUNGRY_LIMBO 20
#define FATIGUE_LIMBO 80
#define SICKNESS_RUN_OUT_DURATION 100
#define FATIGUE_REGENERATION_DURATION 100




