#include "MovablePlatform.h"

USING_NS_CC;

MovablePlatform::MovablePlatform(Player* ref, std::vector<Box*> wood, std::vector<Box*> metal) : GameObject(new ParticleModel())
{
	_playerRef = ref;
	_woodenRef = wood;
	_metalRef = metal;

	_touchZone = nullptr;
	_oScale = 0.0f;
	_nScale = 0.0f;
	_oTouchZoneYScale = 0.0f;
	_nTouchZoneYScale = 0.0f;

	_platformType = 0;
	_collidingWithPlatform = false;
	_collidingWithWall = false;
}

MovablePlatform::MovablePlatform(ParticleModel* particle, Player* ref, std::vector<Box*> wood, std::vector<Box*> metal) : GameObject(particle)
{
	_playerRef = ref;
	_woodenRef = wood;
	_metalRef = metal;

	_touchZone = nullptr;
	_oScale = 0.0f;
	_nScale = 0.0f;
	_oTouchZoneYScale = 0.0f;
	_nTouchZoneYScale = 0.0f;

	_platformType = 0;
	_collidingWithPlatform = false;
	_collidingWithWall = false;
}

MovablePlatform::~MovablePlatform()
{

}

void MovablePlatform::SetZoneSprite()
{
	_touchZone = Sprite::create("Art/Level_Main_Assets/Platforms/Platform_1.png");
	this->addChild(_touchZone);
	_touchZone->setPosition(GetSprite()->getPosition());
	_touchZone->setScaleX(GetSprite()->getScaleX() * 1.2);
	_touchZone->setScaleY(GetSprite()->getScaleY() * 3.5);
	_touchZone->setOpacity(75);

	_oTouchZoneYScale = _touchZone->getScaleY();
}

void MovablePlatform::Update(float delta)
{
	GameObject::Update(delta);
}

void MovablePlatform::SetSprite(cocos2d::Sprite* sprite)
{ 
	_oScale = sprite->getScale();

	GameObject::SetSprite(sprite);
}

void MovablePlatform::Selected()
{
	_nScale = (_oScale*1.2);
	_nTouchZoneYScale = (_oTouchZoneYScale * 6);

	GetSprite()->setScale(_nScale);
	_touchZone->setScaleY(_nTouchZoneYScale);
}

void MovablePlatform::UnSelected()
{
	GetSprite()->setScale(_oScale);
	_touchZone->setScaleY(_oTouchZoneYScale);
}

void MovablePlatform::MovePlatformHorizontal(cocos2d::Vec2 T)
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

void MovablePlatform::MovePlatformVertical(cocos2d::Vec2 T)
{
	if (_collidingWithPlatform)
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
void MovablePlatform::onTouchBegan(Touch* touch, Event* event)
{
	GameManager::sharedGameManager()->setIsObjectTouched(true);
	Vec2 T = touch->getLocation();
	Rect platform = _touchZone->getBoundingBox();

	if (platform.containsPoint(T))
	{
		Selected();
	}
}

void MovablePlatform::onTouchEnded(Touch* touch, Event* event)
{
	GameManager::sharedGameManager()->setIsObjectTouched(false);
	UnSelected();
}

void MovablePlatform::onTouchMoved(Touch* touch, Event* event)
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

void MovablePlatform::onTouchCancelled(Touch* touch, Event* event)
{
	cocos2d::log("touch cancelled");
}