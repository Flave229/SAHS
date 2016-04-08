#include "CreditsScene.h"

USING_NS_CC;

using namespace cocostudio::timeline;


Scene* CreditsScene::createScene()
{
	auto sceneCredits = Scene::create();

	auto layer = CreditsScene::create();

	sceneCredits->addChild(layer);

	return sceneCredits;
}

bool CreditsScene::init()
{
	if (!Layer::init())
	{
		return false;
	}

	auto rootNode = CSLoader::createNode("Credits.csb");
	addChild(rootNode);

	this->scheduleUpdate();

	auto winSize = Director::getInstance()->getVisibleSize();

	auto touchListener = EventListenerTouchOneByOne::create();

	touchListener->onTouchBegan = CC_CALLBACK_2(CreditsScene::onTouchBegan, this);
	touchListener->onTouchEnded = CC_CALLBACK_2(CreditsScene::onTouchEnded, this);
	touchListener->onTouchMoved = CC_CALLBACK_2(CreditsScene::onTouchMoved, this);
	touchListener->onTouchCancelled = CC_CALLBACK_2(CreditsScene::onTouchCancelled, this);

	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);

	_returnButton = static_cast<ui::Button*>(rootNode->getChildByName("ReturnButton"));
	_returnButton->addTouchEventListener(CC_CALLBACK_2(CreditsScene::ReturnButtonPressed, this));
	_returnButton->setPosition(Vec2(winSize.width*0.825f, winSize.height*0.10f));
	_returnButton->setGlobalZOrder(9);


	_logo = Sprite::create("Logo.png");
	_logo->setPosition(Vec2(winSize.width*0.60f, winSize.height*0.75f));
	_logo->setScale(0.75f);

	this->addChild(_logo);

	_planet = Sprite::create("Planet.png");
	_planet->setPosition(Vec2(0.0f, 0.0f - (_planet->getContentSize().height / 8)));
	//_planet->setScale(8.0f);
	this->addChild(_planet);

	_ship = Sprite::create("HuskySpaceShipDamage.png");
	_ship->setPosition(Vec2(winSize.width*0.3f, winSize.height*0.55f));
	_ship->setScale(0.25f);
	_ship->setRotation(30);

	this->addChild(_ship);

	for (int i = 0; i < 100; i++) {
		_stars.push_back(Sprite::create("SpaceStar.png"));

		int randomWidth = cocos2d::RandomHelper::random_real(0.0f, winSize.width);
		int randomHeight = cocos2d::RandomHelper::random_real(0.0f, winSize.height);

		_stars.at(i)->setPosition(Vec2(randomWidth, randomHeight));
		_stars.at(i)->setGlobalZOrder(-2);

		this->addChild(_stars.at(i));
	}


	_creditsBG = Sprite::create("CreditsScreen.png");
	_creditsBG->setPosition(Vec2(winSize.width*0.5f, winSize.height*0.37f));
	_creditsBG->setScaleX(1.5f);
	_creditsBG->setScaleY(1.89);

	this->addChild(_creditsBG);

	_progCredit = (ui::Text*)rootNode->getChildByName("Progr_Credits");
	_progCredit->setPosition(Vec2(winSize.width*0.20f, winSize.height*0.4f));
	_progCredit->setGlobalZOrder(9);
	this->addChild(_progCredit);

	_artCredit = (ui::Text*)rootNode->getChildByName("Art_Credits");
	_artCredit->setPosition(Vec2(winSize.width*0.45f, winSize.height*0.47f));
	_artCredit->setGlobalZOrder(9);
	this->addChild(_artCredit);

	_ldCredit = (ui::Text*)rootNode->getChildByName("LD_Credits");
	_ldCredit->setPosition(Vec2(winSize.width*0.70f, winSize.height*0.47f));
	_ldCredit->setGlobalZOrder(9);
	this->addChild(_ldCredit);

	_rotatePlanetTimerDefault = 240.0f;
	_rotatePlanetTimer = 0.0f;

	/*if (GameManager::sharedGameManager()->getIsGameMuted() == false)

	{
		auEngine->ResumeBackgroundMusic();
	}*/

	return true;
}

void CreditsScene::update(float delta)
{
	_planet->setRotation(_planet->getRotation() + (M_PI / _rotatePlanetTimerDefault));
}

void CreditsScene::ReturnButtonPressed(Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CCLOG("In touch! %d", type);

	if (type == cocos2d::ui::Widget::TouchEventType::ENDED)
	{
		CCLOG("touch ended.");
		this->ReturnToMenu();
	}
}

void CreditsScene::ReturnToMenu()
{
	//auEngine->PauseBackgroundMusic();
	Scene* scene = MenuScene::createScene();

	Director::getInstance()->replaceScene(TransitionFade::create(1, scene));
}

void CreditsScene::RotatePlanet()
{
	auto rotateTo = RotateTo::create(2.0f, 320.0f);
	_planet->runAction(rotateTo);
}

//Touch Functions
bool CreditsScene::onTouchBegan(Touch* touch, Event* event)
{
	cocos2d::log("touch began");
	return true;
}

void CreditsScene::onTouchEnded(Touch* touch, Event* event)
{
	cocos2d::log("touch ended");

	/*Vec2 touchPosition = touch->getLocation();

	int realMuteWidth = _muteButton->getContentSize().width * _muteButton->getScaleX();
	int realMuteHeight = _muteButton->getContentSize().height * _muteButton->getScaleY();

	if (touchPosition.x < _muteButton->getPositionX() + (realMuteWidth / 2)
		&& touchPosition.x > _muteButton->getPositionX() - (realMuteWidth / 2)
		&& touchPosition.y < _muteButton->getPositionY() + (realMuteHeight / 2)
		&& touchPosition.y > _muteButton->getPositionY() - (realMuteHeight / 2))
	{
		MuteButtonPressed();
	}*/
}

void CreditsScene::onTouchMoved(Touch* touch, Event* event)
{
	cocos2d::log("touch moved");
}

void CreditsScene::onTouchCancelled(Touch* touch, Event* event)
{
	cocos2d::log("touch cancelled");
}
