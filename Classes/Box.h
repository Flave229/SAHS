#ifndef __Box_H__
#define __Box_H__

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "cocostudio/CCComExtensionData.h"
#include "ui/CocosGUI.h"
#include "GameManager.h"
#include "ScalingObject.h"
#include "TouchManager.h"
#include "GameObject.h"

USING_NS_CC;


class Box : public GameObject
{
private:
	// Gravity variables
	float	_defaultTouchTimer = 0.5f;
	float	_touchTimer;

	// Scaler
	float scaler1;
	float scaler2;
	float scaler3;
	float scaler;
	float totalDiff;
	int unselect;
	bool isSelected;
public:
	Box(float startingWeight);
	Box(ParticleModel* particle, float startingWeight);
	~Box();

	virtual bool init() override;
	void Update(float delta);

	// Overload SetSprite from GameObject
	void SetSprite(cocos2d::Sprite* sprite);

	// Physics
	void Flip();

	// Scaling
	void SetTotalDiff(float totalDiffNew);
	void Selected();
	void Deselected();
	void SetStartingScale();
	void Collision(cocos2d::Touch* touch);
	void Scaling();
	bool getIsSelected() { return isSelected; }
	//void UpdateBoxScale(){ _box->setScale(scaler); }
};

#endif