#pragma once
#include"cocos2d.h"
#include"TemplateFunctions.h"
#include"VirtualEntity.h"


using namespace cocos2d;

class ControllerLayer :public Layer,public VirtualEntity
{
public://ctors
	ControllerLayer();
	~ControllerLayer();
	virtual bool init();
	CREATE_FUNC(ControllerLayer);
	//初始化
	void touchListener();//触摸
	void reDraw();
	bool setCache();
	void removeTouchListener();
	void onMoveEnd();
public://styles
    //布局函数
	bool setStyle();

public://操作函数
	void move(const MoveDirection direction);
	void pickOrAttack();
	void pause();
	void openPocket();

public://system funcs
	//定时器函数
	void scheduleFunc(float);
	void selectButton();//选择按钮进行回调,按钮状态切换
	//Image* getNewImg(Sprite*);//获取图片信息
	
	virtual void update() {}
	
	virtual bool handleMsg(const Telegram& msg) { return false; }

	virtual bool clear() { return false; }
private:
	static bool _touchable;//是否可以触摸，该层被覆盖时是不可触摸的
	static Point _touchPoint;
	static bool _isTouched;
	Vector<Sprite*> _btnArr;//所有的按钮
	RenderTexture* _render;//渲染器
	EventListenerTouchOneByOne* _eventListener;
	int _selectedBtn;
private://计时器
	int _pocketOpenCount;
	bool _pocketOpenTag;
};




