#include "GameScene.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "Vector"
#include <Box2D\Box2D.h>

USING_NS_CC;

using namespace cocostudio::timeline;

Scene* GameScene ::createScene()
{
	// 'scene' is an autorelease object
	//auto scene = Scene::create();
	auto scene = Scene::createWithPhysics();
	//scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
	scene->getPhysicsWorld()->setGravity(Vec2(0, - 900));
	// 'layer' is an autorelease object
	auto layer = GameScene::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool GameScene::init()
{
	if (!Layer::init())
	{
		return false;
	}

	auto rootNode = CSLoader::createNode("MainScene.csb");

	addChild(rootNode);

	CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
	Vec2 visibleOrigin = CCDirector::sharedDirector()->getVisibleOrigin();
	Sprite* background = Sprite::create("background.png");
	background->setPosition(Point(visibleSize.width / 2,visibleSize.height / 2));
	addChild(background);

	addpipe();//添加第一组水管
	addpipe2();//添加第二组水管
	addbird();//添加小鸟精灵
	addland();//添加地板
	schedule(schedule_selector(GameScene::update1));//开启调用Update函数功能
	schedule(schedule_selector(GameScene::update2));
	schedule(schedule_selector(GameScene::update3));
	//添加全屏单点触控
	auto listenerTouch = EventListenerTouchOneByOne::create();
	listenerTouch->setSwallowTouches(true);
	listenerTouch->onTouchBegan = CC_CALLBACK_2(GameScene::birdTouchBegan, this);
	listenerTouch->onTouchEnded = CC_CALLBACK_2(GameScene::birdTouchEnded, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listenerTouch, this);
	//检测碰撞
	auto contactListener = EventListenerPhysicsContact::create();
	contactListener->onContactBegin = CC_CALLBACK_1(GameScene::onContactBegin, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);
	return true;
}

void GameScene::initAnmiRF()
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Point orgin = Director::getInstance()->getVisibleOrigin();

	std::string sa[4] = { "birds1.png", "birds2.png","birds3.png","birds4.png" };
	for (int i = 0; i < 1; i++)
	{
		Vector<SpriteFrame*> animFrames;
		SpriteFrame* f0 = SpriteFrame::create(sa[i], Rect(0, 0, 100, 200));
		SpriteFrame* f1 = SpriteFrame::create(sa[i+1], Rect(0, 0, 100, 200));
		SpriteFrame* f2 = SpriteFrame::create(sa[i+2], Rect(0, 0, 100, 200));
		SpriteFrame* f3 = SpriteFrame::create(sa[i+3], Rect(0, 0, 100, 200));
		animFrames.pushBack(f0);
		animFrames.pushBack(f1);
		animFrames.pushBack(f2);
		animFrames.pushBack(f3);
		Animation* anmi = Animation::createWithSpriteFrames(animFrames,0.15f);
		anmiAc[i] = Animate::create(anmi);
		anmiAc[i]->retain();
	}
}

void GameScene::update1(float delta)//实现地板左移
{
	/*land1->setPositionX(land1->getPositionX() - 1.0f);  另一个可以实现的方式
	land2->setPositionX(land1->getPositionX() + land1->getContentSize().width - 2.0f);
	if (land2->getPositionX() <= 0)
		land1->setPosition(Point::ZERO);*/
	int iSpeed = 2;
	int posX1 = land1->getPositionX();
	int posX2 = land2->getPositionX();

	posX1 -= iSpeed;
	posX2 -= iSpeed;

	CCSize mapSize = land1->getContentSize();
	if (posX1< - mapSize.width)
	{
		land1->setPositionX(posX2 + mapSize.width);
		land2->setPositionX(posX2);
	}
	else if (posX2< -mapSize.width)
	{
		land2->setPositionX(posX1 + mapSize.width);
		land1->setPositionX(posX1);
	}
	else
	{
		land1->setPositionX(posX1);
		land2->setPositionX(posX2);
	}
}

void GameScene::update2(float delta)//第一组水管的移动
{
	CCSize visibleSize = Director::sharedDirector()->getVisibleSize();
	int ispeed = 2;
	int RandomHeight;
	int pipe_upX = pipe_up->getPositionX();
	int pipe_downX = pipe_down->getPositionX();
	pipe_upX -= ispeed;
	pipe_downX -= ispeed;
	CCSize mapSize = pipe_up->getContentSize();

	if (pipe_upX < - mapSize.width)
	{
		RandomHeight = GameScene::RandomHeight();
		pipe_up->setPosition(Point(visibleSize.width + mapSize.width / 2, pipe_up->getContentSize().height + 80 + RandomHeight));
		pipe_down->setPosition(Point(visibleSize.width + mapSize.width/2,RandomHeight));
	}
	else
	{
		pipe_up->setPositionX(pipe_upX);
		pipe_down->setPositionX(pipe_downX);
		if (pipe_upX == visibleSize.width / 2)
		{
			*p = *p + 1;
		}
	}
	//为小鸟添加一个关于y轴方向上的旋转角度
	auto curVelocity = birdsprite->getPhysicsBody()->getVelocity();
	birdsprite->setRotation(-curVelocity.y*0.1 - 30);
}

void GameScene::update3(float delta)//第二组水管的移动
{
	CCSize visibleSize = Director::sharedDirector()->getVisibleSize();
	int ispeed = 2;
	int RandomHeight;
	int pipe_upX = pipe_up1->getPositionX();
	int pipe_downX = pipe_down1->getPositionX();
	pipe_upX -= ispeed;
	pipe_downX -= ispeed;
	CCSize mapSize = pipe_up1->getContentSize();

	if (pipe_upX < -mapSize.width)
	{
		RandomHeight = GameScene::RandomHeight();
		pipe_up1->setPosition(Point(visibleSize.width + mapSize.width / 2, pipe_up1->getContentSize().height + 80 + RandomHeight));
		pipe_down1->setPosition(Point(visibleSize.width + mapSize.width / 2, RandomHeight));
	}
	else
	{
		pipe_up1->setPositionX(pipe_upX);
		pipe_down1->setPositionX(pipe_downX);
		if (pipe_upX == visibleSize.width / 2)
		{
			*p = *p + 1;
		}
	}
}

void GameScene::addbird()//添加小鸟精灵和刚体
{
	CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
	birdsprite = Sprite::create("birds1.png");
	birdsprite->setTag(1);
	//initAnmiRF();
	//birdsprite->runAction(RepeatForever::create(anmiAc[anmiIndex]->clone()));
	addChild(birdsprite);
	auto Anime = Animation::create();
	for (int i = 1; i < 4; i++)
	{
		auto str_name = String::createWithFormat("birds%d.png", i);
		Anime->addSpriteFrameWithFile(str_name->getCString());
	}
	Anime->setDelayPerUnit(0.2);
	Anime->setLoops(-1);
	bird = Animate::create(Anime);
	birdsprite->runAction(bird);
	bird_body = PhysicsBody::createCircle(15);
	bird_body->setDynamic(true);   //设置为可以被物理场所作用而动作
	bird_body->setContactTestBitmask(1); //必须设置这项为1才能检测到不同的物体碰撞
	//bird_body->setGravityEnable(false);   //设置是否被重力影响,准备画面中不受重力影响
	birdsprite->setPhysicsBody(bird_body); //为小鸟设置刚体
	birdsprite->setPosition(Point(visibleSize.width / 2, visibleSize.height / 2));
	bird_body->addMass(5);
}

void GameScene::addland()//添加地板
{
	Size visibleSize = Director::sharedDirector()->getVisibleSize();
	auto visibleOrigin = Director::sharedDirector()->getVisibleOrigin();
	land1 = Sprite::create("land.png");
	land1->setAnchorPoint(Point(0, 0));
	land1->setPosition(Point(0, 0));
	addChild(land1, 10);

	land2 = Sprite::create("land.png");
	land2->setAnchorPoint(Point(0, 0));
	land2->setPosition(Point(land1->getContentSize().width, 0));
	addChild(land2, 10);

	//auto body = PhysicsBody::createEdgeBox(visibleSize,PHYSICSBODY_MATERIAL_DEFAULT, 5.0f);
	//为地板精灵添加刚体
	Node* groundNode = Node::create();
	auto groundBody = PhysicsBody::createBox(Size(visibleSize.width, land1->getContentSize().height));
	groundBody->setDynamic(false);
	groundBody->setContactTestBitmask(1);
	groundNode->setAnchorPoint(Vec2::ANCHOR_MIDDLE); //物理引擎中的刚体只允许结点锚点设置为中心
	groundNode->setPhysicsBody(groundBody);
	groundNode->setPosition(visibleOrigin.x + visibleSize.width / 2, land1->getContentSize().height / 2);
	this->addChild(groundNode);
}

void GameScene::addpipe()//添加第一组水管
{
	CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();

	pipe_up =Sprite:: create("pipe_up.png");
	pipe_up->setPosition(Point(visibleSize.width, pipe_up->getContentSize().height+80));
	addChild(pipe_up,10);

	pipe_down = Sprite::create("pipe_down.png");
	pipe_down->setPosition(Point(visibleSize.width, 0));
	//min -65 max 170
	addChild(pipe_down,10);
	
	auto pipeUpBody = PhysicsBody::createBox(pipe_up->getContentSize());
	pipeUpBody->setDynamic(false);
	//pipeUpBody->setGravityEnable(false);
	pipeUpBody->setContactTestBitmask(1);
	pipe_up->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	pipe_up->setPhysicsBody(pipeUpBody);

	auto pipeDownBody = PhysicsBody::createBox(pipe_down->getContentSize());
	pipeDownBody->setDynamic(false);
	//pipeDownBody->setGravityEnable(false);
	pipeDownBody->setContactTestBitmask(1);
	pipe_down->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	pipe_down->setPhysicsBody(pipeDownBody);

/*	Node* singlepipe = Node::create();
	singlepipe->addChild(pipe_up);
	singlepipe->addChild(pipe_down);
	singlepipe->setName("newpipe");
	singlepipe->setPosition(Point(70,70));
	addChild(singlepipe);*/

	Vector<Node*> pipes;
	pipes.pushBack(pipe_up);
	pipes.pushBack(pipe_down);

}

void GameScene::addpipe2()//添加第二组水管
{
	CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();

	pipe_up1 = Sprite::create("pipe_up.png");
	int RandomHeight = GameScene::RandomHeight();
	pipe_up1->setPosition(Point(visibleSize.width*1.6, pipe_up1->getContentSize().height+100+RandomHeight));
	addChild(pipe_up1, 10);

	pipe_down1 = Sprite::create("pipe_down.png");
	pipe_down1->setPosition(Point(visibleSize.width*1.6,20 + RandomHeight));
	//min -65 max 170
	addChild(pipe_down1, 10);

	auto pipeUpBody = PhysicsBody::createBox(pipe_up1->getContentSize());
	pipeUpBody->setDynamic(false);
	//pipeUpBody->setGravityEnable(false);
	pipeUpBody->setContactTestBitmask(1);
	pipe_up1->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	pipe_up1->setPhysicsBody(pipeUpBody);

	auto pipeDownBody = PhysicsBody::createBox(pipe_down1->getContentSize());
	pipeDownBody->setDynamic(false);
	//pipeDownBody->setGravityEnable(false);
	pipeDownBody->setContactTestBitmask(1);
	pipe_down1->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	pipe_down1->setPhysicsBody(pipeDownBody);

	/*	Node* singlepipe = Node::create();
	singlepipe->addChild(pipe_up);
	singlepipe->addChild(pipe_down);
	singlepipe->setName("newpipe");
	singlepipe->setPosition(Point(70,70));
	addChild(singlepipe);*/

	Vector<Node*> pipes;
	pipes.pushBack(pipe_up1);
	pipes.pushBack(pipe_down1);

}

int GameScene::RandomHeight()//随机水管高度
{
	int i;
	do
	{
		int x = rand() % 171;
		int y = rand() % 2;
		if (y == 0 && x < 66)
		{
			i = -x;
			break;
		}
		else if (y == 1)
		{
			i = x;
			break;
		}
	} while (1);
	return i;

}

bool GameScene::birdTouchBegan(Touch* touch, Event* event)//为小鸟添加一个触摸事件
{
	CCLOG("Begin");
	//给小鸟一个向上的加速度
	birdsprite->getPhysicsBody()->setVelocity(Vec2(0, 220));
	return true;
}
void GameScene::birdTouchEnded(Touch* touch, Event* event)
{}

bool GameScene::onContactBegin(const PhysicsContact& contact)//碰撞后执行
{
	CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
	Vec2 visivleOrigin = CCDirector::sharedDirector()->getVisibleOrigin();
	//停止水管，地板运动
	this->unschedule(schedule_selector(GameScene::update1));
	this->unschedule(schedule_selector(GameScene::update2));
	this->unschedule(schedule_selector(GameScene::update3));
	birdsprite->stopAction(bird);//停止小鸟拍翅膀动画
	Button* gameRetart = Button::create("button.png");//添加重新开始游戏按钮
	gameRetart->setPosition(Point(visibleSize.width / 2, visibleSize.height * 0.4));
	gameRetart->addTouchEventListener(CC_CALLBACK_2(GameScene::StartGameTouch ,this));
	addChild(gameRetart,10);
	gamePaneIAppear();
	return true;
}

void GameScene::gamePaneIAppear()
{
	CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
	Vec2 visivleOrigin = CCDirector::sharedDirector()->getVisibleOrigin();
	auto gameOverLabel = Sprite::create("game_over.png");//弹出game over图片
	gameOverLabel->setPosition(Point(visibleSize.width/2,visibleSize.height * 0.7));
	addChild(gameOverLabel,10);
	//显示通过水管数量
	auto ScoreTTF = CCLabelTTF::create(String::createWithFormat("%d",i)->getCString(), "Arial", 35);
	ScoreTTF->setPosition(Point(visibleSize.width/2,visibleSize.height/2));
	ScoreTTF->setColor(Color3B(255,255,255));
	addChild(ScoreTTF,10);
	//Node * gameOverPanelNode = Node::create();
	//gameOverPanelNode->addChild(gameOverLabel);
	//addChild(gameOverPanelNode);
	//gameOverPanelNode->setPosition(visivleOrigin.x + visibleSize.width / 2, visivleOrigin.y + visibleSize.height);

}

void GameScene::StartGameTouch(cocos2d::Ref* pSender, Widget::TouchEventType type)//重新游戏按钮触摸设置
{
	switch (type)
	{
	case cocos2d::ui::Widget::TouchEventType::BEGAN:
		CCLOG("BEGAN");
		break;
	case cocos2d::ui::Widget::TouchEventType::MOVED:
		CCLOG("MOVED");
		break;
	case cocos2d::ui::Widget::TouchEventType::ENDED:
		gameRetart();
		break;
	case cocos2d::ui::Widget::TouchEventType::CANCELED:
		CCLOG("CANCELED");
		break;
	default:
		break;
	}
}

void GameScene::gameRetart()//重新运行场景
{
	auto gameScene = GameScene::createScene();
	Director::getInstance()->replaceScene(gameScene);
}