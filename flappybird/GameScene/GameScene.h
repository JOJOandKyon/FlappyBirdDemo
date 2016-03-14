#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "cocos2d.h"
#include "Vector"
#include <Box2D\Box2D.h>
#include "cocos-ext.h"
#include "ui/CocosGUI.h"

using namespace cocos2d;
using namespace cocos2d::ui;

class GameScene : public cocos2d::Layer
{
public:
	// there's no 'id' in cpp, so we recommend returning the class instance pointer
	static cocos2d::Scene* createScene();

	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init();

	// implement the "static create()" method manually
	CREATE_FUNC(GameScene);
	void initAnmiRF();
	virtual void update1(float delta);
	void update2(float delta);
	void update3(float delta);
	void addbird();
	void addland();
	void addpipe();
	void addpipe2();
	int RandomHeight();
	//void birdTouch(cocos2d::Ref* pSender, Widget::TouchEventType type);
	bool birdTouchBegan(Touch* touch, Event* event);
	void birdTouchEnded(Touch* touch, Event* event);
	bool onContactBegin(const PhysicsContact& contact);
	void gamePaneIAppear();
	void gameRetart();
	void StartGameTouch(cocos2d::Ref* pSender, Widget::TouchEventType type);
private:
	Animate* anmiAc[2];
	int anmiIndex = 0;
	Sprite* birdsprite;
	Sprite* land1;
	Sprite* land2;
	PhysicsWorld* world;
	Sprite* pipe_up;
	Sprite* pipe_down;
	Sprite* pipe_up1;
	Sprite* pipe_down1;
	PhysicsBody* bird_body;
	char* j = "经过的水管数：";
	int i = 0;
	int* p = &i;
	Animate* bird;
};

#endif // __GAME_SCENE_H__
