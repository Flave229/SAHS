#ifndef __SCENE1_SCENE_H__
#define __SCENE1_SCENE_H__

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "Define.h"
#include "Player.h"
#include "GameManager.h"
#include "AudioEngine.h"

class Scene1 : public cocos2d::Layer
{
public:
	// there's no 'id' in cpp, so we recommend returning the class instance pointer
	static cocos2d::Scene* createScene();

	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init();

	void CheckCollisions();

	void update(float delta);
	void ScheduleScore(float delta);

	// implement the "static create()" method manually
	CREATE_FUNC(Scene1);

	// Touch input
	virtual bool onTouchBegan(cocos2d::Touch*, cocos2d::Event*);
	virtual void onTouchEnded(cocos2d::Touch*, cocos2d::Event*);
	virtual void onTouchMoved(cocos2d::Touch*, cocos2d::Event*);
	virtual void onTouchCancelled(cocos2d::Touch*, cocos2d::Event*);
private:
	int score;
	Player* player;
	cocos2d::ui::Text*	 TimeLabel;

	Player* player;
	AudioEngine* auEngine;
	std::vector<cocos2d::Sprite*> platforms;

	// Gravity
	const float gravity = -9.81f;

	// Touches
	bool inTouch;
	cocos2d::Vec2 initialTouchPos;
};

#endif // __SCENE1_SCENE_H__
