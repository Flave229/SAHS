#ifndef __Floor_Button_H__
#define __Floor_Button_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "cocostudio/CCComExtensionData.h"

#include "Player.h"
#include "Box.h"

USING_NS_CC;

class FloorButton : public GameObject
{
private:
	bool _activated;
	float _timer;

	// Button Orientation: 0 = Down; 1 = Left; 2 = Up; 3 = Right;
	int _orientation;
public:
	FloorButton(int direction);
	FloorButton(ParticleModel* particle, int direction);
	~FloorButton();

	// Button Orientation
	void SetOrientation(int orientation) { _orientation = orientation; };
	int GetOrientation() { return _orientation; };

	// Collisions
	bool CheckCollision(GameObject* collider, float cR, bool moveThis = true, bool moveCollider = true);

	// Sprite
	void SetSprite(Sprite* newSprite);

	// Active
	void SetActive(bool activeState);
	bool GetActive() { return _activated; };

	// Timer
	float GetTimer() { return _timer; };
};

#endif
