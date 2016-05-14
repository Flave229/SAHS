#include "SceneManager.h"
USING_NS_CC;

using namespace cocostudio::timeline;

SceneManager::SceneManager(int level)
{
	_level = level;
	_gravityOrientation = 0;
}

SceneManager* SceneManager::createScene(int level)
{
	// 'scene' is an autorelease object
	SceneManager* scene = new SceneManager(level);

	if (!scene->init()) {
		return nullptr;
	}

	// 'layer' is an autorelease object
	auto layer = SceneManager::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool SceneManager::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Scene::init())
	{
		return false;
	}

	auto winSize = Director::getInstance()->getVisibleSize(); //Gets the size of the screen
	Vec2 origin = Director::getInstance()->getVisibleOrigin(); //Gets the origin of the screen

	rootNode = CSLoader::createNode("Cocos_Scenes/Levels/Scene" + StringUtils::format("%d", _level) + ".csb");

	exists = CheckNode();

	if (!exists)
	{
		auto scene = MenuScene::createScene();
		return false;
	}

	addChild(rootNode);

	this->scheduleUpdate();

	// GAMEMANAGER
	GameManager::sharedGameManager()->setIsGameLive(true);
	GameManager::sharedGameManager()->setIsGamePaused(true);
	GameManager::sharedGameManager()->setIsObjectTouched(false);
	GameManager::sharedGameManager()->setCurrentLevel(_level);

	// BACKGROUND SETUP
	SetupBackground(rootNode);

	// TOUCH SETUP
	SetupTouches();

	// TIMER SETUP
	SetupTimer(rootNode);

	// AUDIO SETUP
	SetupAudio(rootNode);

	// SPRITE SETUP
	SetupSprites(rootNode);

	// HIGHLIGHT SETUP
	SetupHighlights(rootNode);

	// CLASS SETUP
	SetupClasses(rootNode);

	// BUTTON SETUP
	SetupButtons(rootNode);

	return true;
		
}

bool SceneManager::CheckNode()
{
	if (rootNode == nullptr)
	{
		return false;
	}
	else
	{
		return true;
	}
}

void SceneManager::SetupTouches()
{
	// SINGLE TOUCHES
	//Set up a touch listener.
	auto touchListener = EventListenerTouchOneByOne::create();

	//Set callbacks for our touch functions.
	touchListener->onTouchBegan = CC_CALLBACK_2(SceneManager::onTouchBegan, this);
	touchListener->onTouchEnded = CC_CALLBACK_2(SceneManager::onTouchEnded, this);
	touchListener->onTouchMoved = CC_CALLBACK_2(SceneManager::onTouchMoved, this);
	touchListener->onTouchCancelled = CC_CALLBACK_2(SceneManager::onTouchCancelled, this);

	//Add our touch listener to event listener list.
	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);

	//Setting up Multi Touch Listener
	touchMGR = new TouchManager;
	auto touchesListener = EventListenerTouchAllAtOnce::create();

	touchesListener->onTouchesBegan = CC_CALLBACK_2(TouchManager::onTouchesBegan, touchMGR);
	touchesListener->onTouchesEnded = CC_CALLBACK_2(TouchManager::onTouchesEnded, touchMGR);
	touchesListener->onTouchesMoved = CC_CALLBACK_2(TouchManager::onTouchesMoved, touchMGR);
	touchesListener->onTouchesCancelled = CC_CALLBACK_2(TouchManager::onTouchesCancelled, touchMGR);

	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchesListener, this);
	cocos2d::Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchesListener, this);
}

void SceneManager::SetupTimer(Node* root)
{
	// Get screen size
	auto winSize = Director::getInstance()->getVisibleSize();
	GameManager::sharedGameManager()->startLevelTimer();

	this->schedule(schedule_selector(SceneManager::ScheduleScore), 0.001f);
	int mil = GameManager::sharedGameManager()->getMil();
	int sec = GameManager::sharedGameManager()->getSec();
	int min = GameManager::sharedGameManager()->getMin();

	_timeLabel = (ui::Text*)root->getChildByName("Time");
	_timeLabel->setString(StringUtils::format("%d:%d:%d", min, sec, mil));
	_timeLabel->setPosition(Vec2(winSize.width * 0.5, winSize.height * 0.98));
}

void SceneManager::SetupAudio(Node* root)
{
	// Check if the game is muted before playing background music
	if (GameManager::sharedGameManager()->getIsGameMuted() == false) {
		auEngine->PlayBackgroundMusic("Sound/Music/Testing.mp3", true);
	}
}

void SceneManager::SetupButtons(Node* root)
{
	// Get screen size
	auto winSize = Director::getInstance()->getVisibleSize();
	GameManager::sharedGameManager()->startLevelTimer();

	// START BUTTON
	// Note: This is a debug button that may need removing for the final version
	_startGame = ui::Button::create("Art/Scene_Buttons/Button_Start_Frame.png", "Art/Scene_Buttons/Button_Start_Frame.png", "Art/Scene_Buttons/Button_Start_Frame.png");
	_startGame->addTouchEventListener(CC_CALLBACK_2(SceneManager::StartButtonPressed, this));
	_startGame->setPosition(Vec2(winSize.width*0.5, winSize.height*0.5));
	_startGame->setGlobalZOrder(10);
	this->addChild(_startGame);

	// Retry Button
	_retryButton = ui::Button::create();
	_retryButton->loadTextureNormal("Art/Level_UI/Button_Retry_Off.png");
	_retryButton->loadTexturePressed("Art/Level_UI/Button_Retry_On.png");
	_retryButton->loadTextureDisabled("Art/Level_UI/Button_Retry_On.png");
	_retryButton->addTouchEventListener(CC_CALLBACK_2(SceneManager::RetryButtonPressed, this));
	_retryButton->setPosition(Vec2(winSize.width - (_retryButton->getSize().width / 2), winSize.height - (_retryButton->getSize().height / 2)));

	addChild(_retryButton);

	// Back Button
	_backButton = ui::Button::create();
	_backButton->loadTextureNormal("Art/Level_UI/Button_Back_Off.png");
	_backButton->loadTexturePressed("Art/Level_UI/Button_Back_On.png");
	_backButton->loadTextureDisabled("Art/Level_UI/Button_Back_On.png");
	_backButton->addTouchEventListener(CC_CALLBACK_2(SceneManager::BackButtonPressed, this));
	_backButton->setPosition(Vec2(0.0f + (_backButton->getSize().width / 2), winSize.height - (_backButton->getSize().height / 2)));

	addChild(_backButton);
}

void SceneManager::SetupSprites(Node* root)
{
	// PLAYER
	_playerSprite = (Sprite*)root->getChildByName("Player");
	int i = 1;
	Sprite* tempSprite;

	// PLATFORMS
	while ((tempSprite = (Sprite*)root->getChildByName("Platform_" + StringUtils::format("%d", i))) != nullptr)
	{
		_platformSprites.push_back(tempSprite);
		i++;
	}

	//Horizontal Moving Platforms
	i = 1;
	while ((tempSprite = (Sprite*)root->getChildByName("MovingPlatformY_" + StringUtils::format("%d", i))) != nullptr)
	{
		_movingPlatformVertSprites.push_back(tempSprite);
		i++;
	}

	//Vertical Moving Platforms
	i = 1;
	while ((tempSprite = (Sprite*)root->getChildByName("MovingPlatformX_" + StringUtils::format("%d", i))) != nullptr)
	{
		_movingPlatformHorizSprites.push_back(tempSprite);
		i++;
	}

	//Horizontal Moving Walls
	i = 1;
	while ((tempSprite = (Sprite*)root->getChildByName("MovingWallY_" + StringUtils::format("%d", i))) != nullptr)
	{
		_movingWallVertSprites.push_back(tempSprite);
		i++;
	}

	//Vertical Moving Walls
	i = 1;
	while ((tempSprite = (Sprite*)root->getChildByName("MovingWallX_" + StringUtils::format("%d", i))) != nullptr)
	{
		_movingWallHorizSprites.push_back(tempSprite);
		i++;
	}

	// WALLS
	i = 1;
	while ((tempSprite = (Sprite*)root->getChildByName("Wall_" + StringUtils::format("%d", i))) != nullptr)
	{
		_wallSprites.push_back(tempSprite);
		i++;
	}

	// DOORS
	i = 1;
	while ((tempSprite = (Sprite*)root->getChildByName("Door_" + StringUtils::format("%d", i))) != nullptr)
	{
		_doorSprites.push_back(tempSprite);
		i++;
	}

	i = 1;
	while ((tempSprite = (Sprite*)root->getChildByName("Solid_Door_" + StringUtils::format("%d", i))) != nullptr)
	{
		_solidDoorSprites.push_back(tempSprite);
		i++;
	}

	// HATCHES
	i = 1;
	while ((tempSprite = (Sprite*)root->getChildByName("Hatch_" + StringUtils::format("%d", i))) != nullptr)
	{
		_hatchSprites.push_back(tempSprite);
		i++;
	}

	// WOODEN CRATES
	i = 1;
	while ((tempSprite = (Sprite*)root->getChildByName("Crate_Wooden_" + StringUtils::format("%d", i))) != nullptr)
	{
		_woodenSprites.push_back(tempSprite);
		i++;
	}

	// METAL CRATES
	i = 1;
	while ((tempSprite = (Sprite*)root->getChildByName("Crate_Metal_" + StringUtils::format("%d", i))) != nullptr)
	{
		_metalSprites.push_back(tempSprite);
		i++;
	}


	// SWITCHES
	// Down
	cocos2d::ui::CheckBox* tempCheck;
	i = 1;
	while ((tempCheck = static_cast<cocos2d::ui::CheckBox*>(root->getChildByName("Switch_Down_" + StringUtils::format("%d", i)))) != nullptr)
	{
		tempCheck->addTouchEventListener(CC_CALLBACK_2(SceneManager::SwitchPressed, this));
		_gravSwitchesDown.push_back(tempCheck);
		i++;
	}

	// Left
	i = 1;
	while ((tempCheck = static_cast<cocos2d::ui::CheckBox*>(root->getChildByName("Switch_Left_" + StringUtils::format("%d", i)))) != nullptr)
	{
		tempCheck->addTouchEventListener(CC_CALLBACK_2(SceneManager::SwitchPressed, this));
		_gravSwitchesLeft.push_back(tempCheck);
		i++;
	}

	// Up
	i = 1;
	while ((tempCheck = static_cast<cocos2d::ui::CheckBox*>(root->getChildByName("Switch_Up_" + StringUtils::format("%d", i)))) != nullptr)
	{
		tempCheck->addTouchEventListener(CC_CALLBACK_2(SceneManager::SwitchPressed, this));
		_gravSwitchesUp.push_back(tempCheck);
		i++;
	}

	// Right
	i = 1;
	while ((tempCheck = static_cast<cocos2d::ui::CheckBox*>(root->getChildByName("Switch_Right_" + StringUtils::format("%d", i)))) != nullptr)
	{
		tempCheck->addTouchEventListener(CC_CALLBACK_2(SceneManager::SwitchPressed, this));
		_gravSwitchesRight.push_back(tempCheck);
		i++;
	}

	//
	// Timer Switches
	i = 1;
	while ((tempCheck = static_cast<cocos2d::ui::CheckBox*>(root->getChildByName("SwitchTimer_Down_" + StringUtils::format("%d", i)))) != nullptr)
	{
		tempCheck->addTouchEventListener(CC_CALLBACK_2(SceneManager::SwitchTimerPressed, this));
		_timerSwitchesDown.push_back(tempCheck);
		i++;
	}

	// Left
	i = 1;
	while ((tempCheck = static_cast<cocos2d::ui::CheckBox*>(root->getChildByName("SwitchTimer_Left_" + StringUtils::format("%d", i)))) != nullptr)
	{
		tempCheck->addTouchEventListener(CC_CALLBACK_2(SceneManager::SwitchTimerPressed, this));
		_timerSwitchesLeft.push_back(tempCheck);
		i++;
	}

	// Up
	i = 1;
	while ((tempCheck = static_cast<cocos2d::ui::CheckBox*>(root->getChildByName("SwitchTimer_Up_" + StringUtils::format("%d", i)))) != nullptr)
	{
		tempCheck->addTouchEventListener(CC_CALLBACK_2(SceneManager::SwitchTimerPressed, this));
		_timerSwitchesUp.push_back(tempCheck);
		i++;
	}

	// Right
	i = 1;
	while ((tempCheck = static_cast<cocos2d::ui::CheckBox*>(root->getChildByName("SwitchTimer_Right_" + StringUtils::format("%d", i)))) != nullptr)
	{
		tempCheck->addTouchEventListener(CC_CALLBACK_2(SceneManager::SwitchTimerPressed, this));
		_timerSwitchesRight.push_back(tempCheck);
		i++;
	}

	// EXIT DOOR
	cocos2d::ui::CheckBox* exit;
	i = 1;
	while ((exit = static_cast<ui::CheckBox*>(root->getChildByName("Exit_" + StringUtils::format("%d", i)))) != nullptr) {

		exit->addTouchEventListener(CC_CALLBACK_2(SceneManager::DoorPressed, this));
		exit->setEnabled(false);
		_exit.push_back(exit);

		i++;
		exit = static_cast<ui::CheckBox*>(root->getChildByName("Exit_" + StringUtils::format("%d", i)));
	}

	// DOWN BUTTONS
	cocos2d::Sprite* tempButton;
	i = 1;
	while ((tempButton = (Sprite*)(root->getChildByName("Button_Down_" + StringUtils::format("%d", i)))) != nullptr)
	{
		_downButtons.push_back(tempButton);
		i++;
	}

	// LEFT BUTTONS
	i = 1;
	while ((tempButton = (Sprite*)(root->getChildByName("Button_Left_" + StringUtils::format("%d", i)))) != nullptr)
	{
		_leftButtons.push_back(tempButton);
		i++;
	}

	// UP BUTTONS
	i = 1;
	while ((tempButton = (Sprite*)(root->getChildByName("Button_Up_" + StringUtils::format("%d", i)))) != nullptr)
	{
		_upButtons.push_back(tempButton);
		i++;
	}

	// RIGHT BUTTONS
	i = 1;
	while ((tempButton = (Sprite*)(root->getChildByName("Button_Right_" + StringUtils::format("%d", i)))) != nullptr)
	{
		_rightButtons.push_back(tempButton);
		i++;
	}

	// Rail Start
	cocos2d::Sprite* tempRailStart;
	i = 1;
	while ((tempRailStart = (Sprite*)(root->getChildByName("RailStart_" + StringUtils::format("%d", i)))) != nullptr)
	{
		_railStart.push_back(tempRailStart);
		i++;
	}

	//Rail end
	cocos2d::Sprite* tempRailEnd;
	i = 1;
	while ((tempRailEnd = (Sprite*)(root->getChildByName("RailEnd_" + StringUtils::format("%d", i)))) != nullptr)
	{
		_railEnd.push_back(tempRailEnd);
		i++;
	}

	// Crate Spawn zones
	cocos2d::Sprite* tempWoodSpawn;
	i = 1;
	while ((tempWoodSpawn = (Sprite*)(root->getChildByName("WoodBoxSpawn_" + StringUtils::format("%d", i)))) != nullptr)
	{
		_woodCrateSpawn.push_back(tempWoodSpawn);
		i++;
	}

	cocos2d::Sprite* tempMetalSpawn;
	i = 1;
	while ((tempMetalSpawn = (Sprite*)(root->getChildByName("MetalBoxSpawn_" + StringUtils::format("%d", i)))) != nullptr)
	{
		_metalCrateSpawn.push_back(tempMetalSpawn);
		i++;
	}

	// Player Spawn Point
	_playerSpawn = (Sprite*)root->getChildByName("PlayerSpawn");
}

void SceneManager::SetupBackground(Node* root)
{
	// Get screen size
	auto winSize = Director::getInstance()->getVisibleSize();

	_background1 = Sprite::create("Art/Scene_Backgrounds/Bg_3.png");
	_background1->setPosition(Vec2(winSize.width*0.5f, winSize.height*0.5f));
	_background1->setScaleX(winSize.width / _background1->getContentSize().width);
	_background1->setScaleY(winSize.height / _background1->getContentSize().height);
	_background1->setLocalZOrder(-3);

	_background2 = Sprite::create("Art/Scene_Backgrounds/Bg_4.png");
	_background2->setPosition(Vec2(winSize.width*0.5f, winSize.height*0.5f));
	_background2->setScaleX(winSize.width / _background2->getContentSize().width);
	_background2->setScaleY(winSize.height / _background2->getContentSize().height);
	_background2->setLocalZOrder(-1);

	_background3 = Sprite::create("Art/Scene_Backgrounds/Bg_5.png");
	_background3->setPosition(Vec2(winSize.width*0.5f, winSize.height*0.5f));
	_background3->setScaleX(winSize.width / _background3->getContentSize().width);
	_background3->setScaleY(winSize.height / _background3->getContentSize().height);
	_background3->setLocalZOrder(-2);

	_background4 = Sprite::create("Art/Scene_Backgrounds/Bg_6.png");
	_background4->setPosition(Vec2(winSize.width*0.5f, winSize.height*0.5f));
	_background4->setScaleX(winSize.width / _background4->getContentSize().width);
	_background4->setScaleY(winSize.height / _background4->getContentSize().height);
	_background4->setLocalZOrder(-2);

	_blackTransparency = Sprite::create("Art/Scene_Backgrounds/Bg_7.png");
	_blackTransparency->setPosition(Vec2(winSize.width*0.5f, winSize.height*0.5f));
	_blackTransparency->setScaleX(winSize.width / _blackTransparency->getContentSize().width);
	_blackTransparency->setScaleY(winSize.height / _blackTransparency->getContentSize().height);
	_blackTransparency->setOpacity(123);
	_blackTransparency->setVisible(false);

	this->addChild(_background1);
	this->addChild(_background2);
	this->addChild(_background3);
	this->addChild(_background4);
	this->addChild(_blackTransparency);
}

void SceneManager::SetupHighlights(Node* root)
{
	// Get screen size
	auto winSize = Director::getInstance()->getVisibleSize();

	// SETUP SPRITES
	_topHighlight = Sprite::create("Art/Level_UI/Highlight.png");
	_topHighlight->setPosition(Vec2(winSize.width*0.5f, winSize.height - (_topHighlight->getContentSize().height / 2)));
	_topHighlight->setScaleX(winSize.width);
	_topHighlight->setOpacity(0);
	_topHighlight->setLocalZOrder(3);

	_rightHighlight = Sprite::create("Art/Level_UI/Highlight.png");
	_rightHighlight->setPosition(Vec2(winSize.width - (_rightHighlight->getContentSize().height / 2), winSize.height * 0.5f));
	_rightHighlight->setRotation(90.0f);
	_rightHighlight->setScaleX(winSize.height);
	_rightHighlight->setOpacity(0);
	_rightHighlight->setLocalZOrder(3);

	_bottomHighlight = Sprite::create("Art/Level_UI/Highlight.png");
	_bottomHighlight->setPosition(Vec2(winSize.width*0.5f, 0.0f + (_topHighlight->getContentSize().height / 2)));
	_bottomHighlight->setRotation(180.0f);
	_bottomHighlight->setScaleX(winSize.width);
	_bottomHighlight->setOpacity(0);
	_bottomHighlight->setLocalZOrder(3);

	_leftHighlight = Sprite::create("Art/Level_UI/Highlight.png");
	_leftHighlight->setPosition(Vec2(0.0f + (_rightHighlight->getContentSize().height / 2), winSize.height * 0.5f));
	_leftHighlight->setRotation(270.0f);
	_leftHighlight->setScaleX(winSize.height);
	_leftHighlight->setOpacity(0);
	_leftHighlight->setLocalZOrder(3);

	// SETUP TIMERS
	_topHighlightTime = 1.0f;
	_rightHighlightTime = 1.0f;
	_bottomHighlightTime = 1.0f;
	_leftHighlightTime = 1.0f;

	// SETUP ENABLES
	_topHighlightEnabled = false;
	_rightHighlightEnabled = false;
	_bottomHighlightEnabled = false;
	_leftHighlightEnabled = false;

	// ADD TO THIS
	this->addChild(_topHighlight);
	this->addChild(_leftHighlight);
	this->addChild(_bottomHighlight);
	this->addChild(_rightHighlight);
}

void SceneManager::SetupClasses(Node* root)
{
	// PLAYER
	ParticleModel* particle = new ParticleModel();
	particle->SetMass(20.0f);
	particle->AddForce("Gravity", Vec2(0.0f, -9.81f));

	_player = new Player(particle);
	_player->SetSprite(_playerSprite);
	_player->setName("Player");
	addChild(_player);

	// WOODEN CRATES
	for (int i = 0; i < (int)_woodenSprites.size(); i++) 
	{
		ParticleModel* particle = new ParticleModel();
		particle->SetMass(20.0f);
		particle->AddForce("Gravity", Vec2(0.0f, -9.81f));

		Box* box = new Box(particle, 1.0f);
		box->setName("Crate_Wooden_" + StringUtils::format("%d", i + 1));
		box->SetSprite(_woodenSprites[i]);

		_woodBoxes.push_back(box);

		addChild(box);
	}

	// METAL CRATES
	for (int i = 0; i < (int)_metalSprites.size(); i++) 
	{
		ParticleModel* particle = new ParticleModel();
		particle->SetMass(40.0f);
		particle->AddForce("Gravity", Vec2(0.0f, -9.81f));

		Box* box = new Box(particle, 2.0f);
		box->setName("Crate_Metal_" + StringUtils::format("%d", i + 1));
		box->SetSprite(_metalSprites[i]);

		_metalBoxes.push_back(box);

		addChild(box);
	}

	// SWITCHES
	// Down
	for (int i = 0; i < (int)_gravSwitchesDown.size(); i++) 
	{
		Switch* gravSwitch = Switch::create();
		gravSwitch->setName("Switch_Down_" + StringUtils::format("%d", i + 1));
		gravSwitch->SetSprite(_gravSwitchesDown[i]);
		gravSwitch->SetOrientation(0);

		_switches.push_back(gravSwitch);

		addChild(gravSwitch);
	}

	// Left
	for (int i = 0; i < (int)_gravSwitchesLeft.size(); i++) 
	{
		Switch* gravSwitch = Switch::create();
		gravSwitch->setName("Switch_Left_" + StringUtils::format("%d", i + 1));
		gravSwitch->SetSprite(_gravSwitchesLeft[i]);
		gravSwitch->SetOrientation(1);

		_switches.push_back(gravSwitch);

		addChild(gravSwitch);
	}

	// Up
	for (int i = 0; i < (int)_gravSwitchesUp.size(); i++) 
	{
		Switch* gravSwitch = Switch::create();
		gravSwitch->setName("Switch_Up_" + StringUtils::format("%d", i + 1));
		gravSwitch->SetSprite(_gravSwitchesUp[i]);
		gravSwitch->SetOrientation(2);

		_switches.push_back(gravSwitch);

		addChild(gravSwitch);
	}

	// Right
	for (int i = 0; i < (int)_gravSwitchesRight.size(); i++) 
	{
		Switch* gravSwitch = Switch::create();
		gravSwitch->setName("Switch_Right_" + StringUtils::format("%d", i + 1));
		gravSwitch->SetSprite(_gravSwitchesRight[i]);
		gravSwitch->SetOrientation(3);

		_switches.push_back(gravSwitch);

		addChild(gravSwitch);
	}

	// PLATFORMS
	for (int i = 0; i < (int)_platformSprites.size(); i++)
	{
		ParticleModel* particle = new ParticleModel();
		particle->SetMass(100.0f);

		GameObject* platform = new GameObject(particle);
		platform->setName("Platform_" + StringUtils::format("%d", i + 1));
		platform->SetSprite(_platformSprites.at(i));

		_platforms.push_back(platform);

		addChild(platform);
	}

	// WALLS
	for (int i = 0; i < (int)_wallSprites.size(); i++)
	{
		ParticleModel* particle = new ParticleModel();
		particle->SetMass(100.0f);

		GameObject* wall = new GameObject(particle);
		wall->setName("Wall_" + StringUtils::format("%d", i + 1));
		wall->SetSprite(_wallSprites.at(i));

		_walls.push_back(wall);

		addChild(wall);
	}

	// MOVING PLATFORMS - HORIZONTAL
	for (int i = 0; i < (int)_movingPlatformHorizSprites.size(); i++)
	{
		ParticleModel* particle = new ParticleModel();
		particle->SetMass(100.0f);

		MovablePlatform* movingPlats = new MovablePlatform(particle, _player, _woodBoxes, _metalBoxes);
		movingPlats->setName("MovingPlatform_" + StringUtils::format("%d", i + 1));
		movingPlats->SetSprite(_movingPlatformHorizSprites.at(i));
		movingPlats->SetZoneSprite();

		_movingPlatformsHoriz.push_back(movingPlats);

		addChild(movingPlats);
	}

	// MOVING PLATFORMS - VERTICAL
	for (int i = 0; i < (int)_movingPlatformVertSprites.size(); i++)
	{
		ParticleModel* particle = new ParticleModel();
		particle->SetMass(100.0f);

		MovablePlatform* movingPlats = new MovablePlatform(particle, _player, _woodBoxes, _metalBoxes);
		movingPlats->setName("MovingPlatform_" + StringUtils::format("%d", i + 1));
		movingPlats->SetSprite(_movingPlatformVertSprites.at(i));
		movingPlats->SetZoneSprite();

		_movingPlatformsVert.push_back(movingPlats);

		addChild(movingPlats);
	}

	// MOVING WALLS - HORIZONTAL
	for (int i = 0; i < (int)_movingWallHorizSprites.size(); i++)
	{
		ParticleModel* particle = new ParticleModel();
		particle->SetMass(100.0f);

		MovableWall* movingWall = new MovableWall(particle, _player, _woodBoxes, _metalBoxes);
		movingWall->setName("MovingWall_" + StringUtils::format("%d", i + 1));
		movingWall->SetSprite(_movingWallHorizSprites[i]);
		movingWall->SetZoneSprite();

		_movingWallsHoriz.push_back(movingWall);

		addChild(movingWall);
	}

	// MOVING WALLS - VERTICAL
	for (int i = 0; i < (int)_movingWallVertSprites.size(); i++)
	{
		ParticleModel* particle = new ParticleModel();
		particle->SetMass(100.0f);

		MovableWall* movingWall = new MovableWall(particle, _player, _woodBoxes, _metalBoxes);
		movingWall->setName("MovingWall_" + StringUtils::format("%d", i + 1));
		movingWall->SetSprite(_movingWallVertSprites[i]);
		movingWall->SetZoneSprite();

		_movingWallsVert.push_back(movingWall);

		addChild(movingWall);
	}

	// FLOOR BUTTONS - DOWN
	for (int i = 0; i < (int)_downButtons.size(); i++) 
	{
		ParticleModel* particle = new ParticleModel();
		particle->GetBoundingBox()->SetAdjustmentScaler(1.0f, 1.0f, 0.1f, 1.0f);
		particle->SetMass(50.0f);

		FloorButton* button = new FloorButton(particle, 0);
		button->setName("Button_Down_" + StringUtils::format("%d", i + 1));
		button->SetSprite(_downButtons[i]);
		button->SetActive(false);

		_buttons.push_back(button);

		addChild(button);
	}

	// FLOOR BUTTONS - LEFT
	for (int i = 0; i < (int)_leftButtons.size(); i++) 
	{
		ParticleModel* particle = new ParticleModel();
		particle->GetBoundingBox()->SetAdjustmentScaler(0.1f, 1.0f, 1.0f, 1.0f);
		particle->SetMass(50.0f);

		FloorButton* button = new FloorButton(particle, 1);
		button->setName("Button_Left_" + StringUtils::format("%d", i + 1));
		button->SetSprite(_leftButtons[i]);
		button->SetActive(false);

		_buttons.push_back(button);

		addChild(button);
	}

	// FLOOR BUTTONS - UP
	for (int i = 0; i < (int)_upButtons.size(); i++) 
	{
		ParticleModel* particle = new ParticleModel();
		particle->GetBoundingBox()->SetAdjustmentScaler(1.0f, 1.0f, 1.0f, 0.1f);
		particle->SetMass(50.0f);

		FloorButton* button = new FloorButton(particle, 2);
		button->setName("Button_Up_" + StringUtils::format("%d", i + 1));
		button->SetSprite(_upButtons[i]);
		button->SetActive(false);

		_buttons.push_back(button);

		addChild(button);
	}

	// FLOOR BUTTONS - RIGHT
	for (int i = 0; i < (int)_rightButtons.size(); i++) 
	{
		ParticleModel* particle = new ParticleModel();
		particle->GetBoundingBox()->SetAdjustmentScaler(1.0f, 0.1f, 1.0f, 1.0f);
		particle->SetMass(50.0f);

		FloorButton* button = new FloorButton(particle, 3);
		button->setName("Button_Right_" + StringUtils::format("%d", i + 1));
		button->SetSprite(_rightButtons[i]);
		button->SetActive(false);

		_buttons.push_back(button);

		addChild(button);
	}

	// DOORS
	for (int i = 0; i < (int)_doorSprites.size(); i++) 
	{
		Door* door = Door::create();
		door->setName("Door_" + StringUtils::format("%d", i + 1));
		door->SetSprite(_buttons, _doorSprites[i]);

		_doors.push_back(door);

		addChild(door);
	}

	// HATCHES
	for (int i = 0; i < (int)_hatchSprites.size(); i++) 
	{
		Door* hatch = Door::create();
		hatch->setName("Hatch_" + StringUtils::format("%d", i + 1));
		hatch->SetSprite(_buttons, _hatchSprites[i]);

		_hatches.push_back(hatch);

		addChild(hatch);
	}

	//Timer Switches Down
	for (int i = 0; i < (int)_timerSwitchesDown.size(); i++) 
	{
		SwitchTimer* gravSwitch = SwitchTimer::create();
		gravSwitch->setName("SwitchTimer_Down_" + StringUtils::format("%d", i + 1));
		gravSwitch->SetSprite(_timerSwitchesDown[i]);
		gravSwitch->SetOrientation(0);

		_tSwitches.push_back(gravSwitch);

		addChild(gravSwitch);
	}

	//Timer Switches Left
	for (int i = 0; i < (int)_timerSwitchesLeft.size(); i++) 
	{
		SwitchTimer* gravSwitch = SwitchTimer::create();
		gravSwitch->setName("SwitchTimer_Left_" + StringUtils::format("%d", i + 1));
		gravSwitch->SetSprite(_timerSwitchesLeft[i]);
		gravSwitch->SetOrientation(1);

		_tSwitches.push_back(gravSwitch);

		addChild(gravSwitch);
	}

	//Timer Switches Up
	for (int i = 0; i < (int)_timerSwitchesUp.size(); i++) 
	{
		SwitchTimer* gravSwitch = SwitchTimer::create();
		gravSwitch->setName("SwitchTimer_Up_" + StringUtils::format("%d", i + 1));
		gravSwitch->SetSprite(_timerSwitchesUp[i]);
		gravSwitch->SetOrientation(2);

		_tSwitches.push_back(gravSwitch);

		addChild(gravSwitch);
	}

	//Timer Switches Right
	for (int i = 0; i < (int)_timerSwitchesRight.size(); i++) 
	{
		SwitchTimer* gravSwitch = SwitchTimer::create();
		gravSwitch->setName("SwitchTimer_Right_" + StringUtils::format("%d", i + 1));
		gravSwitch->SetSprite(_timerSwitchesRight[i]);
		gravSwitch->SetOrientation(3);

		_tSwitches.push_back(gravSwitch);

		addChild(gravSwitch);
	}
}

void SceneManager::ScheduleScore(float delta)
{
	if (!GameManager::sharedGameManager()->getIsGamePaused())
	{
		GameManager::sharedGameManager()->updateLevelTimer();
	}
}

void SceneManager::update(float delta)
{
	if (!GameManager::sharedGameManager()->getIsGamePaused())
	{
		if (GameManager::sharedGameManager()->getIsGameLive())
		{
			if (_flipGravityCooldown > 0.0f) 
			{
				_flipGravityCooldown -= delta;

				if (_flipGravityCooldown < 0.0f) 
				{
					_flipGravityCooldown = 0.0f;
				}
			}

			_player->Update(delta);

			for (int i = 0; i < _woodBoxes.size(); i++)
			{
				_woodBoxes.at(i)->Update(delta);
			}
			for (int i = 0; i < _metalBoxes.size(); i++)
			{
				_metalBoxes.at(i)->Update(delta);
			}
			for (int i = 0; i < _platforms.size(); i++)
			{
				_platforms.at(i)->Update(delta);
			}
			for (int i = 0; i < _movingPlatformsHoriz.size(); i++)
			{
				_movingPlatformsHoriz.at(i)->Update(delta);
			}

			_score = GameManager::sharedGameManager()->getTimer();

			int mil = GameManager::sharedGameManager()->getMil();
			int sec = GameManager::sharedGameManager()->getSec();
			int min = GameManager::sharedGameManager()->getMin();

			std::string X = "X";
			std::string Y = "Y";
			/*int playerPosXFl = _player->GetSprite()->getPositionX();
			int playerPosYFl = _player->GetSprite()->getPositionY();*/

			_timeLabel->setString(StringUtils::format("%d:%d:%d", min, sec, mil));

			CheckCollisions();
			CheckNearTimer(delta);
			RevertGravity();
			CheckNear(delta);
			CheckNearDoor(delta);
			IsPlayerInBounds();
		}
		else
		{
			//GameManager::sharedGameManager()->setIsGamePaused(false);
			GameManager::sharedGameManager()->setIsGameLive(false);
			auto scene = GameOverScene::createScene();
			Director::getInstance()->replaceScene(TransitionFade::create(0.0f, scene));
		}
	}
}

void SceneManager::RevertGravity()
{
	for (int i = 0; i < (int)_tSwitches.size(); i++)
	{
		if (_tSwitches[i]->GetRevertGravity() == true)
		{
			FlipGravity(_tSwitches[i]->SwitchGravity());
		}
	}
}

void SceneManager::CheckCollisions()
{
	// PLATFORM COLLISIONS
	for (int i = 0; i < (int)_platforms.size(); i++) 
	{
		_player->CheckCollision(_platforms.at(i), 0.1, true, false);

		for (int i2 = 0; i2 < (int)_woodBoxes.size(); i2++) 
		{
			_woodBoxes.at(i2)->CheckCollision(_platforms.at(i), 0.6, true, false);
		}

		for (int i2 = 0; i2 < (int)_metalBoxes.size(); i2++) 
		{
			_metalBoxes.at(i2)->CheckCollision(_platforms.at(i), 0.4, true, false);
		}
	}

	// WALL COLLISIONS
	for (int i = 0; i < (int)_walls.size(); i++) 
	{
		_player->CheckCollision(_walls.at(i), 0.1, true, false);

		for (int i2 = 0; i2 < (int)_woodBoxes.size(); i2++) 
		{
			_woodBoxes.at(i2)->CheckCollision(_walls.at(i), 0.6, true, false);
		}

		for (int i2 = 0; i2 < (int)_metalBoxes.size(); i2++) 
		{
			_metalBoxes.at(i2)->CheckCollision(_walls.at(i), 0.4, true, false);
		}

		//for (int i2 = 0; i2 < (int)_movingPlatformsHoriz.size(); i2++)
		//{
		//	//_movingPlatformsHoriz[i2]->CheckCollision(_walls.at(i));
		//}

		//for (int i2 = 0; i2 < (int)_movingPlatformsVert.size(); i2++)
		//{
		//	//_movingPlatformsVert[i2]->CheckCollision(_walls.at(i));
		//}
	}

	// DOOR COLLISIONS
	for (int i = 0; i < (int)_doors.size(); i++) 
	{
		if (!_doors[i]->GetOpen()) 
		{
			//_player->CheckWallCollisions(_doors[i]->GetSprite());

			for (int i2 = 0; i2 < (int)_woodBoxes.size(); i2++) 
			{
				//_woodBoxes.at(i2)->CheckCollision(_doors.at(i)->GetParticleModel(), true, false);
				//_woodBoxes[i2]->CheckWallCollisions(_doors[i]->GetSprite());
			}

			for (int i2 = 0; i2 < (int)_metalBoxes.size(); i2++) 
			{
				//_metalBoxes.at(i2)->CheckCollision(_doors.at(i)->GetParticleModel(), true, false);
				//_metalBoxes[i2]->CheckWallCollisions(_doors[i]->GetSprite());
			}
		}
	}

	for (int i = 0; i < (int)_solidDoorSprites.size(); i++) 
	{
		//_player->CheckWallCollisions(_solidDoorSprites[i]);

		for (int i2 = 0; i2 < (int)_woodBoxes.size(); i2++) 
		{
			//_woodBoxes.at(i2)->CheckCollision(_solidDoorSprites.at(i)->GetParticleModel(), true, false);
			//_woodBoxes[i2]->CheckWallCollisions(_solidDoorSprites[i]);
		}

		for (int i2 = 0; i2 < (int)_metalBoxes.size(); i2++) 
		{
			//_metalBoxes.at(i2)->CheckCollision(_solidDoorSprites.at(i)->GetParticleModel(), true, false);
			//_metalBoxes[i2]->CheckWallCollisions(_solidDoorSprites[i]);
		}


	}

	// Hatch Collisions
	for (int i = 0; i < (int)_hatches.size(); i++) 
	{
		if (!_hatches[i]->GetOpen()) 
		{
			if (getName().find("Hatch"))
			{
				//_player->CheckPlatformCollisions(_hatches[i]->GetSprite());

				for (int i2 = 0; i2 < (int)_woodBoxes.size(); i2++) 
				{
					//_woodBoxes.at(i2)->CheckCollision(_hatches.at(i)->GetParticleModel(), true, false);
					//_woodBoxes[i2]->CheckPlatformCollisions(_hatches[i]->GetSprite());
				}

				for (int i2 = 0; i2 < (int)_metalBoxes.size(); i2++) 
				{
					//_metalBoxes.at(i2)->CheckCollision(_hatches.at(i)->GetParticleModel(), true, false);
					//_metalBoxes[i2]->CheckPlatformCollisions(_hatches[i]->GetSprite());
				}
			}
		}
	}

	// MOVING PLATFORM COLLISIONS
	for (int i = 0; i < (int)_movingPlatformsHoriz.size(); i++) 
	{
		_player->CheckCollision(_movingPlatformsHoriz.at(i), 0.1, true, false);

		for (int i2 = 0; i2 < (int)_woodBoxes.size(); i2++) 
		{
			_woodBoxes.at(i2)->CheckCollision(_movingPlatformsHoriz.at(i), 0.6, true, false);
		}

		for (int i2 = 0; i2 < (int)_metalBoxes.size(); i2++) 
		{
			_metalBoxes.at(i2)->CheckCollision(_movingPlatformsHoriz.at(i), 0.4, true, false);
		}
	}

	for (int i = 0; i < (int)_movingPlatformsVert.size(); i++) 
	{
		_player->CheckCollision(_movingPlatformsVert.at(i), 0.1, true, false);

		for (int i2 = 0; i2 < (int)_woodBoxes.size(); i2++) 
		{
			_woodBoxes.at(i2)->CheckCollision(_movingPlatformsVert.at(i), 0.6, true, false);
		}

		for (int i2 = 0; i2 < (int)_metalBoxes.size(); i2++) 
		{
			_metalBoxes.at(i2)->CheckCollision(_movingPlatformsVert.at(i), 0.4, true, false);
		}
	}

	// MOVING WALL COLLISIONS
	for (int i = 0; i < (int)_movingWallsHoriz.size(); i++) 
	{
		_player->CheckCollision(_movingWallsHoriz.at(i), 0.1, true, false);

		for (int i2 = 0; i2 < (int)_woodBoxes.size(); i2++) 
		{
			_woodBoxes.at(i2)->CheckCollision(_movingWallsHoriz.at(i), 0.6, true, false);
		}

		for (int i2 = 0; i2 < (int)_metalBoxes.size(); i2++) 
		{
			_metalBoxes.at(i2)->CheckCollision(_movingWallsHoriz.at(i), 0.4, true, false);
		}
	}

	for (int i = 0; i < (int)_movingWallsVert.size(); i++) 
	{
		_player->CheckCollision(_movingWallsVert.at(i), 0.1, true, false);
		//_player->CheckWallCollisions(_movingWallsVert[i]->getSprite());

		for (int i2 = 0; i2 < (int)_woodBoxes.size(); i2++) 
		{
			_woodBoxes.at(i2)->CheckCollision(_movingWallsVert.at(i), 0.6, true, false);
		}

		for (int i2 = 0; i2 < (int)_metalBoxes.size(); i2++) 
		{
			_metalBoxes.at(i2)->CheckCollision(_movingWallsVert.at(i), 0.4, true, false);
		}
	}

	// BUTTON COLLISIONS
	for (int i = 0; i < (int)_buttons.size(); i++) 
	{
		_buttons.at(i)->SetActive(false);

		if (_buttons.at(i)->CheckCollision(_player, 0.0f, false, true))
		{
			_player->FixCollision(_buttons.at(i));
		}

		for (int i2 = 0; i2 < (int)_woodBoxes.size(); i2++) 
		{
			if (_buttons.at(i)->CheckCollision(_woodBoxes.at(i2), 0.2f, false, true))
			{
				_woodBoxes.at(i2)->FixCollision(_buttons.at(i));
			}
		}

		for (int i2 = 0; i2 < (int)_metalBoxes.size(); i2++) 
		{
			if (_buttons.at(i)->CheckCollision(_metalBoxes.at(i2), 0.2f, false, true))
			{
				_metalBoxes.at(i2)->FixCollision(_buttons.at(i));
			}
		}
	}
}

//Touch Functions
bool SceneManager::onTouchBegan(Touch* touch, Event* event)
{
	cocos2d::log("touch began");

	if (GameManager::sharedGameManager()->getIsGameLive() == true) {
		//Store the coordinates of where this touch began.
		Point touchPos = touch->getLocationInView();
		touchPos = Director::getInstance()->convertToGL(touchPos);
		touchPos = convertToNodeSpace(touchPos);
		Rect currPlatform, currBox;
		Rect currTouchZone;
		_initialTouchPos = touchPos;
		_inTouch = true;

		int boxesSelected = 0;

		// Touch detection for horizontal moving platforms
		for (int i = 0; i < (int)_movingPlatformsHoriz.size(); i++)
		{
			currPlatform = _movingPlatformsHoriz[i]->GetSprite()->getBoundingBox();
			currTouchZone = _movingPlatformsHoriz[i]->GetTouchZone()->getBoundingBox();
			if (currPlatform.containsPoint(_initialTouchPos) || currTouchZone.containsPoint(_initialTouchPos))
			{
				GameManager::sharedGameManager()->setIsObjectTouched(true);
				
				_movingPlatformsHoriz[i]->PlatformType(1);
				_movingPlatformsHoriz[i]->onTouchBegan(touch, event);
				
			}
		}

		// Touch detection for vertical moving platforms
		for (int i = 0; i < (int)_movingPlatformsVert.size(); i++)
		{
			currPlatform = _movingPlatformsVert[i]->GetSprite()->getBoundingBox();
			currTouchZone = _movingPlatformsVert[i]->GetTouchZone()->getBoundingBox();
			if (currPlatform.containsPoint(_initialTouchPos) || currTouchZone.containsPoint(_initialTouchPos))
			{
				GameManager::sharedGameManager()->setIsObjectTouched(true);
				
				_movingPlatformsVert[i]->PlatformType(2);
				_movingPlatformsVert[i]->onTouchBegan(touch, event);
				
			}
		}

		// Touch detection for horizontal moving walls
		for (int i = 0; i < (int)_movingWallsHoriz.size(); i++)
		{
			currPlatform = _movingWallsHoriz[i]->GetSprite()->getBoundingBox();
			currTouchZone = _movingWallsHoriz[i]->GetTouchZone()->getBoundingBox();
			if (currPlatform.containsPoint(_initialTouchPos) || currTouchZone.containsPoint(_initialTouchPos))
			{
				GameManager::sharedGameManager()->setIsObjectTouched(true);
				
				_movingWallsHoriz[i]->PlatformType(1);
				_movingWallsHoriz[i]->onTouchBegan(touch, event);
				
			}
		}

		// Touch detection for vertical moving walls
		for (int i = 0; i < (int)_movingWallsVert.size(); i++)
		{
			currPlatform = _movingWallsVert[i]->GetSprite()->getBoundingBox();
			currTouchZone = _movingWallsVert[i]->GetTouchZone()->getBoundingBox();
			if (currPlatform.containsPoint(_initialTouchPos) || currTouchZone.containsPoint(_initialTouchPos))
			{
				GameManager::sharedGameManager()->setIsObjectTouched(true);
				
				_movingWallsVert[i]->PlatformType(2);
				_movingWallsVert[i]->onTouchBegan(touch, event);
				
			}
		}
		
		for (int i = 0; i < (int)_woodBoxes.size(); i++)
		{
			currBox = _woodBoxes[i]->GetSprite()->getBoundingBox();

			if (currBox.containsPoint(_initialTouchPos))
			{
				GameManager::sharedGameManager()->setIsObjectTouched(true);
				_woodBoxes[i]->Collision(touch);
				
			}
		}


		for (int i = 0; i < (int)_metalBoxes.size(); i++)
		{
			currBox = _metalBoxes[i]->GetSprite()->getBoundingBox();

			if (currBox.containsPoint(_initialTouchPos))
			{
				GameManager::sharedGameManager()->setIsObjectTouched(true);

				_metalBoxes[i]->Collision(touch);
			}
		}

		if (!GameManager::sharedGameManager()->getIsObjectTouched())
		{
			if (_woodBoxes.size() > 0)
			{
				for (int i = 0; i < _woodBoxes.size(); i++)
				{
					if (_woodBoxes[i]->getIsSelected())
					{
						boxesSelected++;
					}
				}
			}
			else if (_metalBoxes.size() > 0)
			{
				for (int i = 0; i < _metalBoxes.size(); i++)
				{
					if (_metalBoxes[i]->getIsSelected())
					{
						boxesSelected++;
						
					}
				}
			}

			
			if (boxesSelected == 0)
			{
				_player->SetTarget(_initialTouchPos);
			}
				
		}

		return true;
	}
	else {
		return false;
	}

	return true;
}

void SceneManager::onTouchEnded(Touch* touch, Event* event)
{
	cocos2d::log("touch ended");

	if (GameManager::sharedGameManager()->getIsGameLive() == true) {
		if (!GameManager::sharedGameManager()->getIsGamePaused()) {
			_inTouch = false;
			Rect currBox;
			// TODO
			// Add checks to ensure no object is clicked
			// If an object is clicked, DO NOT let the player move to it, instead:
			// call the appropiate methods specific to that object

			for (int i = 0; i < (int)_woodBoxes.size(); i++)
			{
				if (_woodBoxes[i]->getIsSelected())
				{
					_woodBoxes[i]->SetTotalDiff(touchMGR->totalDiff);
				}

			}


			for (int i = 0; i < (int)_metalBoxes.size(); i++)
			{
				if (_metalBoxes[i]->getIsSelected())
				{
					_metalBoxes[i]->SetTotalDiff(touchMGR->totalDiff);
				}


			}

			for (int i = 0; i < (int)_movingPlatformsHoriz.size(); i++)
			{
				_movingPlatformsHoriz[i]->onTouchEnded(touch, event);
			}

			for (int i = 0; i < (int)_movingPlatformsVert.size(); i++)
			{
				_movingPlatformsVert[i]->onTouchEnded(touch, event);
			}

			for (int i = 0; i < (int)_movingWallsHoriz.size(); i++)
			{
				_movingWallsHoriz[i]->onTouchEnded(touch, event);
			}

			for (int i = 0; i < (int)_movingWallsVert.size(); i++)
			{
				_movingWallsVert[i]->onTouchEnded(touch, event);
			}
		}
	}
}

void SceneManager::onTouchMoved(Touch* touch, Event* event)
{
	cocos2d::log("touch moved");


	for (int i = 0; i < (int)_movingPlatformsHoriz.size(); i++)
	{
		_movingPlatformsHoriz[i]->onTouchMoved(touch, event);
	}

	for (int i = 0; i < (int)_movingPlatformsVert.size(); i++)
	{
		_movingPlatformsVert[i]->onTouchMoved(touch, event);
	}

	for (int i = 0; i < (int)_movingWallsHoriz.size(); i++)
	{
		_movingWallsHoriz[i]->onTouchMoved(touch, event);
	}

	for (int i = 0; i < (int)_movingWallsVert.size(); i++)
	{
		_movingWallsVert[i]->onTouchMoved(touch, event);
	}
}

void SceneManager::onTouchCancelled(Touch* touch, Event* event)
{
	cocos2d::log("touch cancelled");
}

void SceneManager::SwitchPressed(Ref *sender, cocos2d::ui::Widget::TouchEventType type)
{
	// Find what switch has been clicked
	cocos2d::ui::CheckBox* findCheckBox = (cocos2d::ui::CheckBox*)sender;

	for (int i = 0; i < (int)_tSwitches.size(); i++) {
		if (_tSwitches.at(i)->IsTimerRunning()) {
			_tSwitches.at(i)->DisableSwitchTimer();
		}
	}

	for (int i = 0; i < (int)_switches.size(); i++) {
		if (findCheckBox->getName() == _switches.at(i)->GetSprite()->getName()) {
			// Flip Gravity
			if (_flipGravityCooldown == 0.0f) {
				FlipGravity(_switches.at(i)->GetOrientation());
				_flipGravityCooldown = 1.0f;
			}
		}
	}
}

void SceneManager::SwitchTimerPressed(Ref *sender, cocos2d::ui::Widget::TouchEventType type)
{
	// Find what switch has been clicked
	cocos2d::ui::CheckBox* findCheckBox = (cocos2d::ui::CheckBox*)sender;

	for (int i = 0; i < (int)_tSwitches.size(); i++) {
		if (findCheckBox->getName() == _tSwitches.at(i)->GetSprite()->getName()) {
			// Flip Gravity
			if (_flipGravityCooldown == 0.0f) {
				FlipGravity(_tSwitches.at(i)->GetOrientation());
				_tSwitches.at(i)->ResetTimer();
				_flipGravityCooldown = 1.0f;
			}
		}
		else {
			_tSwitches.at(i)->DisableSwitchTimer();
		}
	}
}

void SceneManager::DoorPressed(Ref *sender, cocos2d::ui::Widget::TouchEventType type)
{
	cocos2d::ui::CheckBox* findCheckBox = (cocos2d::ui::CheckBox*)sender;

	if (GameManager::sharedGameManager()->getIsGameLive()) {
		if (!GameManager::sharedGameManager()->getIsGamePaused()) {
			for (int i = 0; i < (int)_exit.size(); i++) {
				if (findCheckBox->getName() == _exit.at(i)->getName()) {
					auto scene = GameWinScene::createScene();
					Director::getInstance()->replaceScene(TransitionFade::create(0.5f, scene));
				}
			}
		}
	}
}

void SceneManager::BackButtonPressed(Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type == cocos2d::ui::Widget::TouchEventType::ENDED)
	{
		auto scene = LevelSelect::createScene();
		Director::getInstance()->replaceScene(TransitionFade::create(1.5f, scene));
	}
}

void SceneManager::RetryButtonPressed(Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type == cocos2d::ui::Widget::TouchEventType::ENDED)
	{
		auto scene = SceneManager::createScene(_level); //Gets the current level from SceneManager
		Director::getInstance()->replaceScene(TransitionFade::create(1.5f, scene));
	}
}

void SceneManager::StartButtonPressed(Ref* sender, cocos2d::ui::Widget::TouchEventType type)
{
	CCLOG("In Touch! %d", type);

	if (type == cocos2d::ui::Widget::TouchEventType::ENDED)
	{
		CCLOG("touch ended");

		_gravityOrientation = 0;

		_player->SetTarget(_player->GetParticleModel()->GetDisplacement());

		/*for (int i = 0; i < _woodBoxes.size(); i++)
		{
			_woodBoxes[i]->SetOrientationVertical(true);
		}

		for (int i = 0; i < _metalBoxes.size(); i++)
		{
			_metalBoxes[i]->SetOrientationVertical(true);
		}

		for (int i = 0; i < _metalBoxes.size(); i++)
		{
			_metalBoxes[i]->SetOrientationVertical(true);
		}*/

		GameManager::sharedGameManager()->setIsGamePaused(false);
		_startGame->setVisible(false);
	}
}

void SceneManager::CheckNear(float delta)
{
	bool inProximty = false;

	for (int i = 0; i < (int)_switches.size(); i++) {
		// Player needs to be near the switch to press
		float scaledWidth = _switches.at(i)->GetSprite()->getContentSize().width * _switches.at(i)->GetSprite()->getScaleX();
		float scaledHeight = _switches.at(i)->GetSprite()->getContentSize().height * _switches.at(i)->GetSprite()->getScaleY();

		if (_player->GetParticleModel()->GetDisplacementX() - (_player->GetSprite()->getContentSize().width / 2) < _switches.at(i)->GetSprite()->getPositionX() + (scaledWidth / 2) + (_player->GetSprite()->getContentSize().width / 2) + 20
			&& _player->GetParticleModel()->GetDisplacementX() + (_player->GetSprite()->getContentSize().width / 2) > _switches.at(i)->GetSprite()->getPositionX() - (scaledWidth / 2) - (_player->GetSprite()->getContentSize().width / 2) - 20
			&& _player->GetParticleModel()->GetDisplacementY() - (_player->GetSprite()->getContentSize().height / 2) < _switches.at(i)->GetSprite()->getPositionY() + (scaledHeight / 2) + (_player->GetSprite()->getContentSize().height / 2) + 20
			&& _player->GetParticleModel()->GetDisplacementY() + (_player->GetSprite()->getContentSize().height / 2) > _switches.at(i)->GetSprite()->getPositionY() - (scaledHeight / 2) - (_player->GetSprite()->getContentSize().height / 2) - 20)
		{
			//_switches.at(i)->GetSprite()->setEnabled(true);
			_switches.at(i)->GetSprite()->setEnabled(true);

			inProximty = true;

			if (_switches.at(i)->GetOrientation() == 0) {	// Down
				if (!_bottomHighlightEnabled) {
					_bottomHighlightTime = 1.0f;
					_bottomHighlight->runAction(FadeIn::create(_bottomHighlightTime));
					_bottomHighlightEnabled = true;
				}
			}
			else if (_switches.at(i)->GetOrientation() == 1) {	// Left
				if (!_leftHighlightEnabled) {
					_leftHighlightTime = 1.0f;
					_leftHighlight->runAction(FadeIn::create(_leftHighlightTime));
					_leftHighlightEnabled = true;
				}
			}
			else if (_switches.at(i)->GetOrientation() == 2) {	// Up
				if (!_topHighlightEnabled) {
					_topHighlightTime = 1.0f;
					_topHighlight->runAction(FadeIn::create(_topHighlightTime));
					_topHighlightEnabled = true;
				}
			}
			else if (_switches.at(i)->GetOrientation() == 3) {	// Right
				if (!_rightHighlightEnabled) {
					_rightHighlightTime = 1.0f;
					_rightHighlight->runAction(FadeIn::create(_rightHighlightTime));
					_rightHighlightEnabled = true;
				}
			}
		}
		else {
			_switches.at(i)->GetSprite()->setEnabled(false);
		}
	}

	if (_topHighlightEnabled) {
		if (_topHighlightTime > 0.0f) {
			_topHighlightTime -= delta;
		}
		else if (!inProximty) {
			_topHighlight->runAction(FadeOut::create(1.0f));
			_topHighlightEnabled = false;
		}
	}
	else if (_rightHighlightEnabled) {
		if (_rightHighlightTime > 0.0f) {
			_rightHighlightTime -= delta;
		}
		else if (!inProximty) {
			_rightHighlight->runAction(FadeOut::create(1.0f));
			_rightHighlightEnabled = false;
		}
	}
	else if (_bottomHighlightEnabled) {
		if (_bottomHighlightTime > 0.0f) {
			_bottomHighlightTime -= delta;
		}
		else if (!inProximty) {
			_bottomHighlight->runAction(FadeOut::create(1.0f));
			_bottomHighlightEnabled = false;
		}
	}
	else if (_leftHighlightEnabled) {
		if (_leftHighlightTime > 0.0f) {
			_leftHighlightTime -= delta;
		}
		else if (!inProximty) {
			_leftHighlight->runAction(FadeOut::create(1.0f));
			_leftHighlightEnabled = false;
		}
	}
}

void SceneManager::CheckNearTimer(float delta)
{
	bool inProximty = false;

	for (int i = 0; i < (int)_tSwitches.size(); i++) {
		// Player needs to be near the switch to press
		float scaledWidth = _tSwitches.at(i)->GetSprite()->getContentSize().width * _tSwitches.at(i)->GetSprite()->getScaleX();
		float scaledHeight = _tSwitches.at(i)->GetSprite()->getContentSize().height * _tSwitches.at(i)->GetSprite()->getScaleY();

		if (_player->GetParticleModel()->GetDisplacementX() - (_player->GetSprite()->getContentSize().width / 2) < _tSwitches.at(i)->GetSprite()->getPositionX() + (scaledWidth / 2) + (_player->GetSprite()->getContentSize().width / 2) + 20
			&& _player->GetParticleModel()->GetDisplacementX() + (_player->GetSprite()->getContentSize().width / 2) > _tSwitches.at(i)->GetSprite()->getPositionX() - (scaledWidth / 2) - (_player->GetSprite()->getContentSize().width / 2) - 20
			&& _player->GetParticleModel()->GetDisplacementY() - (_player->GetSprite()->getContentSize().height / 2) < _tSwitches.at(i)->GetSprite()->getPositionY() + (scaledHeight / 2) + (_player->GetSprite()->getContentSize().height / 2) + 20
			&& _player->GetParticleModel()->GetDisplacementY() + (_player->GetSprite()->getContentSize().height / 2) > _tSwitches.at(i)->GetSprite()->getPositionY() - (scaledHeight / 2) - (_player->GetSprite()->getContentSize().height / 2) - 20)
		{
			//_tSwitches.at(i)->GetSprite()->setEnabled(true);
			_tSwitches.at(i)->GetSprite()->setEnabled(true);

			inProximty = true;

			if (_tSwitches.at(i)->GetOrientation() == 0) {	// Down
				if (!_bottomHighlightEnabled) {
					_bottomHighlightTime = 1.0f;
					_bottomHighlight->runAction(FadeIn::create(_bottomHighlightTime));
					_bottomHighlightEnabled = true;
				}
			}
			else if (_tSwitches.at(i)->GetOrientation() == 1) {	// Left
				if (!_leftHighlightEnabled) {
					_leftHighlightTime = 1.0f;
					_leftHighlight->runAction(FadeIn::create(_leftHighlightTime));
					_leftHighlightEnabled = true;
				}
			}
			else if (_tSwitches.at(i)->GetOrientation() == 2) {	// Up
				if (!_topHighlightEnabled) {
					_topHighlightTime = 1.0f;
					_topHighlight->runAction(FadeIn::create(_topHighlightTime));
					_topHighlightEnabled = true;
				}
			}
			else if (_tSwitches.at(i)->GetOrientation() == 3) {	// Right
				if (!_rightHighlightEnabled) {
					_rightHighlightTime = 1.0f;
					_rightHighlight->runAction(FadeIn::create(_rightHighlightTime));
					_rightHighlightEnabled = true;
				}
			}
		}
		else {
			_tSwitches.at(i)->GetSprite()->setEnabled(false);
		}
	}

	if (_topHighlightEnabled) {
		if (_topHighlightTime > 0.0f) {
			_topHighlightTime -= delta;
		}
		else if (!inProximty) {
			_topHighlight->runAction(FadeOut::create(1.0f));
			_topHighlightEnabled = false;
		}
	}
	else if (_rightHighlightEnabled) {
		if (_rightHighlightTime > 0.0f) {
			_rightHighlightTime -= delta;
		}
		else if (!inProximty) {
			_rightHighlight->runAction(FadeOut::create(1.0f));
			_rightHighlightEnabled = false;
		}
	}
	else if (_bottomHighlightEnabled) {
		if (_bottomHighlightTime > 0.0f) {
			_bottomHighlightTime -= delta;
		}
		else if (!inProximty) {
			_bottomHighlight->runAction(FadeOut::create(1.0f));
			_bottomHighlightEnabled = false;
		}
	}
	else if (_leftHighlightEnabled) {
		if (_leftHighlightTime > 0.0f) {
			_leftHighlightTime -= delta;
		}
		else if (!inProximty) {
			_leftHighlight->runAction(FadeOut::create(1.0f));
			_leftHighlightEnabled = false;
		}
	}
}

void SceneManager::CheckNearDoor(float delta)
{
	for (int i = 0; i < (int)_exit.size(); i++) {
		float scaledWidth = _exit[i]->getContentSize().width * _exit[i]->getScaleX();
		float scaledHeight = _exit[i]->getContentSize().height * _exit[i]->getScaleY();

		if (_player->GetParticleModel()->GetDisplacementX() - (_player->GetSprite()->getContentSize().width / 2) < _exit[i]->getPositionX() + (scaledWidth / 2) + (_player->GetSprite()->getContentSize().width / 2) + 20
			&& _player->GetParticleModel()->GetDisplacementX() + (_player->GetSprite()->getContentSize().width / 2) > _exit[i]->getPositionX() - (scaledWidth / 2) - (_player->GetSprite()->getContentSize().width / 2) - 20
			&& _player->GetParticleModel()->GetDisplacementY() - (_player->GetSprite()->getContentSize().height / 2) < _exit[i]->getPositionY() + (scaledHeight / 2)
			&& _player->GetParticleModel()->GetDisplacementY() + (_player->GetSprite()->getContentSize().height / 2) > _exit[i]->getPositionY() - (scaledHeight / 2))
		{
			if (!_exit[i]->isEnabled()) {
				_exit[i]->setEnabled(true);
			}
		}
		else {
			if (_exit[i]->isEnabled()) {
				_exit[i]->setEnabled(false);
			}
		}
	}
}

void SceneManager::FlipGravity(int direction)
{
	// Flip Player
	if (_player->GetParticleModel()->GetForce("Gravity") != nullptr)
	{
		// Detach object from 'floor'
		if (_player->GetParticleModel()->GetForce("Gravity")->GetForce().y < 0.0f)
		{
			// Gravity is acting downwards, detach by moving upwards
			_player->GetParticleModel()->SetDisplacementY(_player->GetParticleModel()->GetDisplacementY() + 2.5f);
		}
		else if (_player->GetParticleModel()->GetForce("Gravity")->GetForce().y > 0.0f)
		{
			// Gravity is acting upwards, detach by moving downwards
			_player->GetParticleModel()->SetDisplacementY(_player->GetParticleModel()->GetDisplacementY() - 2.5f);
		}
		else if (_player->GetParticleModel()->GetForce("Gravity")->GetForce().x < 0.0f)
		{
			// Gravity is acting leftwards, detach by moving rightwards
			_player->GetParticleModel()->SetDisplacementX(_player->GetParticleModel()->GetDisplacementX() + 2.5f);
		}
		else if (_player->GetParticleModel()->GetForce("Gravity")->GetForce().x > 0.0f)
		{
			// Gravity is acting rightwards, detach by moving leftwards
			_player->GetParticleModel()->SetDisplacementX(_player->GetParticleModel()->GetDisplacementX() - 2.5f);
		}

		// Apply new force
		if (direction == 0) // Down
		{
			_player->GetParticleModel()->GetForce("Gravity")->SetForce(0.0f, -9.81f);
		}
		else if (direction == 1) // Left
		{
			_player->GetParticleModel()->GetForce("Gravity")->SetForce(-9.81f, 0.0f);
		}
		else if (direction == 2) // Up
		{
			_player->GetParticleModel()->GetForce("Gravity")->SetForce(0.0f, 9.81f);
		}
		else if (direction == 3) // Right
		{
			_player->GetParticleModel()->GetForce("Gravity")->SetForce(9.81f, 0.0f);
		}

		_player->FlipSprite();
	}

	// Flip Wooden Boxes
	for (int i = 0; i < _woodBoxes.size(); i++)
	{
		if (_woodBoxes.at(i)->GetParticleModel()->GetForce("Gravity") != nullptr)
		{
			// Detach object from 'floor'
			if (_woodBoxes.at(i)->GetParticleModel()->GetForce("Gravity")->GetForce().y < 0.0f)
			{
				// Gravity is acting downwards, detach by moving upwards
				_woodBoxes.at(i)->GetParticleModel()->SetDisplacementY(_woodBoxes.at(i)->GetParticleModel()->GetDisplacementY() + 2.5f);
			}
			else if (_woodBoxes.at(i)->GetParticleModel()->GetForce("Gravity")->GetForce().y > 0.0f)
			{
				// Gravity is acting upwards, detach by moving downwards
				_woodBoxes.at(i)->GetParticleModel()->SetDisplacementY(_woodBoxes.at(i)->GetParticleModel()->GetDisplacementY() - 2.5f);
			}
			else if (_woodBoxes.at(i)->GetParticleModel()->GetForce("Gravity")->GetForce().x < 0.0f)
			{
				// Gravity is acting leftwards, detach by moving rightwards
				_woodBoxes.at(i)->GetParticleModel()->SetDisplacementX(_woodBoxes.at(i)->GetParticleModel()->GetDisplacementX() + 2.5f);
			}
			else if (_woodBoxes.at(i)->GetParticleModel()->GetForce("Gravity")->GetForce().x > 0.0f)
			{
				// Gravity is acting rightwards, detach by moving leftwards
				_woodBoxes.at(i)->GetParticleModel()->SetDisplacementX(_woodBoxes.at(i)->GetParticleModel()->GetDisplacementX() - 2.5f);
			}

			// Apply new force
			if (direction == 0) // Down
			{
				_woodBoxes.at(i)->GetParticleModel()->GetForce("Gravity")->SetForce(0.0f, -9.81f);
			}
			else if (direction == 1) // Left
			{
				_woodBoxes.at(i)->GetParticleModel()->GetForce("Gravity")->SetForce(-9.81f, 0.0f);
			}
			else if (direction == 2) // Up
			{
				_woodBoxes.at(i)->GetParticleModel()->GetForce("Gravity")->SetForce(0.0f, 9.81f);
			}
			else if (direction == 3) // Right
			{
				_woodBoxes.at(i)->GetParticleModel()->GetForce("Gravity")->SetForce(9.81f, 0.0f);
			}

			_woodBoxes.at(i)->FlipSprite();
		}
	}

	// Flip Metal Boxes
	for (int i = 0; i < _metalBoxes.size(); i++)
	{
		if (_metalBoxes.at(i)->GetParticleModel()->GetForce("Gravity") != nullptr)
		{
			// Detach object from 'floor'
			if (_metalBoxes.at(i)->GetParticleModel()->GetForce("Gravity")->GetForce().y < 0.0f)
			{
				// Gravity is acting downwards, detach by moving upwards
				_metalBoxes.at(i)->GetParticleModel()->SetDisplacementY(_metalBoxes.at(i)->GetParticleModel()->GetDisplacementY() + 2.5f);
			}
			else if (_metalBoxes.at(i)->GetParticleModel()->GetForce("Gravity")->GetForce().y > 0.0f)
			{
				// Gravity is acting upwards, detach by moving downwards
				_metalBoxes.at(i)->GetParticleModel()->SetDisplacementY(_metalBoxes.at(i)->GetParticleModel()->GetDisplacementY() - 2.5f);
			}
			else if (_metalBoxes.at(i)->GetParticleModel()->GetForce("Gravity")->GetForce().x < 0.0f)
			{
				// Gravity is acting leftwards, detach by moving rightwards
				_metalBoxes.at(i)->GetParticleModel()->SetDisplacementX(_metalBoxes.at(i)->GetParticleModel()->GetDisplacementX() + 2.5f);
			}
			else if (_metalBoxes.at(i)->GetParticleModel()->GetForce("Gravity")->GetForce().x > 0.0f)
			{
				// Gravity is acting rightwards, detach by moving leftwards
				_metalBoxes.at(i)->GetParticleModel()->SetDisplacementX(_metalBoxes.at(i)->GetParticleModel()->GetDisplacementX() - 2.5f);
			}

			// Apply new force
			if (direction == 0) // Down
			{
				_metalBoxes.at(i)->GetParticleModel()->GetForce("Gravity")->SetForce(0.0f, -9.81f);
			}
			else if (direction == 1) // Left
			{
				_metalBoxes.at(i)->GetParticleModel()->GetForce("Gravity")->SetForce(-9.81f, 0.0f);
			}
			else if (direction == 2) // Up
			{
				_metalBoxes.at(i)->GetParticleModel()->GetForce("Gravity")->SetForce(0.0f, 9.81f);
			}
			else if (direction == 3) // Right
			{
				_metalBoxes.at(i)->GetParticleModel()->GetForce("Gravity")->SetForce(9.81f, 0.0f);
			}

			_metalBoxes.at(i)->FlipSprite();
		}
	}

	//_previousDirection = _gravityOrientation;
	//_gravityOrientation = direction;

	//// Handle DOWN gravity
	//if (_previousDirection == 0) {
	//	if (direction == 1) { // Left
	//		_player->GetParticleModel()->SetDisplacementX(_player->GetParticleModel()->GetDisplacementX() - 2.5f);
	//		_player->GetParticleModel()->SetDisplacementY(_player->GetParticleModel()->GetDisplacementY() + 2.5f);
	//		//_player->GetSprite()->setPositionY(_player->GetSprite()->getPositionY() + 0.5f);
	//		//_player->GetSprite()->setPositionX(_player->GetSprite()->getPositionX() - 0.5f);

	//		for (int i = 0; i < (int)_woodBoxes.size(); i++) 
	//		{
	//			_woodBoxes.at(i)->GetParticleModel()->SetDisplacementX(_woodBoxes.at(i)->GetParticleModel()->GetDisplacementX() - 2.5f);
	//			_woodBoxes.at(i)->GetParticleModel()->SetDisplacementY(_woodBoxes.at(i)->GetParticleModel()->GetDisplacementY() + 2.5f);
	//			//_woodBoxes[i]->GetSprite()->setPositionY(_woodBoxes[i]->GetSprite()->getPositionY() + 2.5f);
	//			//_woodBoxes[i]->GetSprite()->setPositionX(_woodBoxes[i]->GetSprite()->getPositionX() - 2.5f);
	//		}

	//		for (int i = 0; i < (int)_metalBoxes.size(); i++) 
	//		{
	//			_metalBoxes.at(i)->GetParticleModel()->SetDisplacementX(_metalBoxes.at(i)->GetParticleModel()->GetDisplacementX() - 2.5f);
	//			_metalBoxes.at(i)->GetParticleModel()->SetDisplacementY(_metalBoxes.at(i)->GetParticleModel()->GetDisplacementY() + 2.5f);
	//			//_metalBoxes[i]->GetSprite()->setPositionY(_metalBoxes[i]->GetSprite()->getPositionY() + 2.5f);
	//			//_metalBoxes[i]->GetSprite()->setPositionX(_metalBoxes[i]->GetSprite()->getPositionX() - 2.5f);
	//		}

	//		_player->GetParticleModel()->GetForce("Gravity")->SetForce(Vec2(-9.81f, 0.0f));
	//		/*_player->SetGravity(-3.81f);
	//		_player->SetFallingVertical(false);
	//		_player->SetFallingHorizontal(true);*/

	//		for (int i = 0; i < (int)_woodBoxes.size(); i++) 
	//		{
	//			_woodBoxes.at(i)->GetParticleModel()->GetForce("Gravity")->SetForce(Vec2(-9.81f, 0.0f));
	//			/*_woodBoxes[i]->SetGravity(-3.81f);
	//			_woodBoxes[i]->SetFallingVertical(false);
	//			_woodBoxes[i]->SetFallingHorizontal(true);*/
	//		}

	//		for (int i = 0; i < (int)_metalBoxes.size(); i++) 
	//		{
	//			_metalBoxes.at(i)->GetParticleModel()->GetForce("Gravity")->SetForce(Vec2(-9.81f, 0.0f));
	//			/*_metalBoxes[i]->SetGravity(-3.81f);
	//			_metalBoxes[i]->SetFallingVertical(false);
	//			_metalBoxes[i]->SetFallingHorizontal(true);*/
	//		}
	//	}
	//	else if (direction == 2) { // Up
	//		_player->GetParticleModel()->SetDisplacementY(_player->GetParticleModel()->GetDisplacementY() + 0.5f);
	//		//_player->GetSprite()->setPositionY(_player->GetSprite()->getPositionY() + 0.5f);

	//		for (int i = 0; i < (int)_woodBoxes.size(); i++) 
	//		{
	//			_woodBoxes.at(i)->GetParticleModel()->SetDisplacementY(_woodBoxes.at(i)->GetParticleModel()->GetDisplacementY() + 2.5f);
	//			//_woodBoxes[i]->GetSprite()->setPositionY(_woodBoxes[i]->GetSprite()->getPositionY() + 2.5f);
	//		}

	//		for (int i = 0; i < (int)_metalBoxes.size(); i++) 
	//		{
	//			_metalBoxes.at(i)->GetParticleModel()->SetDisplacementY(_metalBoxes.at(i)->GetParticleModel()->GetDisplacementY() + 2.5f);
	//			//_metalBoxes[i]->GetSprite()->setPositionY(_metalBoxes[i]->GetSprite()->getPositionY() + 2.5f);
	//		}

	//		_player->GetParticleModel()->GetForce("Gravity")->SetForce(Vec2(0.0f, 9.81f));
	//		/*_player->SetGravity(3.81f);
	//		_player->SetFallingVertical(true);
	//		_player->SetFallingHorizontal(false);*/

	//		for (int i = 0; i < (int)_woodBoxes.size(); i++) 
	//		{
	//			_woodBoxes.at(i)->GetParticleModel()->GetForce("Gravity")->SetForce(Vec2(0.0f, 9.81f));
	//			/*_woodBoxes[i]->SetGravity(3.81f);
	//			_woodBoxes[i]->SetFallingVertical(true);
	//			_woodBoxes[i]->SetFallingHorizontal(false);*/
	//		}

	//		for (int i = 0; i < (int)_metalBoxes.size(); i++) 
	//		{
	//			_metalBoxes.at(i)->GetParticleModel()->GetForce("Gravity")->SetForce(Vec2(0.0f, 9.81f));
	//			/*_metalBoxes[i]->SetGravity(3.81f);
	//			_metalBoxes[i]->SetFallingVertical(true);
	//			_metalBoxes[i]->SetFallingHorizontal(false);*/
	//		}
	//	}
	//	else if (direction == 3) { // Right
	//		_player->GetParticleModel()->SetDisplacementX(_player->GetParticleModel()->GetDisplacementX() + 2.5f);
	//		_player->GetParticleModel()->SetDisplacementY(_player->GetParticleModel()->GetDisplacementY() + 2.5f);
	//		//_player->GetSprite()->setPositionY(_player->GetSprite()->getPositionY() + 0.5f);
	//		//_player->GetSprite()->setPositionX(_player->GetSprite()->getPositionX() + 0.5f);

	//		for (int i = 0; i < (int)_woodBoxes.size(); i++) 
	//		{
	//			_woodBoxes.at(i)->GetParticleModel()->SetDisplacementX(_woodBoxes.at(i)->GetParticleModel()->GetDisplacementX() + 2.5f);
	//			_woodBoxes.at(i)->GetParticleModel()->SetDisplacementY(_woodBoxes.at(i)->GetParticleModel()->GetDisplacementY() + 2.5f);
	//			//_woodBoxes[i]->GetSprite()->setPositionY(_woodBoxes[i]->GetSprite()->getPositionY() + 2.5f);
	//			//_woodBoxes[i]->GetSprite()->setPositionX(_woodBoxes[i]->GetSprite()->getPositionX() + 2.5f);
	//		}

	//		for (int i = 0; i < (int)_metalBoxes.size(); i++) 
	//		{
	//			_metalBoxes.at(i)->GetParticleModel()->SetDisplacementX(_metalBoxes.at(i)->GetParticleModel()->GetDisplacementX() + 2.5f);
	//			_metalBoxes.at(i)->GetParticleModel()->SetDisplacementY(_metalBoxes.at(i)->GetParticleModel()->GetDisplacementY() + 2.5f);
	//			//_metalBoxes[i]->GetSprite()->setPositionY(_metalBoxes[i]->GetSprite()->getPositionY() + 2.5f);
	//			//_metalBoxes[i]->GetSprite()->setPositionX(_metalBoxes[i]->GetSprite()->getPositionX() + 2.5f);
	//		}

	//		_player->GetParticleModel()->GetForce("Gravity")->SetForce(Vec2(9.81f, 0.0f));
	//		/*_player->SetGravity(3.81f);
	//		_player->SetFallingVertical(false);
	//		_player->SetFallingHorizontal(true);*/

	//		for (int i = 0; i < (int)_woodBoxes.size(); i++) 
	//		{
	//			_woodBoxes.at(i)->GetParticleModel()->GetForce("Gravity")->SetForce(Vec2(9.81f, 0.0f));
	//			/*_woodBoxes[i]->SetGravity(3.81f);
	//			_woodBoxes[i]->SetFallingVertical(false);
	//			_woodBoxes[i]->SetFallingHorizontal(true);*/
	//		}

	//		for (int i = 0; i < (int)_metalBoxes.size(); i++) 
	//		{
	//			_metalBoxes.at(i)->GetParticleModel()->GetForce("Gravity")->SetForce(Vec2(9.81f, 0.0f));
	//			/*_metalBoxes[i]->SetGravity(3.81f);
	//			_metalBoxes[i]->SetFallingVertical(false);
	//			_metalBoxes[i]->SetFallingHorizontal(true);*/
	//		}
	//	}
	//}
	//// Handle LEFT gravity
	//else if (_previousDirection == 1) {
	//	if (direction == 0) { // Down
	//		_player->GetParticleModel()->SetDisplacementX(_player->GetParticleModel()->GetDisplacementX() + 2.5f);
	//		_player->GetParticleModel()->SetDisplacementY(_player->GetParticleModel()->GetDisplacementY() - 2.5f);
	//		//_player->GetSprite()->setPositionY(_player->GetSprite()->getPositionY() - 2.5f);
	//		//_player->GetSprite()->setPositionX(_player->GetSprite()->getPositionX() + 2.5f);

	//		for (int i = 0; i < (int)_woodBoxes.size(); i++) 
	//		{
	//			_woodBoxes.at(i)->GetParticleModel()->SetDisplacementX(_woodBoxes.at(i)->GetParticleModel()->GetDisplacementX() + 2.5f);
	//			_woodBoxes.at(i)->GetParticleModel()->SetDisplacementY(_woodBoxes.at(i)->GetParticleModel()->GetDisplacementY() - 2.5f);
	//			//_woodBoxes[i]->GetSprite()->setPositionY(_woodBoxes[i]->GetSprite()->getPositionY() - 2.5f);
	//			//_woodBoxes[i]->GetSprite()->setPositionX(_woodBoxes[i]->GetSprite()->getPositionX() + 2.5f);
	//		}

	//		for (int i = 0; i < (int)_metalBoxes.size(); i++) 
	//		{
	//			_metalBoxes.at(i)->GetParticleModel()->SetDisplacementX(_metalBoxes.at(i)->GetParticleModel()->GetDisplacementX() + 2.5f);
	//			_metalBoxes.at(i)->GetParticleModel()->SetDisplacementY(_metalBoxes.at(i)->GetParticleModel()->GetDisplacementY() - 2.5f);
	//			//_metalBoxes[i]->GetSprite()->setPositionY(_metalBoxes[i]->GetSprite()->getPositionY() - 2.5f);
	//			//_metalBoxes[i]->GetSprite()->setPositionX(_metalBoxes[i]->GetSprite()->getPositionX() + 2.5f);
	//		}

	//		_player->GetParticleModel()->GetForce("Gravity")->SetForce(Vec2(0.0f, -9.81f));
	//		/*_player->SetGravity(-3.81f);
	//		_player->SetFallingVertical(true);
	//		_player->SetFallingHorizontal(false);*/

	//		for (int i = 0; i < (int)_woodBoxes.size(); i++) 
	//		{
	//			_woodBoxes.at(i)->GetParticleModel()->GetForce("Gravity")->SetForce(Vec2(0.0f, -9.81f));
	//			/*_woodBoxes[i]->SetGravity(-3.81f);
	//			_woodBoxes[i]->SetFallingVertical(true);
	//			_woodBoxes[i]->SetFallingHorizontal(false);*/
	//		}

	//		for (int i = 0; i < (int)_metalBoxes.size(); i++) 
	//		{
	//			_metalBoxes.at(i)->GetParticleModel()->GetForce("Gravity")->SetForce(Vec2(0.0f, -9.81f));
	//			/*_metalBoxes[i]->SetGravity(-3.81f);
	//			_metalBoxes[i]->SetFallingVertical(true);
	//			_metalBoxes[i]->SetFallingHorizontal(false);*/
	//		}
	//	}
	//	else if (direction == 2) { // Up
	//		_player->GetParticleModel()->SetDisplacementX(_player->GetParticleModel()->GetDisplacementX() + 2.5f);
	//		_player->GetParticleModel()->SetDisplacementY(_player->GetParticleModel()->GetDisplacementY() + 2.5f);
	//		//_player->GetSprite()->setPositionY(_player->GetSprite()->getPositionY() + 0.5f);
	//		//_player->GetSprite()->setPositionX(_player->GetSprite()->getPositionX() + 0.5f);

	//		for (int i = 0; i < (int)_woodBoxes.size(); i++) 
	//		{
	//			_woodBoxes.at(i)->GetParticleModel()->SetDisplacementX(_woodBoxes.at(i)->GetParticleModel()->GetDisplacementX() + 2.5f);
	//			_woodBoxes.at(i)->GetParticleModel()->SetDisplacementY(_woodBoxes.at(i)->GetParticleModel()->GetDisplacementY() + 2.5f);
	//			//_woodBoxes[i]->GetSprite()->setPositionY(_woodBoxes[i]->GetSprite()->getPositionY() + 2.5f);
	//			//_woodBoxes[i]->GetSprite()->setPositionX(_woodBoxes[i]->GetSprite()->getPositionX() + 2.5f);
	//		}

	//		for (int i = 0; i < (int)_metalBoxes.size(); i++) 
	//		{
	//			_metalBoxes.at(i)->GetParticleModel()->SetDisplacementX(_metalBoxes.at(i)->GetParticleModel()->GetDisplacementX() + 2.5f);
	//			_metalBoxes.at(i)->GetParticleModel()->SetDisplacementY(_metalBoxes.at(i)->GetParticleModel()->GetDisplacementY() + 2.5f);
	//			//_metalBoxes[i]->GetSprite()->setPositionY(_metalBoxes[i]->GetSprite()->getPositionY() + 2.5f);
	//			//_metalBoxes[i]->GetSprite()->setPositionX(_metalBoxes[i]->GetSprite()->getPositionX() + 2.5f);
	//		}

	//		_player->GetParticleModel()->GetForce("Gravity")->SetForce(Vec2(0.0f, 9.81f));
	//		/*_player->SetGravity(3.81f);
	//		_player->SetFallingVertical(true);
	//		_player->SetFallingHorizontal(false);*/

	//		for (int i = 0; i < (int)_woodBoxes.size(); i++) 
	//		{
	//			_woodBoxes.at(i)->GetParticleModel()->GetForce("Gravity")->SetForce(Vec2(0.0f, 9.81f));
	//			/*_woodBoxes[i]->SetGravity(3.81f);
	//			_woodBoxes[i]->SetFallingVertical(true);
	//			_woodBoxes[i]->SetFallingHorizontal(false);*/
	//		}

	//		for (int i = 0; i < (int)_metalBoxes.size(); i++) 
	//		{
	//			_metalBoxes.at(i)->GetParticleModel()->GetForce("Gravity")->SetForce(Vec2(0.0f, 9.81f));
	//			/*_metalBoxes[i]->SetGravity(3.81f);
	//			_metalBoxes[i]->SetFallingVertical(true);
	//			_metalBoxes[i]->SetFallingHorizontal(false);*/
	//		}
	//	}
	//	else if (direction == 3) { // Right
	//		_player->GetParticleModel()->SetDisplacementX(_player->GetParticleModel()->GetDisplacementX() + 2.5f);
	//		//_player->GetSprite()->setPositionX(_player->GetSprite()->getPositionX() + 0.5f);

	//		for (int i = 0; i < (int)_woodBoxes.size(); i++) 
	//		{
	//			_woodBoxes.at(i)->GetParticleModel()->SetDisplacementX(_woodBoxes.at(i)->GetParticleModel()->GetDisplacementX() + 2.5f);
	//			//_woodBoxes[i]->GetSprite()->setPositionX(_woodBoxes[i]->GetSprite()->getPositionX() + 2.5f);
	//		}

	//		for (int i = 0; i < (int)_metalBoxes.size(); i++) 
	//		{
	//			_metalBoxes.at(i)->GetParticleModel()->SetDisplacementX(_metalBoxes.at(i)->GetParticleModel()->GetDisplacementX() + 2.5f);
	//			//_metalBoxes[i]->GetSprite()->setPositionX(_metalBoxes[i]->GetSprite()->getPositionX() + 2.5f);
	//		}

	//		_player->GetParticleModel()->GetForce("Gravity")->SetForce(Vec2(9.81f, 0.0f));
	//		/*_player->SetGravity(3.81f);
	//		_player->SetFallingVertical(false);
	//		_player->SetFallingHorizontal(true);*/

	//		for (int i = 0; i < (int)_woodBoxes.size(); i++)
	//		{
	//			_woodBoxes.at(i)->GetParticleModel()->GetForce("Gravity")->SetForce(Vec2(9.81f, 0.0f));
	//			/*_woodBoxes[i]->SetGravity(3.81f);
	//			_woodBoxes[i]->SetFallingVertical(false);
	//			_woodBoxes[i]->SetFallingHorizontal(true);*/
	//		}

	//		for (int i = 0; i < (int)_metalBoxes.size(); i++)
	//		{
	//			_metalBoxes.at(i)->GetParticleModel()->GetForce("Gravity")->SetForce(Vec2(9.81f, 0.0f));
	//			/*_metalBoxes[i]->SetGravity(3.81f);
	//			_metalBoxes[i]->SetFallingVertical(false);
	//			_metalBoxes[i]->SetFallingHorizontal(true);*/
	//		}
	//	}
	//}
	//// Handle UP gravity
	//else if (_previousDirection == 2) {
	//	if (direction == 1) { // Left
	//		_player->GetParticleModel()->SetDisplacementX(_player->GetParticleModel()->GetDisplacementX() - 2.5f);
	//		_player->GetParticleModel()->SetDisplacementY(_player->GetParticleModel()->GetDisplacementY() - 2.5f);
	//		//_player->GetSprite()->setPositionY(_player->GetSprite()->getPositionY() - 0.5f);
	//		//_player->GetSprite()->setPositionX(_player->GetSprite()->getPositionX() - 0.5f);

	//		for (int i = 0; i < (int)_woodBoxes.size(); i++) 
	//		{
	//			_woodBoxes.at(i)->GetParticleModel()->SetDisplacementX(_woodBoxes.at(i)->GetParticleModel()->GetDisplacementX() - 2.5f);
	//			_woodBoxes.at(i)->GetParticleModel()->SetDisplacementY(_woodBoxes.at(i)->GetParticleModel()->GetDisplacementY() - 2.5f);
	//			//_woodBoxes[i]->GetSprite()->setPositionY(_woodBoxes[i]->GetSprite()->getPositionY() - 2.5f);
	//			//_woodBoxes[i]->GetSprite()->setPositionX(_woodBoxes[i]->GetSprite()->getPositionX() - 2.5f);
	//		}

	//		for (int i = 0; i < (int)_metalBoxes.size(); i++) 
	//		{
	//			_metalBoxes.at(i)->GetParticleModel()->SetDisplacementX(_metalBoxes.at(i)->GetParticleModel()->GetDisplacementX() - 2.5f);
	//			_metalBoxes.at(i)->GetParticleModel()->SetDisplacementY(_metalBoxes.at(i)->GetParticleModel()->GetDisplacementY() - 2.5f);
	//			//_metalBoxes[i]->GetSprite()->setPositionY(_metalBoxes[i]->GetSprite()->getPositionY() - 2.5f);
	//			//_metalBoxes[i]->GetSprite()->setPositionX(_metalBoxes[i]->GetSprite()->getPositionX() - 2.5f);
	//		}

	//		_player->GetParticleModel()->GetForce("Gravity")->SetForce(Vec2(-9.81f, 0.0f));
	//		/*_player->SetGravity(-3.81f);
	//		_player->SetFallingVertical(false);
	//		_player->SetFallingHorizontal(true);*/

	//		for (int i = 0; i < (int)_woodBoxes.size(); i++)
	//		{
	//			_woodBoxes.at(i)->GetParticleModel()->GetForce("Gravity")->SetForce(Vec2(-9.81f, 0.0f));
	//			/*_woodBoxes[i]->SetGravity(-3.81f);
	//			_woodBoxes[i]->SetFallingVertical(false);
	//			_woodBoxes[i]->SetFallingHorizontal(true);*/
	//		}

	//		for (int i = 0; i < (int)_metalBoxes.size(); i++)
	//		{
	//			_metalBoxes.at(i)->GetParticleModel()->GetForce("Gravity")->SetForce(Vec2(-9.81f, 0.0f));
	//			/*_metalBoxes[i]->SetGravity(-3.81f);
	//			_metalBoxes[i]->SetFallingVertical(false);
	//			_metalBoxes[i]->SetFallingHorizontal(true);*/
	//		}
	//	}
	//	else if (direction == 0) { // Down
	//		_player->GetParticleModel()->SetDisplacementY(_player->GetParticleModel()->GetDisplacementY() - 2.5f);
	//		//_player->GetSprite()->setPositionY(_player->GetSprite()->getPositionY() - 0.5f);

	//		for (int i = 0; i < (int)_woodBoxes.size(); i++) 
	//		{
	//			_woodBoxes.at(i)->GetParticleModel()->SetDisplacementY(_woodBoxes.at(i)->GetParticleModel()->GetDisplacementY() - 2.5f);
	//			//_woodBoxes[i]->GetSprite()->setPositionY(_woodBoxes[i]->GetSprite()->getPositionY() - 2.5f);
	//		}

	//		for (int i = 0; i < (int)_metalBoxes.size(); i++) 
	//		{
	//			_metalBoxes.at(i)->GetParticleModel()->SetDisplacementY(_metalBoxes.at(i)->GetParticleModel()->GetDisplacementY() - 2.5f);
	//			//_metalBoxes[i]->GetSprite()->setPositionY(_metalBoxes[i]->GetSprite()->getPositionY() - 2.5f);
	//		}

	//		_player->GetParticleModel()->GetForce("Gravity")->SetForce(Vec2(0.0f, -9.81f));
	//		/*_player->SetGravity(-3.81f);
	//		_player->SetFallingVertical(true);
	//		_player->SetFallingHorizontal(false);*/

	//		for (int i = 0; i < (int)_woodBoxes.size(); i++)
	//		{
	//			_woodBoxes.at(i)->GetParticleModel()->GetForce("Gravity")->SetForce(Vec2(0.0f, -9.81f));
	//			/*_woodBoxes[i]->SetGravity(-3.81f);
	//			_woodBoxes[i]->SetFallingVertical(true);
	//			_woodBoxes[i]->SetFallingHorizontal(false);*/
	//		}

	//		for (int i = 0; i < (int)_metalBoxes.size(); i++)
	//		{
	//			_metalBoxes.at(i)->GetParticleModel()->GetForce("Gravity")->SetForce(Vec2(0.0f, -9.81f));
	//			/*_metalBoxes[i]->SetGravity(-3.81f);
	//			_metalBoxes[i]->SetFallingVertical(true);
	//			_metalBoxes[i]->SetFallingHorizontal(false);*/
	//		}
	//	}
	//	else if (direction == 3) { // Right
	//		_player->GetParticleModel()->SetDisplacementX(_player->GetParticleModel()->GetDisplacementX() + 2.5f);
	//		_player->GetParticleModel()->SetDisplacementY(_player->GetParticleModel()->GetDisplacementY() - 2.5f);
	//		//_player->GetSprite()->setPositionY(_player->GetSprite()->getPositionY() - 0.5f);
	//		//_player->GetSprite()->setPositionX(_player->GetSprite()->getPositionX() + 1.0f);

	//		for (int i = 0; i < (int)_woodBoxes.size(); i++) 
	//		{
	//			_woodBoxes.at(i)->GetParticleModel()->SetDisplacementX(_woodBoxes.at(i)->GetParticleModel()->GetDisplacementX() + 2.5f);
	//			_woodBoxes.at(i)->GetParticleModel()->SetDisplacementY(_woodBoxes.at(i)->GetParticleModel()->GetDisplacementY() - 2.5f);
	//			//_woodBoxes[i]->GetSprite()->setPositionY(_woodBoxes[i]->GetSprite()->getPositionY() - 2.5f);
	//			//_woodBoxes[i]->GetSprite()->setPositionX(_woodBoxes[i]->GetSprite()->getPositionX() + 2.5f);
	//		}

	//		for (int i = 0; i < (int)_metalBoxes.size(); i++) 
	//		{
	//			_metalBoxes.at(i)->GetParticleModel()->SetDisplacementX(_metalBoxes.at(i)->GetParticleModel()->GetDisplacementX() + 2.5f);
	//			_metalBoxes.at(i)->GetParticleModel()->SetDisplacementY(_metalBoxes.at(i)->GetParticleModel()->GetDisplacementY() - 2.5f);
	//			//_metalBoxes[i]->GetSprite()->setPositionY(_metalBoxes[i]->GetSprite()->getPositionY() - 2.5f);
	//			//_metalBoxes[i]->GetSprite()->setPositionX(_metalBoxes[i]->GetSprite()->getPositionX() + 2.5f);
	//		}

	//		_player->GetParticleModel()->GetForce("Gravity")->SetForce(Vec2(9.81f, 0.0f));
	//		/*_player->SetGravity(3.81f);
	//		_player->SetFallingVertical(false);
	//		_player->SetFallingHorizontal(true);*/

	//		for (int i = 0; i < (int)_woodBoxes.size(); i++)
	//		{
	//			_woodBoxes.at(i)->GetParticleModel()->GetForce("Gravity")->SetForce(Vec2(9.81f, 0.0f));
	//			/*_woodBoxes[i]->SetGravity(3.81f);
	//			_woodBoxes[i]->SetFallingVertical(false);
	//			_woodBoxes[i]->SetFallingHorizontal(true);*/
	//		}

	//		for (int i = 0; i < (int)_metalBoxes.size(); i++)
	//		{
	//			_metalBoxes.at(i)->GetParticleModel()->GetForce("Gravity")->SetForce(Vec2(9.81f, 0.0f));
	//			/*_metalBoxes[i]->SetGravity(3.81f);
	//			_metalBoxes[i]->SetFallingVertical(false);
	//			_metalBoxes[i]->SetFallingHorizontal(true);*/
	//		}
	//	}
	//}
	//// Handle RIGHT gravity
	//else if (_previousDirection == 3) {
	//	if (direction == 0) { // Down
	//		_player->GetParticleModel()->SetDisplacementX(_player->GetParticleModel()->GetDisplacementX() - 2.5f);
	//		_player->GetParticleModel()->SetDisplacementY(_player->GetParticleModel()->GetDisplacementY() - 2.5f);
	//		//_player->GetSprite()->setPositionY(_player->GetSprite()->getPositionY() - 0.5f);
	//		//_player->GetSprite()->setPositionX(_player->GetSprite()->getPositionX() - 0.5f);

	//		for (int i = 0; i < (int)_woodBoxes.size(); i++) 
	//		{
	//			_woodBoxes.at(i)->GetParticleModel()->SetDisplacementX(_woodBoxes.at(i)->GetParticleModel()->GetDisplacementX() - 2.5f);
	//			_woodBoxes.at(i)->GetParticleModel()->SetDisplacementY(_woodBoxes.at(i)->GetParticleModel()->GetDisplacementY() - 2.5f);
	//			//_woodBoxes[i]->GetSprite()->setPositionY(_woodBoxes[i]->GetSprite()->getPositionY() - 2.5f);
	//			//_woodBoxes[i]->GetSprite()->setPositionX(_woodBoxes[i]->GetSprite()->getPositionX() - 2.5f);
	//		}

	//		for (int i = 0; i < (int)_metalBoxes.size(); i++) 
	//		{
	//			_metalBoxes.at(i)->GetParticleModel()->SetDisplacementX(_metalBoxes.at(i)->GetParticleModel()->GetDisplacementX() - 2.5f);
	//			_metalBoxes.at(i)->GetParticleModel()->SetDisplacementY(_metalBoxes.at(i)->GetParticleModel()->GetDisplacementY() - 2.5f);
	//			//_metalBoxes[i]->GetSprite()->setPositionY(_metalBoxes[i]->GetSprite()->getPositionY() - 2.5f);
	//			//_metalBoxes[i]->GetSprite()->setPositionX(_metalBoxes[i]->GetSprite()->getPositionX() - 2.5f);
	//		}

	//		_player->GetParticleModel()->GetForce("Gravity")->SetForce(Vec2(0.0f, -9.81f));
	//		/*_player->SetGravity(-3.81f);
	//		_player->SetFallingVertical(true);
	//		_player->SetFallingHorizontal(false);*/

	//		for (int i = 0; i < (int)_woodBoxes.size(); i++)
	//		{
	//			_woodBoxes.at(i)->GetParticleModel()->GetForce("Gravity")->SetForce(Vec2(0.0f, -9.81f));
	//			/*_woodBoxes[i]->SetGravity(-3.81f);
	//			_woodBoxes[i]->SetFallingVertical(true);
	//			_woodBoxes[i]->SetFallingHorizontal(false);*/
	//		}

	//		for (int i = 0; i < (int)_metalBoxes.size(); i++)
	//		{
	//			_metalBoxes.at(i)->GetParticleModel()->GetForce("Gravity")->SetForce(Vec2(0.0f, -9.81f));
	//			/*_metalBoxes[i]->SetGravity(-3.81f);
	//			_metalBoxes[i]->SetFallingVertical(true);
	//			_metalBoxes[i]->SetFallingHorizontal(false);*/
	//		}
	//	}
	//	else if (direction == 2) { // Up
	//		_player->GetParticleModel()->SetDisplacementX(_player->GetParticleModel()->GetDisplacementX() - 2.5f);
	//		_player->GetParticleModel()->SetDisplacementY(_player->GetParticleModel()->GetDisplacementY() + 2.5f);
	//		//_player->GetSprite()->setPositionY(_player->GetSprite()->getPositionY() + 0.5f);
	//		//_player->GetSprite()->setPositionX(_player->GetSprite()->getPositionX() - 0.5f);

	//		for (int i = 0; i < (int)_woodBoxes.size(); i++) 
	//		{
	//			_woodBoxes.at(i)->GetParticleModel()->SetDisplacementX(_woodBoxes.at(i)->GetParticleModel()->GetDisplacementX() - 2.5f);
	//			_woodBoxes.at(i)->GetParticleModel()->SetDisplacementY(_woodBoxes.at(i)->GetParticleModel()->GetDisplacementY() + 2.5f);
	//			//_woodBoxes[i]->GetSprite()->setPositionY(_woodBoxes[i]->GetSprite()->getPositionY() + 2.5f);
	//			//_woodBoxes[i]->GetSprite()->setPositionX(_woodBoxes[i]->GetSprite()->getPositionX() - 2.5f);
	//		}

	//		for (int i = 0; i < (int)_metalBoxes.size(); i++) 
	//		{
	//			_metalBoxes.at(i)->GetParticleModel()->SetDisplacementX(_metalBoxes.at(i)->GetParticleModel()->GetDisplacementX() - 2.5f);
	//			_metalBoxes.at(i)->GetParticleModel()->SetDisplacementY(_metalBoxes.at(i)->GetParticleModel()->GetDisplacementY() + 2.5f);
	//			//_metalBoxes[i]->GetSprite()->setPositionY(_metalBoxes[i]->GetSprite()->getPositionY() + 2.5f);
	//			//_metalBoxes[i]->GetSprite()->setPositionX(_metalBoxes[i]->GetSprite()->getPositionX() - 2.5f);
	//		}

	//		_player->GetParticleModel()->GetForce("Gravity")->SetForce(Vec2(0.0f, 9.81f));
	//		/*_player->SetGravity(3.81f);
	//		_player->SetFallingVertical(true);
	//		_player->SetFallingHorizontal(false);*/

	//		for (int i = 0; i < (int)_woodBoxes.size(); i++)
	//		{
	//			_woodBoxes.at(i)->GetParticleModel()->GetForce("Gravity")->SetForce(Vec2(0.0f, 9.81f));
	//			/*_woodBoxes[i]->SetGravity(3.81f);
	//			_woodBoxes[i]->SetFallingVertical(true);
	//			_woodBoxes[i]->SetFallingHorizontal(false);*/
	//		}

	//		for (int i = 0; i < (int)_metalBoxes.size(); i++)
	//		{
	//			_metalBoxes.at(i)->GetParticleModel()->GetForce("Gravity")->SetForce(Vec2(0.0f, 9.81f));
	//			/*_metalBoxes[i]->SetGravity(3.81f);
	//			_metalBoxes[i]->SetFallingVertical(true);
	//			_metalBoxes[i]->SetFallingHorizontal(false);*/
	//		}
	//	}
	//	else if (direction == 1) { // Left
	//		_player->GetParticleModel()->SetDisplacementX(_player->GetParticleModel()->GetDisplacementX() - 2.5f);
	//		_player->GetSprite()->setPositionX(_player->GetSprite()->getPositionX() - 2.5f);

	//		for (int i = 0; i < (int)_woodBoxes.size(); i++) 
	//		{
	//			_woodBoxes.at(i)->GetParticleModel()->SetDisplacementX(_woodBoxes.at(i)->GetParticleModel()->GetDisplacementX() - 2.5f);
	//			//_woodBoxes[i]->GetSprite()->setPositionX(_woodBoxes[i]->GetSprite()->getPositionX() - 2.5f);
	//		}

	//		for (int i = 0; i < (int)_metalBoxes.size(); i++) 
	//		{
	//			_metalBoxes.at(i)->GetParticleModel()->SetDisplacementX(_metalBoxes.at(i)->GetParticleModel()->GetDisplacementX() - 2.5f);
	//			//_metalBoxes[i]->GetSprite()->setPositionX(_metalBoxes[i]->GetSprite()->getPositionX() - 2.5f);
	//		}

	//		_player->GetParticleModel()->GetForce("Gravity")->SetForce(Vec2(-9.81f, 0.0f));
	//		/*_player->SetGravity(-3.81f);
	//		_player->SetFallingVertical(false);
	//		_player->SetFallingHorizontal(true);*/

	//		for (int i = 0; i < (int)_woodBoxes.size(); i++)
	//		{
	//			_woodBoxes.at(i)->GetParticleModel()->GetForce("Gravity")->SetForce(Vec2(-9.81f, 0.0f));
	//			/*_woodBoxes[i]->SetGravity(-3.81f);
	//			_woodBoxes[i]->SetFallingVertical(false);
	//			_woodBoxes[i]->SetFallingHorizontal(true);*/
	//		}

	//		for (int i = 0; i < (int)_metalBoxes.size(); i++)
	//		{
	//			_metalBoxes.at(i)->GetParticleModel()->GetForce("Gravity")->SetForce(Vec2(-9.81f, 0.0f));
	//			/*_metalBoxes[i]->SetGravity(-3.81f);
	//			_metalBoxes[i]->SetFallingVertical(false);
	//			_metalBoxes[i]->SetFallingHorizontal(true);*/
	//		}
	//	}
	//}

	//_player->FlipPlayer();
}

void SceneManager::IsPlayerInBounds()
{
	auto winSize = Director::getInstance()->getVisibleSize();
	// Checks if the player has gone off the bottom of the screen
	if (_player->GetParticleModel()->GetDisplacementY() < (0.0f - _player->GetSprite()->getContentSize().height)
		|| _player->GetParticleModel()->GetDisplacementX() < (0.0f - _player->GetSprite()->getContentSize().width))
	{
		GameManager::sharedGameManager()->setIsGameLive(false);
		//_player->GetSprite()->setPosition(_woodCrateSpawn[0]->getPosition());
		
	}
	else if (_player->GetParticleModel()->GetDisplacementY() > (winSize.height + _player->GetSprite()->getContentSize().height)
		|| _player->GetParticleModel()->GetDisplacementX() > (winSize.width + _player->GetSprite()->getContentSize().width))
	{
		//_player->GetSprite()->setPosition(_woodCrateSpawn[0]->getPosition());
		GameManager::sharedGameManager()->setIsGameLive(false);
	}
}

void SceneManager::IsCrateInBounds()
{
	auto winSize = Director::getInstance()->getVisibleSize();

	// Moves Wood crates to spawn area if off screen
	for (int i = 0; i < (int)_woodBoxes.size(); i++)
	{
		if (_woodBoxes[i]->GetSprite()->getPosition().y < (0.0f -_woodBoxes[i]->GetSprite()->getContentSize().height)
			|| _woodBoxes[i]->GetSprite()->getPosition().x < (0.0f - _woodBoxes[i]->GetSprite()->getContentSize().width))
		{
			_woodBoxes[i]->GetSprite()->setPosition(_woodCrateSpawn[i]->getPosition());
		}
		else if (_woodBoxes[i]->GetSprite()->getPosition().y > (winSize.height + _woodBoxes[i]->GetSprite()->getContentSize().height)
			|| _woodBoxes[i]->GetSprite()->getPosition().x >(winSize.width + _woodBoxes[i]->GetSprite()->getContentSize().width))
		{
			_woodBoxes[i]->GetSprite()->setPosition(_woodCrateSpawn[i]->getPosition());
		}
	}

	// Moves Metal crates to spawn area if off screen
	for (int i = 0; i < (int)_metalBoxes.size(); i++)
	{
		if (_metalBoxes[i]->GetSprite()->getPosition().y < (0.0f - _metalBoxes[i]->GetSprite()->getContentSize().height)
			|| _metalBoxes[i]->GetSprite()->getPosition().x < (0.0f - _metalBoxes[i]->GetSprite()->getContentSize().width))
		{
			_metalBoxes[i]->GetSprite()->setPosition(_metalCrateSpawn[i]->getPosition());
		}
		else if (_metalBoxes[i]->GetSprite()->getPosition().y >(winSize.height + _metalBoxes[i]->GetSprite()->getContentSize().height)
			|| _metalBoxes[i]->GetSprite()->getPosition().x >(winSize.width + _metalBoxes[i]->GetSprite()->getContentSize().width))
		{
			_metalBoxes[i]->GetSprite()->setPosition(_metalCrateSpawn[i]->getPosition());
		}
	}
}

SceneManager::~SceneManager()
{
	// I have no idea if this is ever called
	delete _player;
	delete _timeLabel;
	delete _background1;
	delete _background2;
	delete _background3;
	delete _background4;
	delete _blackTransparency;
	delete auEngine;

	for (int i = 0; i < (int)_platforms.size(); i++) {
		delete _platforms[i];
	}

	_platforms.clear();

	for (int i = 0; i < (int)_walls.size(); i++) {
		delete _walls[i];
	}

	_walls.clear();

	for (int i = 0; i < (int)_gravSwitchesDown.size(); i++) {
		delete _switches.at(i)->GetSprite();
	}

	for (int i = 0; i < (int)_gravSwitchesLeft.size(); i++) {
		delete _switches.at(i)->GetSprite();
	}

	for (int i = 0; i < (int)_gravSwitchesUp.size(); i++) {
		delete _switches.at(i)->GetSprite();
	}

	for (int i = 0; i < (int)_gravSwitchesRight.size(); i++) {
		delete _switches.at(i)->GetSprite();
	}

	_gravSwitchesDown.clear();
	_gravSwitchesLeft.clear();
	_gravSwitchesUp.clear();
	_gravSwitchesRight.clear();

	for (int i = 0; i < (int)_timerSwitchesDown.size(); i++) {
		delete _switches.at(i)->GetSprite();
	}

	for (int i = 0; i < (int)_timerSwitchesLeft.size(); i++) {
		delete _switches.at(i)->GetSprite();
	}

	for (int i = 0; i < (int)_timerSwitchesUp.size(); i++) {
		delete _switches.at(i)->GetSprite();
	}

	for (int i = 0; i < (int)_timerSwitchesRight.size(); i++) {
		delete _switches.at(i)->GetSprite();
	}

	_timerSwitchesDown.clear();
	_timerSwitchesLeft.clear();
	_timerSwitchesUp.clear();
	_timerSwitchesRight.clear();
}
