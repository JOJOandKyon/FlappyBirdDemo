#ifndef _START_SCENE_H_
#define _START_SCENE_H_

#include "cocos2d.h"

using namespace cocos2d;

class Start:public Layer
{
public:
	static Scene* createScene();
	virtual bool init();
	CREATE_FUNC(Start);
	void addBackground();
	void addLand();
	void addBird();
	void update(float delta);
	bool onTouchBegan(Touch* touch,Event* event);
	void GameStart();
private:
	Size visibleSize = Director::sharedDirector()->getVisibleSize();
	Sprite* Background;
	Sprite* Bird;
	Sprite* Land;
};
#endif 