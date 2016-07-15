#pragma once
#include"cocos2d.h"
#include"MovingEntity.h"
#include"TemplateFunctions.h"
using namespace cocos2d;

#define UI_LAYER_FPS 40


typedef std::function<void()> pocketCallBack;

class Pocket:public Layer,public VirtualEntity
{
public:
	Pocket();
	~Pocket();
	virtual bool init();
	CREATE_FUNC(Pocket);
public:
	virtual void update() {}
	virtual bool handleMsg(const Telegram& msg);
	virtual bool clear();
public:
	void openPocket();
	void closePocket();
	bool setCache();
	bool addItems(MovingEntity*);
	bool touchListener();
	void removeTouchListener();
	void selectCloseBtn();
	void scheduleFunc(float);
	void refresh();
public:
	bool _isOpen;
private:
	vector< MovingEntity*> _productsList;
	vector<Sprite*> _productsSprites;
	Sprite* _nullPocket;
	Sprite* _closeBtn;
	Point _touchPoint;
	bool _isTouched;
	EventListenerTouchOneByOne* _eventListener;
};
