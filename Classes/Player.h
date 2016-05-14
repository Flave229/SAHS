#ifndef __Player_H__
#define __Player_H__

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "GameManager.h"
#include "GameObject.h"
USING_NS_CC;

class Player : public GameObject
{
private:
	// Sprite
	int _frame;
	bool _running;
	float _spriteTimer;
	float _spriteTimerDefault;

	// Move to
	Vec2	_targetPos;
	float	_speed;

	void MoveToTarget(float deltaTime);
public:
	Player();
	Player(ParticleModel* particle);
	~Player();
	
	void Update(float delta);

	// Movement
	void SetTarget(Vec2 target);
};

#endif