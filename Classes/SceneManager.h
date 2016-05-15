#ifndef __SCENE1_SCENE_H__
#define __SCENE1_SCENE_H__

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include <iomanip>
#include "Define.h"
#include "Player.h"
#include "GameManager.h"
#include "AudioEngine.h"
#include "GameOverScene.h"
#include "GameWinScene.h"
#include "Box.h"
#include "Switch.h"
#include "FloorButton.h"
#include "TouchManager.h"
#include "MovablePlatform.h"
#include "Door.h"
#include "MovableWall.h"
#include "SwitchTimer.h"
#include "MenuScene.h"

USING_NS_CC;

class SceneManager : public Scene
{
private:
	int _level;
	int _score;
	Node* rootNode;
	// UI
	cocos2d::ui::Text*		_timeLabel;
	cocos2d::ui::Button*	_startGame;
	cocos2d::ui::Button*	_retryButton;
	cocos2d::ui::Button*	_backButton;

	// AUDIO
	AudioEngine* auEngine;

	// Sprites that need to be passed to classes
	std::vector<cocos2d::Sprite*>		_platformSprites;
	std::vector<cocos2d::Sprite*>		_wallSprites;

	Sprite*								_playerSprite;
	std::vector<cocos2d::Sprite*>		_woodenSprites;
	std::vector<cocos2d::Sprite*>		_metalSprites;

	std::vector<cocos2d::Sprite*>		_downButtons;
	std::vector<cocos2d::Sprite*>		_leftButtons;
	std::vector<cocos2d::Sprite*>		_upButtons;
	std::vector<cocos2d::Sprite*>		_rightButtons;

	std::vector<cocos2d::Sprite*>		_movingPlatformVertSprites;
	std::vector<cocos2d::Sprite*>		_movingPlatformHorizSprites;
	std::vector<cocos2d::Sprite*>		_movingWallVertSprites;
	std::vector<cocos2d::Sprite*>		_movingWallHorizSprites;

	std::vector<cocos2d::Sprite*>		_doorSprites;
	std::vector<cocos2d::Sprite*>		_solidDoorSprites;
	std::vector<cocos2d::Sprite*>		_hatchSprites;

	std::vector<cocos2d::ui::CheckBox*> _gravSwitchesDown;
	std::vector<cocos2d::ui::CheckBox*> _gravSwitchesLeft;
	std::vector<cocos2d::ui::CheckBox*> _gravSwitchesUp;
	std::vector<cocos2d::ui::CheckBox*> _gravSwitchesRight;

	std::vector<cocos2d::ui::CheckBox*> _timerSwitchesDown;
	std::vector<cocos2d::ui::CheckBox*> _timerSwitchesLeft;
	std::vector<cocos2d::ui::CheckBox*> _timerSwitchesUp;
	std::vector<cocos2d::ui::CheckBox*> _timerSwitchesRight;
	
	Sprite*								_playerSpawn;

	// BACKGROUND
	Sprite*	_background1;
	Sprite*	_background2;
	Sprite*	_background3;
	Sprite*	_background4;
	Sprite* _blackTransparency;

	// GRAVITY HIGHLIGHTS
	Sprite* _topHighlight;
	float	_topHighlightTime;
	bool	_topHighlightEnabled;

	Sprite* _rightHighlight;
	float	_rightHighlightTime;
	bool	_rightHighlightEnabled;

	Sprite* _bottomHighlight;
	float	_bottomHighlightTime;
	bool	_bottomHighlightEnabled;

	Sprite* _leftHighlight;
	float	_leftHighlightTime;
	bool	_leftHighlightEnabled;

	// SCENE ELEMENTS
	std::vector<cocos2d::ui::CheckBox*> _exit;

	std::vector<bool>					_flipped;
	std::vector<cocos2d::Sprite*>		_railStart;
	std::vector<cocos2d::Sprite*>		_railEnd;
	std::vector<cocos2d::Sprite*>		_woodCrateSpawn;
	std::vector<cocos2d::Sprite*>		_metalCrateSpawn;

	// SCENE CLASSES
	Player* _player;
	std::vector<GameObject*>		_platforms;
	std::vector<GameObject*>		_walls;
	std::vector<Box*>				_woodBoxes;
	std::vector<Box*>				_metalBoxes;
	std::vector<Switch*>			_switches;
	std::vector<SwitchTimer*>		_tSwitches;
	std::vector<FloorButton*>		_buttons;
	std::vector<Door*>				_doors;
	std::vector<Door*>				_hatches;
	std::vector<MovablePlatform*>	_movingPlatformsVert;
	std::vector<MovablePlatform*>	_movingPlatformsHoriz;
	std::vector<MovableWall*>		_movingWallsVert;
	std::vector<MovableWall*>		_movingWallsHoriz;

	// GRAVITY
	// Gravity Orientation: 0 = Down; 1 = Left; 2 = Up; 3 = Right;
	int		_previousDirection;
	int		_gravityOrientation;
	bool	_gravityHighlight;
	float	_flipGravityCooldown = 1.0f;	// One second cooldown
	void FlipGravity(int direction);

	// TOUCHES
	TouchManager*	touchMGR;
	bool			_inTouch;
	cocos2d::Vec2	_initialTouchPos;
	bool			isObjectTouched;

	bool exists;


public:
	// there's no 'id' in cpp, so we recommend returning the class instance pointer
	static SceneManager* createScene(int level);

	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	SceneManager(int level);
	virtual bool init();
	~SceneManager();

	void SetupTimer(Node* root);
	void SetupAudio(Node* root);
	void SetupButtons(Node* root);
	void SetupSprites(Node* root);
	void SetupBackground(Node* root);
	void SetupHighlights(Node* root);
	void SetupClasses(Node* root);

	void CheckCollisions();

	void update(float delta);
	void ScheduleScore(float delta);
	void CheckNear(float delta);
	void CheckNearTimer(float delta);
	void CheckNearDoor(float delta);
	void IsPlayerInBounds();
	void IsCrateInBounds();
	void RevertGravity();

	bool CheckNode();

	// implement the "static create()" method manually
	//CREATE_FUNC(Scene1);

	// Touch input
	void SetupTouches();

	virtual bool onTouchBegan(cocos2d::Touch*, cocos2d::Event*);
	virtual void onTouchEnded(cocos2d::Touch*, cocos2d::Event*);
	virtual void onTouchMoved(cocos2d::Touch*, cocos2d::Event*);
	virtual void onTouchCancelled(cocos2d::Touch*, cocos2d::Event*);

	void SwitchPressed(Ref *sender, cocos2d::ui::Widget::TouchEventType type);
	void DoorPressed(Ref *sender, cocos2d::ui::Widget::TouchEventType type);
	void BackButtonPressed(Ref* sender, cocos2d::ui::Widget::TouchEventType type);
	void RetryButtonPressed(Ref* sender, cocos2d::ui::Widget::TouchEventType type);
	void StartButtonPressed(Ref* sender, cocos2d::ui::Widget::TouchEventType type);
	void SwitchTimerPressed(Ref *sender, cocos2d::ui::Widget::TouchEventType type);

};

#endif // __SCENE1_SCENE_H__
