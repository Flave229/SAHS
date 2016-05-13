#include "MovingPlatform.h"

USING_NS_CC;

using namespace cocostudio::timeline;

MovingPlatform::MovingPlatform(ParticleModel* particle, Player* ref, std::vector<Box*> wood, std::vector<Box*> metal) : GameObject(particle)
{
	_playerRef = ref;
	_woodenRef = wood;
	_metalRef = metal;

	CollidingWithPlatform = false;
	CollidingWithWall = false;
}

bool MovingPlatform::init()
{
	if (!Node::init())
	{
		return false;
	}

	this->scheduleUpdate();

	return true;
}

MovingPlatform::~MovingPlatform()
{

}

void MovingPlatform::setZoneSprite()
{
	_touchZone = Sprite::create("Art/Level_Main_Assets/Platforms/Platform_1.png");
	this->addChild(_touchZone);
	_touchZone->setPosition(GetSprite()->getPosition());
	_touchZone->setScaleX(GetSprite()->getScaleX() * 1.2);
	_touchZone->setScaleY(GetSprite()->getScaleY() * 3.5);
	_touchZone->setOpacity(75);

	OTouchZoneYScale = _touchZone->getScaleY();
}

void MovingPlatform::Update(float delta)
{
	GameObject::Update(delta);
}

void MovingPlatform::SetSprite(cocos2d::Sprite* sprite)
{ 
	OScale = sprite->getScale();

	GameObject::SetSprite(sprite);
}

void MovingPlatform::Selected()
{
	NScale = (OScale*1.2);
	NTouchZoneYScale = (OTouchZoneYScale * 6);

	GetSprite()->setScale(NScale);
	_touchZone->setScaleY(NTouchZoneYScale);
}

void MovingPlatform::UnSelected()
{
	GetSprite()->setScale(OScale);
	_touchZone->setScaleY(OTouchZoneYScale);
}

void MovingPlatform::MovePlatformHorizontal(cocos2d::Vec2 T)
{
	_touchZone->setPositionX(T.x);
	GetParticleModel()->SetDisplacementX(T.x);

	float scaledWidth = GetSprite()->getContentSize().width * GetSprite()->getScaleX();
	float scaledHeight = GetSprite()->getContentSize().height * GetSprite()->getScaleY();
	float scaledPlayerWidth = _playerRef->GetSprite()->getContentSize().width * _playerRef->GetSprite()->getScaleX();
	float scaledPlayerHeight = _playerRef->GetSprite()->getContentSize().height * _playerRef->GetSprite()->getScaleY();

	/*if (_playerRef->GetOrientationVertical()) {
		if (_playerRef->GetSprite()->getPositionX() - (scaledPlayerWidth / 2) < GetSprite()->getPositionX() + (scaledWidth / 2)
			&& _playerRef->GetSprite()->getPositionX() + (scaledPlayerWidth / 2) > GetSprite()->getPositionX() - (scaledWidth / 2)
			&& _playerRef->GetSprite()->getPositionY() - (scaledPlayerHeight / 2) < GetSprite()->getPositionY() + (scaledHeight / 2)
			&& _playerRef->GetSprite()->getPositionY() + (scaledPlayerHeight / 2) > GetSprite()->getPositionY() - (scaledHeight / 2))
		{
			_playerRef->GetSprite()->setPositionX(T.x);
			_playerRef->SetTarget(T);
		}
	}*/

	for (int i = 0; i < (int)_woodenRef.size(); i++) {
		float scaledWidth = GetSprite()->getContentSize().width * GetSprite()->getScaleX();
		float scaledHeight = GetSprite()->getContentSize().height * GetSprite()->getScaleY();
		float scaledBoxWidth = _woodenRef.at(i)->GetSprite()->getContentSize().width * _woodenRef.at(i)->GetSprite()->getScaleX();
		float scaledBoxHeight = _woodenRef.at(i)->GetSprite()->getContentSize().height *_woodenRef.at(i)->GetSprite()->getScaleY();

		if (_woodenRef.at(i)->GetSprite()->getPositionX() - (scaledBoxWidth / 2) < GetSprite()->getPositionX() + (scaledWidth / 2)
			&& _woodenRef.at(i)->GetSprite()->getPositionX() + (scaledBoxWidth / 2) > GetSprite()->getPositionX() - (scaledWidth / 2)
			&& _woodenRef.at(i)->GetSprite()->getPositionY() - (scaledBoxHeight / 2) < GetSprite()->getPositionY() + (scaledHeight / 2)
			&& _woodenRef.at(i)->GetSprite()->getPositionY() + (scaledBoxHeight / 2) > GetSprite()->getPositionY() - (scaledHeight / 2))
		{
			_woodenRef.at(i)->GetSprite()->setPositionX(T.x);
		}
	}

	for (int i = 0; i < (int)_metalRef.size(); i++) {
		float scaledWidth = GetSprite()->getContentSize().width * GetSprite()->getScaleX();
		float scaledHeight = GetSprite()->getContentSize().height * GetSprite()->getScaleY();
		float scaledBoxWidth = _metalRef.at(i)->GetSprite()->getContentSize().width * _metalRef.at(i)->GetSprite()->getScaleX();
		float scaledBoxHeight = _metalRef.at(i)->GetSprite()->getContentSize().height *_metalRef.at(i)->GetSprite()->getScaleY();

		if (_metalRef.at(i)->GetSprite()->getPositionX() - (scaledBoxWidth / 2) < GetSprite()->getPositionX() + (scaledWidth / 2)
			&& _metalRef.at(i)->GetSprite()->getPositionX() + (scaledBoxWidth / 2) > GetSprite()->getPositionX() - (scaledWidth / 2)
			&& _metalRef.at(i)->GetSprite()->getPositionY() - (scaledBoxHeight / 2) < GetSprite()->getPositionY() + (scaledHeight / 2)
			&& _metalRef.at(i)->GetSprite()->getPositionY() + (scaledBoxHeight / 2) > GetSprite()->getPositionY() - (scaledHeight / 2))
		{
			_metalRef.at(i)->GetSprite()->setPositionX(T.x);
		}
	}
}

void MovingPlatform::MovePlatformVertical(cocos2d::Vec2 T)
{
	if (CollidingWithPlatform)
	{
		_touchZone->setPositionY(T.y);
		GetSprite()->setPositionY(T.y);
	}
	else
	{
		_touchZone->setPositionY(GetSprite()->getPositionY());
		GetSprite()->setPositionY(T.y);
	}
}

//Touch Functions
void MovingPlatform::onTouchBegan(Touch* touch, Event* event)
{
	GameManager::sharedGameManager()->setIsObjectTouched(true);
	Vec2 T = touch->getLocation();
	Rect platform = _touchZone->getBoundingBox();

	if (platform.containsPoint(T))
	{
		Selected();
	}
}

void MovingPlatform::onTouchEnded(Touch* touch, Event* event)
{
	GameManager::sharedGameManager()->setIsObjectTouched(false);
	UnSelected();
}

void MovingPlatform::onTouchMoved(Touch* touch, Event* event)
{
	Vec2 T = touch->getLocation();
	Rect platform = _touchZone->getBoundingBox();

	if (!CollidingWithPlatform || !CollidingWithWall)
	{
		if (platform.containsPoint(T) && platformType == 1)
		{
			MovePlatformHorizontal(T);
		}
		else if (platform.containsPoint(T) && platformType == 2)
		{
			MovePlatformVertical(T);
		}
	}
}

void MovingPlatform::onTouchCancelled(Touch* touch, Event* event)
{
	cocos2d::log("touch cancelled");
}