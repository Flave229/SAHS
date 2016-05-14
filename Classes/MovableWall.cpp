#include "MovableWall.h"

USING_NS_CC;

MovableWall::MovableWall(Player* ref, std::vector<Box*> wood, std::vector<Box*> metal) : GameObject(new ParticleModel())
{
	_playerRef = ref;
	_woodenRef = wood;
	_metalRef = metal;

	_touchZone = nullptr;
	_oScale = 0.0f;
	_nScale = 0.0f;
	_oTouchZoneXScale = 0.0f;
	_nTouchZoneXScale = 0.0f;

	_platformType = 0;
	_collidingWithPlatform = false;
	_collidingWithWall = false;
}

MovableWall::MovableWall(ParticleModel* particle, Player* ref, std::vector<Box*> wood, std::vector<Box*> metal) : GameObject(particle)
{
	_playerRef = ref;
	_woodenRef = wood;
	_metalRef = metal;

	_touchZone = nullptr;
	_oScale = 0.0f;
	_nScale = 0.0f;
	_oTouchZoneXScale = 0.0f;
	_nTouchZoneXScale = 0.0f;

	_platformType = 0;
	_collidingWithPlatform = false;
	_collidingWithWall = false;
}

MovableWall::~MovableWall()
{
}

void MovableWall::SetZoneSprite()
{
	_touchZone = Sprite::create("Art/Level_Main_Assets/Platforms/Wall_1.png");
	this->addChild(_touchZone);
	_touchZone->setPosition(GetSprite()->getPosition());
	_touchZone->setScaleX(GetSprite()->getScaleX() * 3.5);
	_touchZone->setScaleY(GetSprite()->getScaleY() * 1.2);
	_touchZone->setOpacity(75);

	_oTouchZoneXScale = _touchZone->getScaleX();
}

void MovableWall::Update(float delta)
{
	// TO DO
}

void MovableWall::SetSprite(cocos2d::Sprite* sprite)
{
	_oScale = sprite->getScale();

	GameObject::SetSprite(sprite);
}

void MovableWall::Selected()
{
	_nScale = (_oScale*1.2);
	_nTouchZoneXScale = (_oTouchZoneXScale * 6);

	GetSprite()->setScale(_nScale);
	_touchZone->setScaleX(_nTouchZoneXScale);
}

void MovableWall::UnSelected()
{
	GetSprite()->setScale(_oScale);
	_touchZone->setScaleX(_oTouchZoneXScale);
}

void MovableWall::MovePlatformHorizontal(cocos2d::Vec2 T)
{
	if (_collidingWithPlatform)
	{
		_touchZone->setPositionX(T.x);
		GetSprite()->setPositionX(T.x);
	}
	else
	{
		_touchZone->setPositionX(GetSprite()->getPositionX());
		GetSprite()->setPositionX(T.x);
	}
	
}

void MovableWall::MovePlatformVertical(cocos2d::Vec2 T)
{
	_touchZone->setPositionY(T.y);
	GetSprite()->setPositionY(T.y);

	float scaledWidth = GetSprite()->getContentSize().width * GetSprite()->getScaleX();
	float scaledHeight = GetSprite()->getContentSize().height * GetSprite()->getScaleY();
	float scaledPlayerWidth = _playerRef->GetSprite()->getContentSize().width * _playerRef->GetSprite()->getScaleX();
	float scaledPlayerHeight = _playerRef->GetSprite()->getContentSize().height * _playerRef->GetSprite()->getScaleY();

	/*if (_playerRef->GetOrientationHorizontal()) {
		if (_playerRef->GetSprite()->getPositionX() - (scaledPlayerHeight / 2) < getSprite()->getPositionX() + (scaledWidth / 2)
			&& _playerRef->GetSprite()->getPositionX() + (scaledPlayerHeight / 2) > getSprite()->getPositionX() - (scaledWidth / 2)
			&& _playerRef->GetSprite()->getPositionY() - (scaledPlayerWidth / 2) < getSprite()->getPositionY() + (scaledHeight / 2)
			&& _playerRef->GetSprite()->getPositionY() + (scaledPlayerWidth / 2) > getSprite()->getPositionY() - (scaledHeight / 2))
		{
			_playerRef->GetSprite()->setPositionY(T.y);
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
			_woodenRef.at(i)->GetSprite()->setPositionY(T.y);
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
			_metalRef.at(i)->GetSprite()->setPositionY(T.y);
		}
	}
}

//Touch Functions
void MovableWall::onTouchBegan(Touch* touch, Event* event)
{
	GameManager::sharedGameManager()->setIsObjectTouched(true);
	Vec2 T = touch->getLocation();
	Rect platform = _touchZone->getBoundingBox();

	if (platform.containsPoint(T))
	{
		Selected();
	}
}

void MovableWall::onTouchEnded(Touch* touch, Event* event)
{
	GameManager::sharedGameManager()->setIsObjectTouched(false);
	UnSelected();
}

void MovableWall::onTouchMoved(Touch* touch, Event* event)
{

	Vec2 T = touch->getLocation();
	Rect platform = _touchZone->getBoundingBox();

	if (!_collidingWithPlatform || !_collidingWithWall)
	{
		if (platform.containsPoint(T) && _platformType == 1)
		{
			MovePlatformHorizontal(T);
		}
		else if (platform.containsPoint(T) && _platformType == 2)
		{
			MovePlatformVertical(T);
		}
	}
}

void MovableWall::onTouchCancelled(Touch* touch, Event* event)
{
	cocos2d::log("touch cancelled");
}