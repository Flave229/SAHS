#ifndef __Platform_SCENE_H__
#define __Platform_SCENE_H__

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "Define.h"
#include "GameManager.h"
#include "Player.h"
#include "Box.h"
#include "GameObject.h"
USING_NS_CC;

class MovingPlatform : public GameObject
{
private:
	Player* _playerRef;
	std::vector<Box*> _woodenRef;
	std::vector<Box*> _metalRef;

	Sprite* _touchZone;
	float OScale;
	float NScale;
	float OTouchZoneYScale;
	float NTouchZoneYScale;

	int platformType = 0; // Gives platform types a number: 1 = Horizontal, 2 = vertical, 0 = no platform
	bool CollidingWithWall;
	bool CollidingWithPlatform;
public:
	MovingPlatform(ParticleModel* particle, Player* ref, std::vector<Box*> wood, std::vector<Box*> metal);
	~MovingPlatform();

	virtual bool init() override;

	void Update(float delta);

	void SetSprite(Sprite* newSprite);
	void setZoneSprite();
	Sprite* getTouchZone() { return _touchZone; }

	void PlatformType(int type) { platformType = type; }

	void Selected();
	void UnSelected();

	void MovePlatformHorizontal(cocos2d::Vec2 T);
	void MovePlatformVertical(cocos2d::Vec2 T);

	// Touch input
	virtual void onTouchBegan(cocos2d::Touch*, cocos2d::Event*);
	virtual void onTouchEnded(cocos2d::Touch*, cocos2d::Event*);
	virtual void onTouchMoved(cocos2d::Touch*, cocos2d::Event*);
	virtual void onTouchCancelled(cocos2d::Touch*, cocos2d::Event*);
};

#endif // __Platform_SCENE_H__