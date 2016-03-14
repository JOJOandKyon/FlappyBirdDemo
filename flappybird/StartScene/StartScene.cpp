#include "StartScene.h"
#include "cocostudio\CocoStudio.h"
#include "GameScene\GameScene.h"
#include "ui/CocosGUI.h"

USING_NS_CC;
using namespace cocostudio::timeline;
using namespace cocos2d::ui;

Scene* Start::createScene()
{
	auto scene = Scene::create();
	auto Layer = Start::create();
	scene->addChild(Layer);
	return scene;
}

bool Start::init()
{
	if (!Layer::init())
	{
		return  false;
	}
	auto rootNode = CSLoader::createNode("MainScene.csb");
	addChild(rootNode);
	addBackground();
	addLand();
	addBird();
	schedule(schedule_selector(Start::update),0.3);
	auto listenerTouch = EventListenerTouchOneByOne::create();
	listenerTouch->onTouchBegan = CC_CALLBACK_2(Start::onTouchBegan,this);
	listenerTouch->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listenerTouch, this);
	return true;
}

void Start::addBackground()
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Background = Sprite::create("background.png");
	Background->setPosition(Point(visibleSize.width/2,visibleSize.height/2));
	addChild(Background);
}

void Start::addLand()
{
	Land = Sprite::create("land.png");
	Land->setAnchorPoint(ccp(0,0));
	Land->setPosition(Point(0,0));
	addChild(Land);
}

void Start::addBird()
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Bird = Sprite::create();
	auto Anime = Animation::create();
	for (int i = 1; i < 5; i++)
	{
		auto str_name = String::createWithFormat("birds%d.png",i);
		Anime->addSpriteFrameWithFile(str_name->getCString());
	}
	Anime->setDelayPerUnit(0.15);
	Anime->setLoops(-1);

	Bird->setPosition(Point(visibleSize.width / 2, visibleSize.height / 2+15));
	Bird->runAction(Animate::create(Anime));
	Bird->setTag(100);
	addChild(Bird);
}

void Start::update(float delta)
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
	int distance = 3;
	if (Bird->getTag()==100)
	{
		Bird->setPositionY(visibleSize.height / 2 + distance);
		Bird->setTag(101);
	}
	if (Bird->getTag()==101)
	{
		Bird->setPositionY(visibleSize.height / 2 - 2*distance);
		Bird->setTag(102);
	}
	else if (Bird->getTag() == 102)
	{
		Bird->setPositionY(visibleSize.height / 2 + 2 * distance);
		Bird->setTag(101);
	}
}

bool Start::onTouchBegan(Touch* touch, Event* event)
{
	CCLOG("begin");
	GameStart();
	return true;
}

void Start::GameStart()
{
	auto gameScene = GameScene::createScene();
	Director::getInstance()->setDepthTest(true);
	Director::getInstance()->replaceScene(gameScene);
}